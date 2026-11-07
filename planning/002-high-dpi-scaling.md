# VS-002: Reliable Windows High-DPI Scaling

- Status: Planned
- Branch: `vibe-sweep`
- Depends on: [VS-001](001-windows-baseline.md)

## Goal

Make the Windows UI scale cleanly at fractional and integer display scales, including movement between monitors. Establish a consistent logical-coordinate policy before changing individual widgets.

## Repository findings

- [main.cpp](../src/core/main.cpp) already sets `Qt::AA_EnableHighDpiScaling` before application construction for Qt 5; its comment notes that Qt 6 enables scaling automatically.
- [lmms.exe.manifest](../cmake/nsis/lmms.exe.manifest) declares `dpiAware=false`, and [the NSIS CMake file](../cmake/nsis/CMakeLists.txt) installs it. Its actual effect on the running executable must be verified; this is a suspected conflict, not a proven root cause.
- [embed.cpp](../src/gui/embed.cpp) uses the primary screen DPR for SVG rasterization. Window-specific DPI and cached images need inspection.

## Implementation plan

1. Capture the baseline process DPI-awareness mode, Qt screen/window DPR, Windows compatibility overrides, and scaling environment variables. Compare installed and build-tree execution.
2. Trace embedded/external manifest handling, Qt platform startup, and packaging. Ensure consistent per-monitor awareness using the supported Qt/Windows mechanism; reconcile conflicting manifest metadata only after verifying which resource is effective.
3. Retain the existing Qt-version guards. Evaluate Qt 5 high-resolution pixmap support and fractional-scale rounding policy against the selected Qt versions. Avoid adding redundant Qt 6 switches or relying on global environment overrides as the product fix.
4. Use logical widget coordinates throughout. Apply DPR to raster backing-store resolution/metadata where necessary; do not multiply all widget geometry or fonts by DPR. Plan cache refresh on screen/DPI changes.
5. Review window bounds, popup placement, and asset loading. Put broader unrelated widget defects into follow-up issues.

## Validation and acceptance

- [ ] Effective process awareness and startup/manifest decisions documented.
- [ ] Application remains usable at 100%, 125%, 150%, and 200% Windows scaling, without doubled sizing or system-stretched whole-window blur.
- [ ] Mixed-DPI movement refreshes rendering and leaves windows, menus, and tooltips reachable; unavailable hardware checks stay pending.
- [ ] Main application and packaged/local installation behave consistently.
- [ ] Baseline Windows build and native suite pass again; Qt 5 compatibility is checked when that toolchain is available, with any gap recorded.
- [ ] Before/after screenshots and exact scale/DPR values retained.

## Reference and evidence

[Qt high-DPI documentation](https://doc.qt.io/qt-6/highdpi.html) explains logical coordinates, raster buffer DPR, and Windows behavior. Consult the documentation matching the actual Qt baseline during implementation.

Results: Not implemented or tested. Follow with [VS-003](003-vector-knobs-faders.md).
