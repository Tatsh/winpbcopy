set(GCC_CLANG_SHARED_C_FLAGS -fno-builtin)
set(GCC_CLANG_DEBUG_C_FLAGS
    -ggdb
    -O0
    -pedantic
    -Wall
    -Wdeprecated-declarations
    -Wempty-body
    -Werror
    -Werror=float-conversion
    -Werror=implicit-function-declaration
    -Werror=incompatible-pointer-types
    -Werror=int-conversion
    -Werror=return-type
    -Wmissing-braces
    -Wmissing-field-initializers
    -Wno-keyword-macro
    -Wno-missing-prototypes
    -Wno-strict-prototypes
    -Wno-trigraphs
    -Wno-unknown-pragmas
    -Wparentheses
    -Wpointer-sign
    -Wshadow
    -Wsign-compare
    -Wswitch
    -Wuninitialized
    -Wunknown-pragmas
    -Wunreachable-code
    -Wunused-function
    -Wunused-label
    -Wunused-parameter
    -Wunused-value
    -Wunused-variable)

function(add_min_winnt target)
  if(CMAKE_C_COMPILER_ID MATCHES "Clang|GNU" OR CMAKE_C_COMPILER MATCHES "/winegcc$")
    get_property(IS_64BIT GLOBAL PROPERTY FIND_LIBRARY_USE_LIB64_PATHS)
    # mingw can do 2000 without extra help. 64-bit requires at least XP.
    if(IS_64BIT AND NOT TARGET_VERSION MATCHES "^(0x501|0x6)")
      message(
        FATAL_ERROR
          "Invalid version for 64-bit. Must be XP (0x501) or higher. Version: ${TARGET_VERSION}")
    endif()
    target_compile_definitions(${target} PRIVATE _WIN32_WINNT=${TARGET_VERSION})
  else() # MSVC gets Vista+
    target_compile_definitions(${target} PRIVATE _WIN32_WINNT=${TARGET_VERSION})
  endif()
endfunction()

function(add_asan target)
  if(CMAKE_C_COMPILER_ID MATCHES "Clang|GNU" OR CMAKE_C_COMPILER MATCHES "/winegcc$")
    target_compile_options(${target} PRIVATE $<$<BOOL:${ENABLE_ASAN}>:-fsanitize=address>)
    target_link_options(${target} PRIVATE $<$<BOOL:${ENABLE_ASAN}>:-fsanitize=address>)
  endif()
endfunction()

function(add_ubsan target)
  if(CMAKE_C_COMPILER_ID MATCHES "Clang|GNU" OR CMAKE_C_COMPILER MATCHES "/winegcc$")
    target_compile_options(${target} PRIVATE $<$<BOOL:${ENABLE_UBSAN}>:-fsanitize=undefined>)
    target_link_options(${target} PRIVATE $<$<BOOL:${ENABLE_UBSAN}>:-fsanitize=undefined>)
  endif()
endfunction()

function(add_flags target)
  if(CMAKE_C_COMPILER_ID MATCHES "Clang|GNU" OR CMAKE_C_COMPILER MATCHES "/winegcc$")
    target_compile_options(${target} PRIVATE ${GCC_CLANG_SHARED_C_FLAGS}
                                             $<$<CONFIG:Debug>:${GCC_CLANG_DEBUG_C_FLAGS}>)
  else()
    target_compile_options(${target} PRIVATE /Zc:forScope /Zc:inline /Zc:wchar_t /external:W0
                                             /utf-8)
  endif()
endfunction()

if(WIN32
   OR MINGW
   OR CMAKE_C_COMPILER MATCHES "winegcc")
  set(EXECS pbcopy pbpaste)
  foreach(exe ${EXECS})
    add_executable(${exe} ${exe}_main.c ${exe}.c macros.h)
    add_min_winnt(${exe})
    add_flags(${exe})
    add_asan(${exe})
    add_ubsan(${exe})
  endforeach()
  install(TARGETS pbcopy pbpaste RUNTIME)
  install(FILES ../man/pbcopy.1 ../man/pbpaste.1 TYPE MAN)
  # FIXME include license file in zip

  # FIXME install path should not have version number

  # FIXME may need Visual C++ runtime

  # FIXME missing license in installer

  # FIXME Set publisher

  # FIXME Set 'file description'?

  # FIXME Set Product name

  # FIXME Set 'file version'?

  # FIXME Set Product version

  # FIXME Set Copyright

  # FIXME Set language

  # FIXME No UNIX-style packaging. Just have a single root.
  set(CPACK_GENERATOR "NSIS;ZIP")
  include(CPack)
endif()

if(BUILD_TESTS AND CMAKE_COMPILER_IS_GNUCXX)
  enable_testing()
  set(mocked_functions
      CloseClipboard
      EmptyClipboard
      GetClipboardData
      GetLastError
      GlobalAlloc
      GlobalFree
      GlobalLock
      GlobalUnlock
      OpenClipboard
      SetClipboardData
      Sleep
      isatty
      read)
  foreach(func ${mocked_functions})
    list(APPEND test_flags "-Wl,--wrap,${func}")
  endforeach()
  add_executable(pbcopy_tests macros.h pbcopy.c pbpaste.c test.c)
  add_asan(pbcopy_tests)
  add_ubsan(pbcopy_tests)
  add_flags(pbcopy_tests)
  add_min_winnt(pbcopy_tests)
  target_compile_options(pbcopy_tests PRIVATE $<$<BOOL:${ENABLE_COVERAGE}>:--coverage>)
  target_link_libraries(pbcopy_tests cmocka $<$<BOOL:${ENABLE_COVERAGE}>:gcov>)
  target_link_options(pbcopy_tests PRIVATE ${test_flags})
  add_test(NAME general_tests COMMAND pbcopy_tests)
endif()
