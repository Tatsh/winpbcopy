local utils = import 'utils.libjsonnet';

{
  uses_user_defaults: true,
  project_type: 'c',
  project_name: 'winpbcopy',
  version: '0.0.6',
  description: 'pbcopy/paste for Windows.',
  social+: {
    custom_badges: [
      '[![Tests](https://github.com/Tatsh/winpbcopy/actions/workflows/tests.yml/badge.svg)](https://github.com/Tatsh/winpbcopy/actions/workflows/tests.yml)',
      '[![Coverage Status](https://coveralls.io/repos/github/Tatsh/winpbcopy/badge.svg?branch=master)](https://coveralls.io/github/Tatsh/winpbcopy?branch=master)',
    ],
  },
  keywords: ['clipboard', 'console', 'terminal', 'windows'],
  want_main: false,
  want_codeql: false,
  want_tests: false,
  // C/C++ only
  vcpkg+: {
    dependencies: [{
      name: 'cmocka',
      platform: 'linux|mingw',
    }],
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
