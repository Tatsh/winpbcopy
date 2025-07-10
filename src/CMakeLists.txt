include_directories(. ${CMAKE_CURRENT_BINARY_DIR}/generated ${CMAKE_CURRENT_BINARY_DIR})

if(CMAKE_BUILD_TYPE STREQUAL Debug AND (CMAKE_CXX_COMPILER_ID STREQUAL GNU OR CMAKE_CXX_COMPILER_ID
                                                                              STREQUAL Clang))
  set(DEBUG_FLAGS -ggdb -Wall -Werror -Wextra -Wno-unknown-pragmas)
endif()

set(LIBNAME_SRCS other.h)
add_library(name STATIC ${LIBNAME_SRCS} ${CMAKE_CURRENT_BINARY_DIR}/generated/namedebug.cpp)
target_compile_options(name PRIVATE ${DEBUG_FLAGS})
target_link_libraries(name PRIVATE ${SHARED_DEPS})

add_executable(${CMAKE_PROJECT_NAME} main.cpp)
target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE ${DEBUG_FLAGS})
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE name ${SHARED_DEPS})

if(BUILD_TESTS)
  enable_testing()
  include(FetchContent)
  FetchContent_Declare(googletest
                       URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip)
  FetchContent_MakeAvailable(googletest)
  add_executable(${CMAKE_PROJECT_NAME}-tests test.cpp)
  target_compile_options(${CMAKE_PROJECT_NAME}-tests PRIVATE ${DEBUG_FLAGS})
  if(CMAKE_COMPILER_IS_GNUCXX)
    target_compile_options(${CMAKE_PROJECT_NAME}-tests PRIVATE --coverage)
  endif()
  target_link_libraries(${CMAKE_PROJECT_NAME}-tests PRIVATE name ${SHARED_DEPS} GTest::gtest_main
                                                            GTest::gmock gcov)
  include(GoogleTest)
  gtest_discover_tests(${CMAKE_PROJECT_NAME}-tests)
endif()
