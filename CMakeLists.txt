cmake_minimum_required(VERSION 3.31)
project(winpbcopy VERSION 0.0.3)
include(GNUInstallDirs)

option(BUILD_TESTS "Build tests." OFF)

set(CMAKE_C_STANDARD 23)
set(GCC_CLANG_SHARED_C_FLAGS "-fno-builtin")
set(GCC_CLANG_DEBUG_C_FLAGS
    "-ggdb;\
-O0;\
-pedantic;\
-Wall;\
-Wdeprecated-declarations;\
-Wempty-body;\
-Werror;\
-Werror=float-conversion;\
-Werror=implicit-function-declaration;\
-Werror=incompatible-pointer-types;\
-Werror=int-conversion;\
-Werror=return-type;\
-Wmissing-braces;\
-Wmissing-field-initializers;\
-Wno-missing-prototypes;\
-Wno-strict-prototypes;\
-Wno-trigraphs;\
-Wno-unknown-pragmas;\
-Wparentheses;\
-Wpointer-sign;\
-Wshadow;\
-Wsign-compare;\
-Wswitch;\
-Wuninitialized;\
-Wunknown-pragmas;\
-Wunreachable-code;\
-Wunused-function;\
-Wunused-label;\
-Wunused-parameter;\
-Wunused-value;\
-Wunused-variable")

add_subdirectory(src)
