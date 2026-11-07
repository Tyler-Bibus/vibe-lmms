# VS-005: Portfolio-Inspired Cyberpunk Default Theme

- Status: In progress
- Branch: `vibe-sweep-vs-005`
- Depends on: [VS-002](002-high-dpi-scaling.md), [VS-003](003-vector-knobs-faders.md), [VS-004](004-dpi-aware-text.md)

## Goal and reference

Modernize [data/themes/default/style.css](../data/themes/default/style.css) with the visual language of the owner's [portfolio](https://tyler-bibus.github.io/personal/): near-black violet surfaces, electric purple and hot magenta accents, crisp panel edges, and readable technical typography.

The live page could not be rendered by the web fetch during planning. The portfolio's [index.css](https://github.com/Tyler-Bibus/personal/blob/main/portfolio/src/index.css) imports [styles/cyber.css](https://github.com/Tyler-Bibus/personal/blob/main/portfolio/src/styles/cyber.css), which was inspected as the design source. Confirm the live appearance before implementation; these values are source-derived, not a screenshot comparison.

## Proposed palette mapping

| Role | Portfolio values | LMMS use |
| --- | --- | --- |
| Base | `#04010a`, `#080312` | Workspace and recessed backgrounds |
| Panels | `#0b0616`, `#100a1f` | Editor surfaces, menus, dialogs |
| Borders | `#2b1149`, `#5c1c90` | Panel separation and active edges |
| Primary accent | `#ff2bd6` | Selection, focus, active controls |
| Secondary accent | `#b026ff`, `#8b5cf6` | Secondary highlights and grids |
| Text | `#ece4ff`, `#b6a7cf` | Primary and secondary labels |

## Implementation plan

1. Capture baseline selectors, widget states, `resources:` image references, palette behavior, and `qproperty-*` colors. Identify custom-painted surfaces that CSS cannot reach; record follow-ups rather than assuming every element is stylable.
2. Apply a consistent hierarchy to editors, browser, Mixer, menus, dialogs, inputs, tabs, scrollbars, tooltips, LCD labels, and knobs. Use restrained borders and supported Qt gradients for the neon character.
3. Define normal, hover, pressed, selected, checked, disabled, and keyboard-focus states explicitly. Preserve semantic recording, warning, and meter/clipping colors so accent colors do not obscure meaning.
4. Translate the design to Qt style sheets. Do not paste browser CSS variables, web animations, `clip-path`, or glow effects into QSS. Document palette roles in comments; use supported declarations and existing properties. See [Qt stylesheet syntax](https://doc.qt.io/qt-6/stylesheet-syntax.html).
5. Keep dense DAW text readable. The portfolio uses Audiowide, Rajdhani, and Share Tech Mono; use available local fallbacks initially. Any bundled-font proposal needs a separate resource/dependency decision. Apply VS-004 typography consistently.
6. Keep this issue focused on the default theme; preserve classic-theme behavior and existing image paths. Avoid animated scanlines/glitches over editors or meters.

## Validation and acceptance

- [ ] Screenshot comparisons show a coherent portfolio-inspired palette across the major windows at all shared DPI scales.
- [ ] Text and active states have measured readable contrast; aim for 4.5:1 for normal text and 3:1 for essential control boundaries/focus indicators.
- [ ] Selection, keyboard focus, disabled controls, warning/record states, meters, and grid hierarchy remain distinguishable.
- [ ] No unsupported-style warnings, broken resource references, clipping, or layout regressions.
- [ ] `tests/scripted/check-strings` and final build/native tests pass; demo playback and interaction smoke checks pass.
- [ ] Theme loading/restart and switching to classic and back are checked; final screenshots and any known gaps are recorded.

Results: The default QSS now uses violet surfaces, lavender text, purple selection and grids, magenta focus, and accented default knobs. Existing instrument artwork colors and recording, clipping, and peak-meter colors are retained. All 24 referenced stylesheet images exist, and tinycss2 reports no stylesheet syntax errors. The post-change check-strings run could not start because the Carla submodule is absent from this worktree; it remains an integration check. The proposed primary text/panel contrast is 15.76:1, selected text/background 10.41:1, magenta focus/panel 6.05:1, and scrollbar handle/panel 3.23:1. Piano roll regular and selected note text contrast is at least 6.25:1 and 5.06:1 respectively at full brightness, and 10.41:1 and 9.02:1 at the configured opacity over the editor background.

The baseline build, eight native tests, and three scripted checks passed before this change. Runtime Qt parsing, screenshots, theme switching, demo playback, and DPI checks remain for integration. Existing red step notes and pink current-step notes may have less than 4.5:1 text contrast at high volume because the painter normalizes color brightness; preserving their semantic colors leaves this for visual review or a code-level solution. Custom-painted surfaces and existing bitmap artwork may still retain their original colors. Final integration completes the initial sweep only after all five issue checklists have evidence.
