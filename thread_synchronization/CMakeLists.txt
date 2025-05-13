cmake_minimum_required(VERSION 3.10)
project(thread_synchronization VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable warnings
if(MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()

# Main executable
add_executable(thread_sync
    src/main.cpp
    src/thread_manager.cpp
    src/marker_thread.cpp
    src/array_manager.cpp
    src/sync_primitives.cpp
)

# Add thread library support
find_package(Threads REQUIRED)
target_link_libraries(thread_sync PRIVATE Threads::Threads)

# Test configuration
option(BUILD_TESTS "Build the tests" ON)
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()

# Generate code coverage report
option(ENABLE_COVERAGE "Enable coverage reporting" OFF)
if(ENABLE_COVERAGE)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(thread_sync PRIVATE --coverage)
        target_link_options(thread_sync PRIVATE --coverage)
    endif()
endif()