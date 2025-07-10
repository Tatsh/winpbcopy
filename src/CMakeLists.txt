if(WIN32 OR MINGW)
  set(EXECS pbcopy pbpaste)
  foreach(exe ${EXECS})
    add_executable(${exe} ${exe}_main.c ${exe}.c)
    if(CMAKE_COMPILER_IS_GNUCXX OR ${CMAKE_C_COMPILER_ID} STREQUAL "Clang")
      target_compile_options(${exe} PRIVATE ${GCC_CLANG_SHARED_C_FLAGS}
                                            $<$<CONFIG:Debug>:${GCC_CLANG_DEBUG_C_FLAGS}>)
    elseif(MSVC)
      # Disable warnings about secure CRT, but still enable compatible signature versions
      # http://stackoverflow.com/a/119752/374110
      target_compile_definitions(${exe} PRIVATE _CRT_SECURE_NO_WARNINGS nullptr=NULL)
    endif()
  endforeach()

  install(TARGETS pbcopy pbpaste RUNTIME)
  install(FILES ../man/pbcopy.1 ../man/pbpaste.1 TYPE MAN)
  set(CPACK_GENERATOR "NSIS;ZIP")
  include(CPack)
endif()

if(BUILD_TESTS AND CMAKE_COMPILER_IS_GNUCXX)
  enable_testing()
  list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR})
  find_package(cmocka REQUIRED)
  set(mocked_functions
      "CloseClipboard;\
EmptyClipboard;\
GetClipboardData;\
GetLastError;\
GlobalAlloc;\
GlobalFree;\
GlobalLock;\
GlobalUnlock;\
OpenClipboard;\
SetClipboardData;\
Sleep;\
isatty;\
read")
  foreach(func ${mocked_functions})
    list(APPEND test_flags "-Wl,--wrap,${func}")
  endforeach()
  add_executable(pbcopy_tests pbcopy.c pbpaste.c test.c)
  target_compile_definitions(
    pbcopy_tests
    PRIVATE
      "CF_TEXT=1;\
GMEM_MOVEABLE=0x0002;\
GMEM_ZEROINIT=0x0040;\
FALSE=0;\
TRUE=1")
  target_compile_options(
    pbcopy_tests PRIVATE ${GCC_CLANG_SHARED_C_FLAGS} $<$<CONFIG:Debug>:${GCC_CLANG_DEBUG_C_FLAGS}>
                         --coverage)
  target_link_options(pbcopy_tests PRIVATE ${test_flags})
  target_link_libraries(pbcopy_tests cmocka gcov)
  add_test(NAME general_tests COMMAND pbcopy_tests)
endif()
