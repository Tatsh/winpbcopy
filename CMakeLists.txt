cmake_minimum_required(VERSION 3.26 FATAL_ERROR)
project(winpbcopy 0.0.1)
include(GNUInstallDirs)

set(GCC_CLANG_SHARED_C_FLAGS "-fno-builtin;-std=gnu2x")
set(GCC_CLANG_DEBUG_C_FLAGS "-ggdb;\
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

add_executable(pbcopy pbcopy.c)
add_executable(pbpaste pbpaste.c)
if(CMAKE_COMPILER_IS_GNUCC OR ${CMAKE_C_COMPILER_ID} STREQUAL "Clang")
  target_compile_options(pbcopy
  PRIVATE ${GCC_CLANG_SHARED_C_FLAGS}
          $<$<CONFIG:Debug>:${GCC_CLANG_DEBUG_C_FLAGS}>)
  target_compile_options(pbpaste
  PRIVATE ${GCC_CLANG_SHARED_C_FLAGS}
          $<$<CONFIG:Debug>:${GCC_CLANG_DEBUG_C_FLAGS}>)
elseif(MSVC)
  # Disable warnings about secure CRT, but still enable compatible signature
  # versions http://stackoverflow.com/a/119752/374110
  target_compile_definitions(pbcopy PRIVATE _CRT_SECURE_NO_WARNINGS nullptr=NULL)
  target_compile_definitions(pbpaste PRIVATE _CRT_SECURE_NO_WARNINGS nullptr=NULL)
endif()
install(TARGETS pbcopy pbpaste RUNTIME)
install(FILES man/pbcopy.1 man/pbpaste.1 TYPE MAN)
set(CPACK_GENERATOR "NSIS;ZIP")
include(CPack)
