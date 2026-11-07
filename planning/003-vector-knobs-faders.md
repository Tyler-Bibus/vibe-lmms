# VS-003: Crisp Vector Knob Arcs and Fader Ticks

- Status: In progress
- Branch: `vibe-sweep-vs-003`
- Depends on: [VS-001](001-windows-baseline.md), [VS-002](002-high-dpi-scaling.md)

## Goal

Use native QPainter vector rendering and antialiasing for crisp knob arcs, indicators, fader ticks, and control surfaces at Windows display scales. Preserve the controls' musical values and interactions.

## Repository findings

- [Knob.cpp](../src/gui/widgets/Knob.cpp), `drawKnob()`, already draws antialiased arcs/lines, but rasterizes them into `QImage(size(), ...)` without explicit DPR metadata. Legacy variants also draw a pixmap body. Adding antialiasing alone will not address that cache.
- [Fader.cpp](../src/gui/widgets/Fader.cpp) already paints meters and ticks with QPainter. `paintEvent()` still draws a pixmap handle; `paintFaderTicks()` does not explicitly enable antialiasing.
- Associated API and theme properties live in [Knob.h](../include/Knob.h) and [Fader.h](../include/Fader.h).

## Implementation plan

1. Inventory styled and legacy knob variants plus default/custom fader handles and their callers. Capture min/center/max and enabled/disabled appearances.
2. Draw knob arcs, indicators, and ticks using floating-point logical geometry and intentional pen widths/caps. Prefer drawing directly on the widget; if caching is retained, allocate at physical resolution, set DPR, and invalidate on size, DPI, palette, style, state, and value changes.
3. Replace default raster control surfaces that remain a source of blur with QPainter paths, shapes, and gradients. Preserve explicit custom artwork support or document a compatibility decision before removing it.
4. Enable antialiasing for the appropriate vector passes in Fader. Keep meter rectangles and tick alignment deliberate at fractional scales; indiscriminate smoothing can soften thin lines.
5. Preserve theme color properties, center/bipolar arcs, logarithmic mappings, fader zero-dB alignment, tick preference, hit areas, and sizing contracts. Keep input/model code unchanged unless a demonstrated rendering geometry defect requires it.

## Validation and acceptance

- [ ] Crisp arcs, ticks, and default handles at 100%, 125%, 150%, and 200%; no low-resolution cached-vector enlargement.
- [ ] All knob variants, extreme/center values, resize, disabled state, and monitor transitions checked.
- [ ] Fader ticks on/off, linear/dB controls, peaks, and zero-dB marker remain correct.
- [ ] Drag, wheel, fine adjustment, reset, automation, and context-menu interactions match VS-001.
- [ ] Rapid control movement and active mixer repainting remain responsive during playback; compare CPU/repaint behavior to baseline.
- [ ] Windows build/native tests pass; focused geometry tests are added only for new nontrivial logic.

Results: Knob arcs and indicators now paint directly with antialiasing in logical coordinates, without the low-resolution image cache. Legacy artwork sizing uses its logical size and disabled artwork retains its device pixel ratio. Fader ticks use antialiased strokes snapped to physical pixels; their dB mapping and zero marker remain unchanged. Input handling is unchanged.

Theme-provided knob bodies and default or custom fader handles remain artwork for compatibility, so raster assets may still blur when enlarged. Direct drawing may affect repaint performance. The unchanged Windows baseline passed its build, eight native tests, and three scripted checks. This change passed `git diff --check` and static review; the integrated build, visual checks at each scale, interactions, monitor transitions, and playback performance remain to be verified.
