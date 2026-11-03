{
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
      run: 'sudo apt-get update\nsudo apt-get install -y gcc-mingw-w64-x86-64\n',
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
}
