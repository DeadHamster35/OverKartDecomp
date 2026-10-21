#!/usr/bin/env python3
"""Fail if src/OverKartLibrary/ differs across the three trees.

    python tools/check_library_shared.py

Baseline is LibraryDecomp. AmpedUpDecomp and OverKartDecomp are compared to it
and to each other. Generated .s files are skipped. CRLF is treated as equal to LF.

OverKartDefaults.c is host-owned and lives outside this subtree.
StockAliases.h stays host-owned until Stage 7 deletes it.
Amped MainInclude.h omits actors.h / actors_extended.h (Stage 1.5 Object* item actors).
S9 expected: exit 0.
"""

from __future__ import print_function

import hashlib
import os
import sys

REPOS = ("LibraryDecomp", "AmpedUpDecomp", "OverKartDecomp")
SUBTREE = os.path.join("src", "OverKartLibrary")
ALLOW = frozenset({"StockAliases.h", "MainInclude.h"})
SKIP_EXT = frozenset({".s"})
SKIP_NAMES = frozenset({".DS_Store"})


def code_root():
    here = os.path.abspath(__file__)
    repo = os.path.dirname(os.path.dirname(here))
    return os.path.dirname(repo)


def repo_path(root, name):
    path = os.path.join(root, name)
    if not os.path.isdir(path):
        sys.exit("error: missing sibling repo %s at %s" % (name, path))
    return path


def file_digest(path):
    with open(path, "rb") as handle:
        data = handle.read().replace(b"\r\n", b"\n")
    return hashlib.sha1(data).hexdigest()


def collect(repo):
    root = os.path.join(repo, SUBTREE)
    out = {}
    if not os.path.isdir(root):
        return out
    for dirpath, _dirnames, filenames in os.walk(root):
        for name in filenames:
            ext = os.path.splitext(name)[1].lower()
            if ext in SKIP_EXT or name in SKIP_NAMES:
                continue
            path = os.path.join(dirpath, name)
            rel = os.path.relpath(path, root).replace("\\", "/")
            out[rel] = file_digest(path)
    return out


def allow_key(rel):
    return os.path.basename(rel)


def main():
    root = code_root()
    hashes = {}
    for name in REPOS:
        hashes[name] = collect(repo_path(root, name))
        print("%-16s %4d files under %s" % (name, len(hashes[name]), SUBTREE))

    failures = 0
    pairs = (
        ("LibraryDecomp", "AmpedUpDecomp"),
        ("LibraryDecomp", "OverKartDecomp"),
        ("AmpedUpDecomp", "OverKartDecomp"),
    )
    for a, b in pairs:
        all_rels = sorted(set(hashes[a]) | set(hashes[b]))
        only_a = sorted(set(hashes[a]) - set(hashes[b]))
        only_b = sorted(set(hashes[b]) - set(hashes[a]))
        differing = []
        allowed = []
        for rel in all_rels:
            if rel in only_a or rel in only_b:
                continue
            if hashes[a][rel] == hashes[b][rel]:
                continue
            if allow_key(rel) in ALLOW:
                allowed.append(rel)
            else:
                differing.append(rel)

        print("\n%s vs %s" % (a, b))
        print("  common     %d" % (len(set(hashes[a]) & set(hashes[b]))))
        print("  only in %s: %d" % (a, len(only_a)))
        print("  only in %s: %d" % (b, len(only_b)))
        print("  allowed    %d" % len(allowed))
        print("  differing  %d" % len(differing))
        for rel in allowed:
            print("    allowed  %s" % rel)
        for rel in only_a:
            print("    only-%s  %s" % (a, rel))
            if allow_key(rel) not in ALLOW:
                failures += 1
        for rel in only_b:
            print("    only-%s  %s" % (b, rel))
            if allow_key(rel) not in ALLOW:
                failures += 1
        for rel in differing:
            print("    differ   %s" % rel)
            failures += 1

    if failures:
        print("\nFAIL: %d Library file(s) differ" % failures)
        return 1
    print("\nPASS: Library copies match")
    return 0


if __name__ == "__main__":
    sys.exit(main())
