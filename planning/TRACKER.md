# Vibe Sweep Tracker

This folder is the internal issue tracker for this personal LMMS fork. Each issue has a stable ID and its own Markdown file. Keep this index and the corresponding issue status synchronized.

## Current scope

- Branch: `vibe-sweep`
- Baseline source revision: `458c10754186d96eec963e1b2a38fbccbf581a6e`.
- Phase: planning only. Creating these documents does not start or complete implementation.
- No build, dependency installation, test run, source change, or theme change has been performed for this sweep.
- Start implementation only when requested. First build and test the unchanged C++ baseline, then proceed in the order below.

## Issues

| Order | ID | Target | Status | Prerequisite |
| --- | --- | --- | --- | --- |
| 1 | [VS-001](001-windows-baseline.md) | Initial Windows build, test suite, and launch smoke test | Planned | None |
| 2 | [VS-002](002-high-dpi-scaling.md) | Reliable Windows high-DPI scaling | Planned | VS-001 baseline accepted |
| 3 | [VS-003](003-vector-knobs-faders.md) | Crisp vector knob arcs and fader ticks | Planned | VS-002 |
| 4 | [VS-004](004-dpi-aware-text.md) | DPI-aware LCD labels and floating text | Planned | VS-002, VS-003 |
| 5 | [VS-005](005-cyberpunk-default-theme.md) | Portfolio-inspired cyberpunk default theme | Planned | VS-002 through VS-004 |

## Workflow

Use `Planned`, `In progress`, `Blocked`, or `Done`. Prerequisites describe execution order, not work already started. Before starting an issue, update both documents. Record blockers, decisions, commands, results, and relevant commit IDs in that issue. Mark it done only after its acceptance checklist is satisfied and evidence is recorded.

New issues use the next unused number and `NNN-short-description.md`; never reuse IDs. Link discoveries and follow-ups from their originating issue and add them to this table. A failed baseline stays recorded as a failure; track any required source repair separately and rerun the baseline after the repair.

## Shared verification

For UI work, compare Windows scaling at 100%, 125%, 150%, and 200%, plus mixed-DPI monitor movement where hardware permits. Record actual Windows scale, Qt version, window DPR, theme, and source revision with screenshots. Mark unavailable hardware checks explicitly as pending.

Use the same toolchain and representative project throughout the sweep. Rerun the native test suite after C++ changes and at final integration. Check Song Editor, Piano Roll, Mixer, instrument windows, dialogs, tooltips, and keyboard focus. Compare playback and control interactions against VS-001.

Store generated logs, screenshots, and builds under ignored `build/vibe-sweep/<issue-id>/`; retain concise outcomes in the issue Markdown. Commit durable review images deliberately if needed. Do not mark proposed checks as passed.

Documentation changes do not alter the baseline application source. At execution time, record the current commit and verify that source/assets still match the baseline above.
