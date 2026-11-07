# VS-001: Initial Windows Build and Test Baseline

- Status: Native build and tests passed; GUI/audio smoke pending
- Branch: `vibe-sweep-vs-001`
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

- [x] Unchanged source revision and reproducible environment recorded.
- [x] Native Windows configure/build succeeds; complete logs retained.
- [x] All native tests pass; counts and failures/skips are recorded.
- [x] Scripted checks pass, or explicit blockers are recorded and resolved before accepting the baseline.
- [ ] Local launch, demo playback, project round-trip, and screenshots recorded.
- [ ] Existing defects are documented separately from later regressions.

Results: Native build, all eight registered tests, and all scripted checks passed. GUI/audio smoke remains pending.

## Baseline execution (2026-11-07)

Source revision: `be5ac75e8d0fc43318d59452b785c623431f4c1d`. The application source and asset diff was empty; recursive submodules were initialized at their recorded revisions. Evidence is retained in the ignored `build/vibe-sweep/` directory of this worktree (`submodules.txt`, `source-diff.patch`, and logs). Host reports Windows build 26200; Visual Studio Community 2022 17.14.9 supplies MSVC 14.44.35207, CMake 3.31.6-msvc6 and Ninja 1.12.1. Qt 6.8.3 MSVC x64 was installed locally with aqtinstall 3.3.0. Workspace-local vcpkg revision is `6fdd23fa0a9479a9f9a1c48178696f60cf39255d` (tool 2026-10-29). Configure follows the MSVC CI settings with compile cache omitted; see `configure.log` for actual output.

Scripted checks: `check-strings` and `check-namespace` passed on Windows with Python 3.13.3 and tinycss2 1.5.1 (zero errors each). `verify` initially failed on Windows with `WinError 32` while removing a temporary directory. It passed in WSL Ubuntu with an isolated `GIT_CONFIG_GLOBAL`; expected fixture errors in the log are assertions, followed by `SUCCESS`. No persistent global Git configuration was changed.

The first configure completed dependency installation but failed because the installed MSVC 19.44 is below the project minimum 19.51. A workspace-local MSVC 14.51.36231 (compiler 19.51) was extracted from Microsoft VS 2026 channel packages; it uses the installed Windows SDK 10.0.26100 and the already built 14.44 vcpkg libraries. Local MSYS2 Perl 5.42.3 and its List::MoreUtils/XML::Parser modules satisfy the LADSPA generator. The portable setup disables the optional 32-bit VST bridge (`WANT_VST_32=OFF`); Qt 6, x64, RelWithDebInfo, official target architecture, and warnings-as-errors match the CI baseline. Compile caching was omitted. `configure-1451-perl.log` records successful configure and `build-1451.log` records all 1,462 build tasks completing successfully. `ctest.log` records 8/8 registered native tests passed, with matching CRT and Qt DLLs on PATH and `QT_QPA_PLATFORM=offscreen`. No tests were skipped. `install.log` and `windeployqt.log` record the staged installation; matching 14.51 CRT DLLs were copied app-local. The saved install is `build/vibe-sweep/baseline-install/lmms.exe`; use `QT_QPA_PLATFORM=windows` and an isolated config for GUI smoke. GUI launch, demo playback, project round-trip and screenshots remain pending; no claim of audio verification is made.
