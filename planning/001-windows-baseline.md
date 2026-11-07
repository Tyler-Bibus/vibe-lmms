# VS-001: Initial Windows Build and Test Baseline

- Status: Planned
- Branch: `vibe-sweep`
- Prerequisites: implementation requested by the owner
- Next: [VS-002](002-high-dpi-scaling.md)

## Goal

Build the current, unchanged C++ application on Windows, run its test suite, and verify that it launches and plays audio before making application or theme changes. Establish reproducible evidence for every subsequent issue.

## Repository findings

The `msvc` job in [build.yml](../.github/workflows/build.yml) uses x64 MSVC, Ninja, Qt 6.8, and vcpkg's `x64-windows` triplet. This is the primary Windows baseline, although the repository defaults to Qt 5. All checked submodules were uninitialized during planning. No local toolchain availability, build success, or test result has been established.

## Execution plan

1. Record `git rev-parse HEAD`, source diff, Windows version, compiler, CMake, Ninja, Qt, and vcpkg versions/revisions. Confirm application sources and assets match the tracker baseline; planning documents may differ.
2. Initialize submodules with `git submodule update --init --recursive`. Record their revisions. Use an x64 MSVC developer shell and a Qt installation matching the compiler architecture.
3. Configure and build outside the source tree using the example below, adapting installation paths. Follow CI's dependency manifest. Record all option deviations; compiler caching is optional.
4. Enumerate and run all native tests. Eight test executables are currently listed in [tests/CMakeLists.txt](../tests/CMakeLists.txt); investigate missing or unregistered tests rather than accepting an empty run.
5. Run the scripted checks from `.github/workflows/checks.yml` in a compatible Python/Unix environment with `tinycss2`. Run `verify` in an isolated environment: it changes Git configuration, including global `protocol.file.allow`. Isolate its global Git config rather than changing the owner's persistent settings. Report native and scripted results separately.
6. Stage a local installation and launch `lmms.exe`. Open the editors and Mixer, load a bundled demo, play/stop audio, move a knob/fader, save/reopen a temporary project, then quit. Record audio backend, missing optional plugins, warnings, and baseline screenshots.

## Proposed PowerShell commands

These commands are a runbook, not completed actions. Supply real paths first.

```powershell
$vcpkgRoot = "<vcpkg-root>"
$qtRoot = "<Qt-6.8-msvc-x64-root>"
cmake -S . -B build/vibe-sweep/baseline -G Ninja `
  --toolchain "$vcpkgRoot/scripts/buildsystems/vcpkg.cmake" `
  -DCMAKE_PREFIX_PATH="$qtRoot" -DCMAKE_BUILD_TYPE=RelWithDebInfo `
  -DWANT_QT6=ON -DTARGET_UARCH=official -DUSE_WERROR=ON `
  -DVCPKG_TARGET_TRIPLET=x64-windows -DVCPKG_HOST_TRIPLET=x64-windows
cmake --build build/vibe-sweep/baseline --parallel 4
ctest --test-dir build/vibe-sweep/baseline/tests -N
ctest --test-dir build/vibe-sweep/baseline/tests --output-on-failure --no-tests=error -j2
cmake --install build/vibe-sweep/baseline --prefix "$PWD/target/vibe-sweep-baseline"
```

Resolve runtime DLL and Qt platform-plugin discovery using the matching Qt/vcpkg environment or existing packaging logic. Do not mistake a missing runtime dependency for a completed launch test.

## Acceptance and evidence

- [ ] Unchanged source revision and reproducible environment recorded.
- [ ] Native Windows configure/build succeeds; complete logs retained.
- [ ] All native tests pass; counts and failures/skips are recorded.
- [ ] Scripted checks pass, or explicit blockers are recorded and resolved before accepting the baseline.
- [ ] Local launch, demo playback, project round-trip, and screenshots recorded.
- [ ] Existing defects are documented separately from later regressions.

Results: Not run. This issue gates application changes in VS-002 through VS-005.
