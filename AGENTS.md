# Repository Guidelines

## Project Structure & Module Organization

This personal fork contains LMMS, a cross-platform digital audio workstation written in C++20 with Qt. Application code lives in `src/core/`, `src/gui/`, and `src/tracks/`; shared headers are in `include/`. Instrument and effect implementations live in `plugins/`. Bundled dependencies reside in `src/3rdparty/`, including Git submodules. Assets, translations, presets, and demo projects are under `data/`. Tests live in `tests/src/` and `tests/scripted/`. Build support is in `cmake/` and `buildtools/`; CI workflows are in `.github/workflows/`.

## Build, Test, and Development Commands

Install platform dependencies first; use `INSTALL.txt`, `vcpkg.json`, and the matching CI build job as references. Qt 5.15 is the default baseline; Qt 6 support is experimental.

- `git submodule update --init --recursive`: fetch required bundled dependencies.
- `cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug`: configure an out-of-tree development build. Windows may require generator and toolchain arguments from CI.
- `cmake --build build --parallel`: compile the application, plugins, and tests.
- `ctest --test-dir build/tests --output-on-failure`: run unit tests.
- `cmake --install build --prefix "<local-prefix>"`: stage a local installation; launch LMMS from its `bin/` directory.

For multi-configuration generators, add `--config Debug` to build/install commands and `-C Debug` to CTest.

## Coding Style & Naming Conventions

Follow `.editorconfig`: tabs with width four. C++ formatting follows `.clang-format`, including a 120-column limit; format changed files with `clang-format -i <file>`. Use `CamelCase` types, `camelCase` functions, and existing `m_` member naming. Preserve surrounding conventions and license headers. Static-analysis settings are in `.clang-tidy`; CI also uses ShellCheck and yamllint.

## Testing Guidelines

Use Qt Test with `QCOMPARE`/`QVERIFY`. Name files `<Component>Test.cpp`, place them under the matching `tests/src/` directory, and register them in `tests/CMakeLists.txt`. Add regression coverage for changed behavior; no numeric coverage threshold is configured. Run `tests/scripted/verify`, `tests/scripted/check-strings`, and `tests/scripted/check-namespace` in a compatible Unix environment; string checks require Python's `tinycss2`. Manually verify affected GUI and audio behavior.

## Commit & Pull Request Guidelines

History uses short imperative subjects, such as `Fix missing scale & chord translations`; merged upstream commits often include `(#1234)`. Keep commits focused. Describe the problem, resulting behavior, validation, and relevant issues in PRs; include screenshots for visible changes.

## Fork & Agent Guidance

Agent-assisted contributions are welcome. Follow the owner's requested scope; upstream requests to open an issue before major work do not require upstream approval for this fork.
