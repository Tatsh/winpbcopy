# winpbcopy

[![QA](https://github.com/Tatsh/winpbcopy/actions/workflows/qa.yml/badge.svg)](https://github.com/Tatsh/winpbcopy/actions/workflows/qa.yml)
[![Build](https://github.com/Tatsh/winpbcopy/actions/workflows/cmake.yml/badge.svg)](https://github.com/Tatsh/winpbcopy/actions/workflows/cmake.yml)
![GitHub tag (with filter)](https://img.shields.io/github/v/tag/Tatsh/winpbcopy)
![GitHub](https://img.shields.io/github/license/Tatsh/winpbcopy)
![GitHub commits since latest release (by SemVer including pre-releases)](https://img.shields.io/github/commits-since/Tatsh/winpbcopy/v0.0.1/master)

An equivalent of macOS' `pbcopy` and `pbpaste` commands.

## How to build

### MSYS2

Requirements:

- `cmake`
- `mingw64/mingw-w64-x86_64-gcc`
- `make` or `ninja`

```shell
mkdir build
cd build
cmake ..
make
```

## Installation

Install to a location in `PATH`/`${env:PATH}`. The `install` target is usable in MSYS2 and similar
environments.
