#!/usr/bin/env python3
"""Exercise migration history, restart safety, and refusal paths on a throwaway DB."""

import os
from pathlib import Path
import shutil
import subprocess
import sys
import tempfile
import unittest

from integration import ROOT, database, free_port, running_service


class MigrationTests(unittest.TestCase):
    binary = None

    def assert_startup_refused(self, service, migrations, message):
        result = subprocess.run(
            [str(self.binary)], cwd=ROOT, text=True, capture_output=True, timeout=10,
            env={**os.environ, "AUDIT_LOG_DB_URL": service.db_url,
                 "AUDIT_LOG_MIGRATIONS": str(migrations), "AUDIT_LOG_HOST": "127.0.0.1",
                 "AUDIT_LOG_PORT": str(free_port())},
        )
        output = (result.stdout + result.stderr).replace(service.db_url, "[test database]")
        self.assertNotEqual(result.returncode, 0, output)
        self.assertIn(message, output)

    @staticmethod
    def snapshot(service):
        return tuple(service.sql(statement).stdout.strip() for statement in (
            "select jsonb_agg(to_jsonb(event) order by id)::text from audit_events event",
            "select jsonb_agg(to_jsonb(history) order by version)::text from audit_schema_history history",
            "select last_value::text || ':' || is_called::text from audit_events_id_seq",
        ))

    def test_restart_checksum_and_existing_schema(self):
        with database() as service:
            with self.subTest("first startup"):
                with running_service(service, self.binary):
                    self.assertEqual(service.request()[2]["totalElements"], 12)
                    self.assertEqual(service.sql("select version from audit_schema_history order by version").stdout.strip(), "1\n2\n3")
                    original = self.snapshot(service)
                    self.assertEqual(original[2], "12:true")

            with self.subTest("restart does not reseed or advance sequence"):
                with running_service(service, self.binary):
                    self.assertEqual(self.snapshot(service), original)

            with self.subTest("changed applied migration is refused"):
                with tempfile.TemporaryDirectory(prefix="audit-cpp-migration-check-") as temporary:
                    migrations = Path(temporary) / "migration"
                    shutil.copytree(ROOT / "db" / "migration", migrations)
                    with (migrations / "V1__create_audit_events.sql").open("a") as file:
                        file.write("\n-- checksum must catch this change\n")
                    self.assert_startup_refused(service, migrations, "Applied migration is missing or changed: V1")
                    self.assertEqual(self.snapshot(service), original)

            with self.subTest("existing schema without C++ history is refused"):
                service.sql("drop table audit_schema_history")
                self.assert_startup_refused(service, ROOT / "db" / "migration",
                                            "Existing audit_events has no C++ migration history")
                self.assertEqual(service.sql("select count(*) from audit_events").stdout.strip(), "12")
                self.assertEqual(service.sql("select last_value from audit_events_id_seq").stdout.strip(), "12")
                self.assertEqual(service.sql("select to_regclass('audit_schema_history') is null").stdout.strip(), "t")


def main():
    MigrationTests.binary = Path(sys.argv[1] if len(sys.argv) > 1 else ROOT / "build" / "audit-log-service").resolve()
    if not MigrationTests.binary.is_file():
        raise RuntimeError(f"Build the service first; executable not found: {MigrationTests.binary}")
    suite = unittest.defaultTestLoader.loadTestsFromTestCase(MigrationTests)
    return 0 if unittest.TextTestRunner(verbosity=2).run(suite).wasSuccessful() else 1


if __name__ == "__main__":
    try:
        sys.exit(main())
    except (OSError, RuntimeError) as error:
        print(f"Migration test setup failed: {error}", file=sys.stderr)
        sys.exit(1)
