# VS-004: DPI-Aware LCD Labels and Floating Text

- Status: In progress
- Branch: `vibe-sweep`
- Depends on: [VS-002](002-high-dpi-scaling.md), [VS-003](003-vector-knobs-faders.md)

## Goal

Replace inappropriate fixed pixel typography with dynamic sizing that responds to the active display's DPI and the application font. Keep LCD labels and floating text readable without clipping or applying Qt's scale twice.

## Repository findings

- [LcdWidget.cpp](../src/gui/widgets/LcdWidget.cpp) uses `adjustedToPixelSize(..., DEFAULT_FONT_SIZE)` for labels and adds a fixed nine logical units for label height. Its digits are pixmap-atlas cells, not text glyphs.
- [FontHelper.h](../include/FontHelper.h) defines pixel-size constants and a `setPixelSize()` helper used by other widgets; a global change could expand this issue's scope substantially.
- [TextFloat.cpp](../src/gui/widgets/TextFloat.cpp) currently uses QLabels with inherited fonts and only a bold-title stylesheet. No hardcoded pixel font size was found there. Trace the effective inherited/theme font before prescribing a replacement.
- Knob/fader value popups also use [SimpleTextFloat.cpp](../src/gui/widgets/SimpleTextFloat.cpp); include them in the audit and validation.

## Implementation plan

1. Record effective application/widget fonts, font metrics, logical DPI, and window DPR at each test scale. Separate small-font defects from bitmap-digit blur.
2. Define a shared sizing policy for these targets using inherited or point-based fonts and QFontMetricsF for layout. Tie raster resolution to window DPR and let Qt map logical text sizes to physical pixels. If an explicit physical-pixel conversion is necessary, apply it once and explain why.
3. Update LcdWidget label measurement and painting together. Replace fixed label-height allowances with metrics, preserving borders, seamless cells, negative values, leading zeros, and spinbox composition.
4. Make TextFloat title/body fonts, margins, wrapping, and positioning respond to font/screen changes where the audit identifies a defect. Preserve correct inherited scaling instead of adding a multiplier to working QLabels.
5. Check LCD atlas logical dimensions and raster DPR handling. Treat replacing the digit artwork/font system as a separate follow-up if required; larger labels alone will not sharpen atlas digits.

## Validation and acceptance

- [ ] LCD labels and floating text are legible and unclipped at all shared Windows scales and after monitor movement.
- [ ] Long/localized labels, negative values, digit counts, title/body combinations, and empty fields fit correctly.
- [ ] Popup geometry, cursor/parent placement, screen bounds, timeout, and dismissal behavior remain correct.
- [ ] No double scaling; application/system font changes produce consistent geometry.
- [ ] Scope of any FontHelper or SimpleTextFloat change is documented; unrelated users are checked if shared behavior changes.
- [ ] Build/native tests pass; screenshots and measured font/DPR values recorded.

Reference: [Qt high-DPI model](https://doc.qt.io/qt-6/highdpi.html).

Results: Implemented inherited-font LCD label sizing and painting with font metrics, including space for descenders and shadow, and refreshes geometry on font and screen changes. TextFloat retains inherited fonts, wraps text to the target screen width, and repositions when its font or screen changes. SimpleTextFloat already inherits its font and remains unchanged. The LCD digit atlas remains raster artwork and was not replaced.

Validation: Added an offscreen Qt regression test using a synthetic LCD atlas for font-dependent geometry and observable digit rendering. The unchanged Windows baseline passed before implementation. The new build and test, mixed-monitor behavior, screenshots, and measured font/DPR values remain pending integrated native and visual verification. Very large icons or content taller than the screen may still exceed available geometry.
