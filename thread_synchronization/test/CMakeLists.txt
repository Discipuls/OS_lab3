CMAKE_MINIMUM_REQUIRED(VERSION 3.10)

set(TEST_SOURCES
    unit_tests.cpp
)

add_executable(thread_sync_tests ${TEST_SOURCES})

# Include main source files for testing, excluding main.cpp
target_sources(thread_sync_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/src/thread_manager.cpp
    ${CMAKE_SOURCE_DIR}/src/marker_thread.cpp
    ${CMAKE_SOURCE_DIR}/src/array_manager.cpp
    ${CMAKE_SOURCE_DIR}/src/sync_primitives.cpp
)

# Find and link Google Test
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
target_link_libraries(thread_sync_tests PRIVATE ${GTEST_BOTH_LIBRARIES} Threads::Threads)

# Include main project headers
target_include_directories(thread_sync_tests PRIVATE ${CMAKE_SOURCE_DIR}/src)

# Add test to CTest
add_test(NAME thread_synchronization_tests COMMAND thread_sync_tests)

# Generate code coverage report for tests
if(ENABLE_COVERAGE)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(thread_sync_tests PRIVATE --coverage)
        target_link_options(thread_sync_tests PRIVATE --coverage)
    endif()
endif()