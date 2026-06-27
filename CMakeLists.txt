cmake_minimum_required(VERSION 3.16)
project(CentralidadRedes CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ── LSP Support ──────────────────────────────────────────────────────────────
# This automatically forces generation of compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Release build with optimizations by default
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS_RELEASE "-O2")
set(CMAKE_CXX_FLAGS_DEBUG   "-g -Wall -Wextra")

# ── Sources ──────────────────────────────────────────────────────────────────
# Note: Added 'utils' directory recursive search just in case .cpp files end up there
file(GLOB_RECURSE SOURCES
    "src/graph/*.cpp"
    "src/metrics/*.cpp"
    "src/parsers/*.cpp"
    "src/experiments/*.cpp"
    "src/utils/*.cpp"
    "utils/*.cpp" 
)

# If you don't have a src/main.cpp yet, change add_executable to add_library
# add_library(centralidad ${SOURCES})
add_executable(centralidad src/main.cpp ${SOURCES})

# ── Fix Include Paths for LSP ────────────────────────────────────────────────
# We explicitly tell CMake (and clangd) all the root locations for headers
target_include_directories(centralidad PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/graph
    ${CMAKE_CURRENT_SOURCE_DIR}/src/metrics
    ${CMAKE_CURRENT_SOURCE_DIR}/utils
)

# ── Tests ────────────────────────────────────────────────────────────────────
enable_testing()

file(GLOB TEST_SOURCES "tests/*.cpp")
foreach(test_src ${TEST_SOURCES})
    get_filename_component(test_name ${test_src} NAME_WE)
    add_executable(${test_name} ${test_src} ${SOURCES})
    
    # Mirror the exact same include directories to your test binaries
    target_include_directories(${test_name} PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/src/graph
        ${CMAKE_CURRENT_SOURCE_DIR}/src/metrics
        ${CMAKE_CURRENT_SOURCE_DIR}/utils
    )
    add_test(NAME ${test_name} COMMAND ${test_name})
endforeach()
