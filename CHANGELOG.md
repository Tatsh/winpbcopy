<!-- markdownlint-configure-file {"MD024": { "siblings_only": true } } -->

# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/), and this project
adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

## [0.0.6] - 2026-05-04

### Added

- Published 32-bit MinGW (i686) installer and ZIP artefacts to GitHub releases.

### Changed

- Targeted Windows 2000 (`_WIN32_WINNT=0x500`) for 32-bit MinGW builds; other configurations
  continue to target Windows XP or Vista as before.
- Trimmed the build-msys workflow matrix to only the systems that publish release artefacts
  (`mingw32`, `ucrt64`, and `clangarm64`), removing the now-redundant `release` flag.

## [0.0.5] - 2026-05-04

### Changed

- Adapted GitHub workflows from the bpmdetect project, including build artefact uploads,
  attestations, NSIS packaging, and a winget publish workflow.
- Updated CMake NSIS settings and removed winegcc support.
- Bumped development dependencies, including cspell, prettier, prettier-plugin-sort-json, and
  markdownlint-cli2.
- Bumped GitHub Actions dependencies, including actions/checkout, actions/upload-artifact,
  actions/attest-build-provenance, github/codeql-action, softprops/action-gh-release, and
  egor-tensin/setup-mingw.

## [0.0.4]

### Changed

- Quality of life updates.

## [0.0.2]

### Changed

- Quality of life updates.

## [0.0.1]

First release.

[unreleased]: https://github.com/Tatsh/winpbcopy/compare/v0.0.6...HEAD
[0.0.6]: https://github.com/Tatsh/winpbcopy/compare/v0.0.5...v0.0.6
[0.0.5]: https://github.com/Tatsh/winpbcopy/compare/v0.0.4...v0.0.5
[0.0.4]: https://github.com/Tatsh/winpbcopy/compare/v0.0.3...v0.0.4
[0.0.2]: https://github.com/Tatsh/winpbcopy/compare/v0.0.1...v0.0.2
[0.0.1]: https://github.com/Tatsh/winpbcopy/releases/tag/v0.0.1
