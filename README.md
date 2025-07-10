# winpbcopy

[![GitHub tag (with filter)](https://img.shields.io/github/v/tag/Tatsh/winpbcopy)](https://github.com/Tatsh/winpbcopy/tags)
[![License](https://img.shields.io/github/license/Tatsh/winpbcopy)](https://github.com/Tatsh/winpbcopy/blob/master/LICENSE.txt)
[![GitHub commits since latest release (by SemVer including pre-releases)](https://img.shields.io/github/commits-since/Tatsh/winpbcopy/v0.0.3/master)](https://github.com/Tatsh/winpbcopy/compare/v0.0.3...master)
[![CodeQL](https://github.com/Tatsh/winpbcopy/actions/workflows/codeql.yml/badge.svg)](https://github.com/Tatsh/winpbcopy/actions/workflows/codeql.yml)
[![QA](https://github.com/Tatsh/winpbcopy/actions/workflows/qa.yml/badge.svg)](https://github.com/Tatsh/winpbcopy/actions/workflows/qa.yml)
[![Tests](https://github.com/Tatsh/winpbcopy/actions/workflows/tests.yml/badge.svg)](https://github.com/Tatsh/winpbcopy/actions/workflows/tests.yml)
[![Coverage Status](https://coveralls.io/repos/github/Tatsh/winpbcopy/badge.svg?branch=master)](https://coveralls.io/github/Tatsh/winpbcopy?branch=master)
[![GitHub Pages](https://github.com/Tatsh/winpbcopy/actions/workflows/pages/pages-build-deployment/badge.svg)](https://tatsh.github.io/winpbcopy/)
[![pre-commit](https://img.shields.io/badge/pre--commit-enabled-brightgreen?logo=pre-commit&logoColor=white)](https://github.com/pre-commit/pre-commit)
[![Stargazers](https://img.shields.io/github/stars/Tatsh/winpbcopy?logo=github&style=flat)](https://github.com/Tatsh/winpbcopy/stargazers)

[![@Tatsh](https://img.shields.io/badge/dynamic/json?url=https%3A%2F%2Fpublic.api.bsky.app%2Fxrpc%2Fapp.bsky.actor.getProfile%2F%3Factor%3Ddid%3Aplc%3Auq42idtvuccnmtl57nsucz72%26query%3D%24.followersCount%26style%3Dsocial%26logo%3Dbluesky%26label%3DFollow%2520%40Tatsh&query=%24.followersCount&style=social&logo=bluesky&label=Follow%20%40Tatsh)](https://bsky.app/profile/Tatsh.bsky.social)
[![Mastodon Follow](https://img.shields.io/mastodon/follow/109370961877277568?domain=hostux.social&style=social)](https://hostux.social/@Tatsh)

An equivalent of macOS' `pbcopy` and `pbpaste` commands.

## How to build

### Linux

Only tests can be run on Linux unless you build using `CC=winegcc CXX=wineg++`.

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

Downloads are available in the [releases section](https://github.com/Tatsh/winpbcopy/releases).

Install to a location in `PATH`/`${env:PATH}`. The `install` target is usable in MSYS2 and similar
environments.
