# Vibe Sweep Tracker

This folder is the internal issue tracker for this personal LMMS fork. Each issue has a stable ID and its own Markdown file. Keep this index and the corresponding issue status synchronized.

## Current scope

- Branch: `vibe-sweep`
- Baseline source revision: `458c10754186d96eec963e1b2a38fbccbf581a6e`.
- Phase: all five issue branches implemented and merged; automated integration verification passed. Interactive GUI acceptance remains pending.
- Documentation was committed as `be5ac75e8`. Each issue has an isolated worktree on `vibe-sweep-vs-NNN`.
- The unchanged baseline built and passed its native suite before feature patches were applied. The tested integrated source revision is `0b3f38b4aa6fe6130031fe9fa6d2b15e60e5c1e2`.
- Issue statuses remain `In progress` while the unchecked interactive acceptance criteria remain outstanding; this does not mean implementation is missing.

## Issues

| Order | ID | Target | Status | Issue commit |
| --- | --- | --- | --- | --- |
| 1 | [VS-001](001-windows-baseline.md) | Initial Windows build, tests, and smoke evidence | In progress | `da094a53b` |
| 2 | [VS-002](002-high-dpi-scaling.md) | Windows high-DPI setup | In progress | `00efbe5f9` |
| 3 | [VS-003](003-vector-knobs-faders.md) | Vector knob arcs and fader ticks | In progress | `8d7439dc6` |
| 4 | [VS-004](004-dpi-aware-text.md) | DPI-aware LCD labels and floating text | In progress | `a3787474b` |
| 5 | [VS-005](005-cyberpunk-default-theme.md) | Cyberpunk default theme | In progress | `922f94311` |

Dependencies were preserved at integration: VS-001, VS-002, VS-003, VS-004, then VS-005. Each issue was developed by a Terra subagent in its own worktree; the original issue branches and baseline installation are retained.

## Verified results (2026-11-07)

| Check | Result |
| --- | --- |
| Unchanged Windows baseline | 1,462 build steps completed; 8/8 native tests passed |
| Integrated Windows build | Passed; 9/9 native tests passed, including `LcdWidgetTest` |
| LCD offscreen DPI checks | Passed at `QT_SCALE_FACTOR=1`, `1.25`, `1.5`, and `2` |
| Scripted checks | `verify`, `check-strings`, and `check-namespace` passed on integrated source |
| Audio render smoke | Baseline and integrated copies of `Root84-TrancyLoop.mmpz` rendered successfully to non-silent stereo 16-bit WAV, 44.1 kHz, 477,440 frames each |
| Formatting and static checks | Changed C++ regions formatted; Git whitespace, stylesheet syntax, resource references, and selected contrast pairs checked |

The portable x64 build uses MSVC 14.51.36231, Qt 6.8.3, Windows SDK 10.0.26100, and the initialized submodules. The installed MSVC 19.44 was rejected by the repository's 19.51 minimum; no compiler requirement was weakened. The optional 32-bit VST bridge is disabled because it requires a registered Visual Studio 2026 generator. Script `verify` ran in WSL with isolated Git configuration after Windows temporary-directory cleanup failed.

Evidence and separate runnable installations are retained locally under `build/vibe-sweep/worktrees/vs-001/build/vibe-sweep/`: `integration-*.log`, `audio-smoke/`, `baseline-install/`, and `integration-install/`. Staging required app-local Qt, vcpkg runtime DLLs, and matching MSVC CRT DLLs. These generated artifacts are intentionally ignored by Git.

## Remaining acceptance and known limits

- The Windows Computer Use helper could not connect to its native pipe, including after retry and kernel reset. Screenshots, interactive controls, theme switching, real monitor movement, process-awareness measurement, and audio-device playback are unverified. Offscreen scale-factor tests do not replace physical-monitor checks.
- Both baseline and integrated audio logs report Carla plugin DLL load failures. This is a retained baseline limitation, not a newly introduced rendering failure.
- Audio WAV hashes differ between builds, and also between two unchanged baseline renders. Successful non-silent output and equal frame counts are the smoke-test evidence; bitwise equality is not an oracle for this project.
- Qt 5 and the optional 32-bit VST bridge were not built. Existing raster artwork and primary-screen SVG caches remain limitations; see VS-002 through VS-005 for details.

## Workflow

Use `Planned`, `In progress`, `Blocked`, or `Done`. Prerequisites describe execution order, not work already started. Before starting an issue, update both documents. Record blockers, decisions, commands, results, and relevant commit IDs in that issue. Mark it done only after its acceptance checklist is satisfied and evidence is recorded.

New issues use the next unused number and `NNN-short-description.md`; never reuse IDs. Link discoveries and follow-ups from their originating issue and add them to this table. A failed baseline stays recorded as a failure; track any required source repair separately and rerun the baseline after the repair.

## Shared verification

For UI work, compare Windows scaling at 100%, 125%, 150%, and 200%, plus mixed-DPI monitor movement where hardware permits. Record actual Windows scale, Qt version, window DPR, theme, and source revision with screenshots. Mark unavailable hardware checks explicitly as pending.

Use the same toolchain and representative project throughout the sweep. Rerun the native test suite after C++ changes and at final integration. Check Song Editor, Piano Roll, Mixer, instrument windows, dialogs, tooltips, and keyboard focus. Compare playback and control interactions against VS-001.

Store generated logs, screenshots, and builds under ignored `build/vibe-sweep/<issue-id>/`; retain concise outcomes in the issue Markdown. Commit durable review images deliberately if needed. Do not mark proposed checks as passed.

Documentation changes do not alter the baseline application source. At execution time, record the current commit and verify that source/assets still match the baseline above.
