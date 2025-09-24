local utils = import 'utils.libjsonnet';

{
  project_type: 'c',
  project_name: 'winpbcopy',
  version: '0.0.4',
  description: 'pbcopy/paste for Windows.',
  keywords: ['clipboard', 'console', 'terminal', 'windows'],
  want_main: false,
  want_codeql: false,
  want_tests: false,
  copilot+: {
    intro: "winpbcopy simulates macOS' pbcopy/pbpaste commands.",
  },
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
  package_json+: {
    scripts+: {
      'check-formatting': "cmake-format --check CMakeLists.txt src/CMakeLists.txt && clang-format -n src/*.c src/*.h && prettier -c . && markdownlint-cli2 '**/*.md' '#node_modules'",
      format: 'cmake-format -i CMakeLists.txt src/CMakeLists.txt && clang-format -i src/*.c src/*.h && yarn prettier -w .',
    },
  },
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
