# VS-002: Reliable Windows High-DPI Scaling

- Status: In progress
- Branch: `vibe-sweep`
- Depends on: [VS-001](001-windows-baseline.md)

## Goal

Make the Windows UI scale cleanly at fractional and integer display scales, including movement between monitors. Establish a consistent logical-coordinate policy before changing individual widgets.

## Repository findings

- [main.cpp](../src/core/main.cpp) already sets `Qt::AA_EnableHighDpiScaling` before application construction for Qt 5; its comment notes that Qt 6 enables scaling automatically.
- The former `cmake/nsis/lmms.exe.manifest` declared `dpiAware=false` and was installed alongside the executable. The installer also set the system-wide `PreferExternalManifest` preference. Their actual effect on the running process was not measured; a conflict remains suspected rather than proven.
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

Results: The installer no longer ships the external DPI-unaware manifest or sets the machine-wide `PreferExternalManifest` registry preference. It deletes a stale LMMS external manifest when installing over an existing copy; an existing registry preference is left untouched. Qt 5 now enables high-DPI pixmaps before application construction alongside its existing scaling attribute. Qt's platform integration determines process awareness; the effective mode still requires runtime measurement. SVG pixmaps still use the primary screen DPR and existing cached or retained pixmaps do not automatically refresh when windows move between screens.

The unchanged Windows baseline built successfully (1462/1462), passed all eight CTest tests and all three scripted checks. For this change, `git diff --check` and `git apply --check` on the draft passed. A two-stage CMake string evaluation produced the intended NSIS upgrade command `Delete "$INSTDIR\lmms.exe.manifest"`; the full installer and changed application have not yet been built or exercised. Windows scale and mixed-monitor checks, installed-versus-build-tree awareness, screenshots, and a Qt 5 build remain pending. Follow with [VS-003](003-vector-knobs-faders.md).
