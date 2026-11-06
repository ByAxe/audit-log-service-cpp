#!/usr/bin/env python3
"""Reveal the session-2 pagination incident in a fresh, disposable database."""
import json
import os
from pathlib import Path
import sys

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "tests"))
from integration import database, running_service


def main():
    if os.environ.get("AUDIT_LOG_TEST_DB_URL"):
        raise SystemExit("Unset AUDIT_LOG_TEST_DB_URL: this demo only uses its own temporary database.")
    binary = Path(sys.argv[1] if len(sys.argv) > 1 else ROOT / "build/audit-log-service").resolve()
    if not binary.is_file():
        raise SystemExit("Build the service first: cmake -S . -B build && cmake --build build -j2")

    with database() as service, running_service(service, binary):
        def query(page, size=3):
            status, _, result = service.request(params={"page": page, "size": size})
            assert status == 200, (status, result)
            return result

        def ids(page):
            return [event["id"] for event in page["content"]]

        initial = query(0, 200)
        first = query(0)
        control = query(1)
        assert first["totalElements"] == control["totalElements"] == 12
        assert not set(ids(first)) & set(ids(control)), "Control pages unexpectedly overlap"
        print(json.dumps({"control": [ids(first), ids(control)], "totalElements": 12}))

        status, _, created = service.request(method="POST", body={
            "actor": "session-2@example.test", "action": "LOGIN", "outcome": "ALLOWED"
        })
        assert status == 201, (status, created)
        second = query(1)
        assert second["totalElements"] == first["totalElements"] + 1
        repeated = set(ids(first)) & set(ids(second))
        assert repeated, "The new event did not move ahead of the first page; check the server clock and branch."

        pages = [first, second, query(2), query(3)]
        seen = {event_id for page in pages for event_id in ids(page)}
        missing = set(ids(initial)) - seen
        assert missing == {1}, ("Expected the original four-page scan to miss ID 1", missing)
        assert missing <= set(ids(query(4))), "The skipped event must still be readable"
        assert set(ids(initial)) <= set(ids(query(0, 200))), "An original event is no longer readable"
        print(json.dumps({
            "afterPost": {"page": 1, "ids": ids(second), "totalElements": second["totalElements"]},
            "fourPageScan": [ids(page) for page in pages],
            "repeatedIds": sorted(repeated), "missedIds": sorted(missing),
            "page4": ids(query(4)), "originalEventsStillReadable": True,
            "result": "SESSION_2_BUG_REPRODUCED"
        }))


if __name__ == "__main__":
    main()
