#!/usr/bin/env python3
"""Check C++ include dependencies that keep the domain pure and queries read-only."""

from pathlib import Path
import re
import unittest


ROOT = Path(__file__).resolve().parents[1]
SOURCE_ROOTS = (ROOT / "include", ROOT / "src")
INCLUDE = re.compile(r'^\s*#\s*include\s*[<"]([^>"]+)[>"]', re.MULTILINE)
STANDARD_HEADERS = set("""
algorithm any array atomic barrier bit bitset charconv chrono codecvt compare complex concepts
condition_variable coroutine cassert cctype cerrno cfenv cfloat cinttypes ciso646 climits clocale
cmath csetjmp csignal cstdalign cstdarg cstdbool cstddef cstdint cstdio cstdlib cstring ctgmath
ctime cuchar cwchar cwctype deque exception execution expected filesystem format forward_list
fstream functional future initializer_list iomanip ios iosfwd iostream istream iterator latch
limits list locale map mdspan memory memory_resource mutex new numbers numeric optional ostream
print queue random ranges ratio regex scoped_allocator semaphore set shared_mutex source_location
span spanstream sstream stack stacktrace stdexcept stop_token streambuf string string_view strstream
syncstream system_error thread tuple type_traits typeindex typeinfo unordered_map unordered_set
utility valarray variant vector version
""".split())


def sources():
    return {path.resolve(): path.read_text() for root in SOURCE_ROOTS for path in root.rglob("*")
            if path.suffix in (".hpp", ".h", ".cpp", ".cc")}


def resolve_include(source, included, files):
    for candidate in (source.parent / included, *(root / included for root in SOURCE_ROOTS)):
        if candidate.resolve() in files:
            return candidate.resolve()
    return None


def in_layer(path, layer):
    return layer in path.relative_to(ROOT).parts


def is_append_dependency(path):
    return in_layer(path, "append") or "append" in path.stem.split("_")


class ArchitectureTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.files = sources()
        cls.graph = {path: {target for included in INCLUDE.findall(text)
                            if (target := resolve_include(path, included, cls.files)) is not None}
                     for path, text in cls.files.items()}

    def test_domain_depends_only_on_standard_library_and_domain(self):
        domain = [path for path in self.files if in_layer(path, "domain")]
        self.assertTrue(domain, "No domain source files found")
        for path in domain:
            for included in INCLUDE.findall(self.files[path]):
                with self.subTest(source=str(path.relative_to(ROOT)), included=included):
                    target = resolve_include(path, included, self.files)
                    if target is not None:
                        self.assertTrue(in_layer(target, "domain"), "Domain depends on an outer layer")
                    else:
                        self.assertIn(included, STANDARD_HEADERS, "Domain depends on a framework or external header")

    def test_query_has_no_transitive_dependency_on_append(self):
        query = [path for path in self.files if in_layer(path, "query")]
        self.assertTrue(query, "No query source files found")
        for source in query:
            pending = [source]
            visited = set()
            while pending:
                path = pending.pop()
                if path in visited:
                    continue
                visited.add(path)
                self.assertFalse(is_append_dependency(path),
                                 f"{source.relative_to(ROOT)} reaches append dependency {path.relative_to(ROOT)}")
                pending.extend(self.graph[path])

    def test_query_does_not_call_write_repository_methods(self):
        for path, text in self.files.items():
            if in_layer(path, "query"):
                code = re.sub(r'//[^\n]*|/\*.*?\*/|"(?:\\.|[^"\\])*"', "", text, flags=re.DOTALL)
                self.assertIsNone(re.search(r'(?:\.|->)\s*(?:save|save_all|save_and_flush|persist|merge|remove|delete_by_id)\s*\(', code),
                                  f"Query calls a write repository method in {path.relative_to(ROOT)}")


if __name__ == "__main__":
    unittest.main(verbosity=2)
