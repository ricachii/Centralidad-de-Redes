cmake_minimum_required(VERSION 3.16)
project(CentralidadRedes CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Release build with optimizations by default
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS_RELEASE "-O2")
set(CMAKE_CXX_FLAGS_DEBUG   "-g -Wall -Wextra")

# ── Sources ──────────────────────────────────────────────────────────────────
file(GLOB_RECURSE SOURCES
    "src/graph/*.cpp"
    "src/metrics/*.cpp"
    "src/parsers/*.cpp"
    "src/experiments/*.cpp"
    "src/utils/*.cpp"
)

add_executable(centralidad src/main.cpp ${SOURCES})
target_include_directories(centralidad PRIVATE src)

# ── Tests ────────────────────────────────────────────────────────────────────
enable_testing()

file(GLOB TEST_SOURCES "tests/*.cpp")
foreach(test_src ${TEST_SOURCES})
    get_filename_component(test_name ${test_src} NAME_WE)
    add_executable(${test_name} ${test_src} ${SOURCES})
    target_include_directories(${test_name} PRIVATE src)
    add_test(NAME ${test_name} COMMAND ${test_name})
endforeach()
