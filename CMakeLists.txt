include(CheckCSourceCompiles)
include(CheckTypeSize)

cmake_minimum_required(VERSION 3.31)
project(
  winpbcopy
  DESCRIPTION "pbcopy/paste for Windows."
  HOMEPAGE_URL "https://github.com/Tatsh/winpbcopy"
  VERSION 0.0.4)
include(GNUInstallDirs)

if((CMAKE_C_COMPILER_ID STREQUAL "MSVC" AND CMAKE_C_COMPILER_VERSION VERSION_GREATER_EQUAL 19.37.0.0
   )
   OR CMAKE_COMPILER_IS_GNUCC
   OR CMAKE_C_COMPILER_ID MATCHES "Clang"
   OR CMAKE_C_COMPILER MATCHES "/winegcc$")
  set(CMAKE_C_STANDARD 23)
endif()

option(BUILD_TESTS "Build tests." OFF)
option(ENABLE_ASAN "Enable address sanitiser." OFF)
option(ENABLE_COVERAGE "Link tests with gcov." OFF)
option(ENABLE_UBSAN "Enable undefined behaviour sanitiser." OFF)
if(MSVC)
  set(TARGET_VERSION
      0x600
      CACHE STRING "Target Windows version in 0xMMNN format.")
else()
  set(TARGET_VERSION 0x501)
endif()

if(BUILD_TESTS)
  find_package(cmocka REQUIRED)
endif()

set(CMAKE_REQUIRED_DEFINITIONS -D_WIN32_WINNT=${TARGET_VERSION} -DUNICODE -D_UNICODE)
check_type_size(_Bool STDBOOL BUILTIN_TYPES_ONLY LANGUAGE C)
check_c_source_compiles(
  "#include <stdlib.h>
int main(void) {
  free(nullptr);
  return 0;
}"
  HAVE_NULLPTR_KEYWORD)
add_compile_definitions(
  _CRT_SECURE_NO_WARNINGS _UNICODE HAVE_NULLPTR_T=$<IF:$<BOOL:${HAVE_NULLPTR_KEYWORD}>,1,0>
  HAVE_STDBOOL=$<IF:$<BOOL:${HAVE_STDBOOL}>,1,0> UNICODE WIN32_LEAN_AND_MEAN)

add_subdirectory(src)
