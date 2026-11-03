local utils = import 'utils.libjsonnet';

{
  uses_user_defaults: true,
  project_type: 'c',
  project_name: 'winpbcopy',
  version: '0.0.6',
  description: 'pbcopy/paste for Windows.',
  custom_project_badges: [
    {
      anchor: '[![Tests](https://github.com/Tatsh/winpbcopy/actions/workflows/tests.yml/badge.svg)]',
      href: 'https://github.com/Tatsh/winpbcopy/actions/workflows/tests.yml',
    },
    {
      anchor: '[![Coverage Status](https://coveralls.io/repos/github/Tatsh/winpbcopy/badge.svg?branch=master)]',
      href: 'https://coveralls.io/github/Tatsh/winpbcopy?branch=master',
    },
  ],
  keywords: ['clipboard', 'console', 'terminal', 'windows'],
  want_main: false,
  want_codeql: true,
  want_tests: false,
  want_msys2: true,
  // C/C++ only
  vcpkg+: {
    dependencies: [{
      name: 'cmocka',
      platform: 'linux|mingw',
    }],
  },
  github+: {
    codeql+: {
      languages: ['actions'],
      runs_on: 'ubuntu-latest',
      workflow_file: 'codeql-actions.yml',
    },
    workflows+: {
      publish_msys2+: {
        fork: 'Tatsh/MINGW-packages',
        package_name: 'winpbcopy',
      },
    },
  },
  cz+: {
    commitizen+: {
      version_files+: [
        'man/pbcopy.1',
        'man/pbpaste.1',
      ],
    },
  },
  clang_format_args: 'src/*.c src/*.h',
  vscode+: {
    c_cpp+: {
      configurations: [
        {
          cStandard: 'gnu23',
          compilerPath: '/usr/bin/gcc',
          cppStandard: 'gnu++23',
          includePath: [
            '${workspaceFolder}/src/**',
            '${workspaceFolder}/build/generated',
            '/usr/include/wine/windows',
          ],
          name: 'Linux',
        },
      ],
    },
    settings+: {
      'cmake.configureArgs': [
        '-DBUILD_TESTS=ON',
        '-DCMAKE_BUILD_TYPE=Debug',
        '-DCMAKE_VERBOSE_MAKEFILE=ON',
        '-DENABLE_ASAN=ON',
        '-DENABLE_UBSAN=ON',
      ],
    },
  },
}
+ {
  want_scan_build: true,
  github+: { workflows+: {
    scan_build+: {
      configure_command: 'cmake -S . -B build-scan -DCMAKE_BUILD_TYPE=Debug -DCMAKE_SYSTEM_NAME=Windows -DCMAKE_C_COMPILER_TARGET=x86_64-w64-windows-gnu',
      documentation_steps: [
        {
          name: 'Build project homepage',
          uses: 'actions/jekyll-build-pages@44a6e6beabd48582f863aeeb6cb2151cc1716697',
          with: {
            destination: 'build/html',
          },
        },
      ],
      env: {},
      setup_steps: [
        {
          name: 'Install build dependencies',
          run: |||
            sudo apt-get update
            sudo apt-get install -y gcc-mingw-w64-x86-64
          |||,
        },
        {
          name: 'Install current CMake',
          run: 'pipx install cmake',
        },
        {
          name: 'Locate MinGW runtime libraries',
          run: 'echo "LDFLAGS=-L$(dirname "$(x86_64-w64-mingw32-gcc -print-libgcc-file-name)")" >> "$GITHUB_ENV"',
        },
      ],
    },
  } },
}
