cmake_minimum_required(VERSION 3.24)

project(Epifinance)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY lib)

file(GLOB_RECURSE SRC_ENGINE
    include/**.hpp
    include/**.inl
    src/**.cpp
)

add_executable(Market
    ${SRC_ENGINE}
)

target_include_directories(Market PRIVATE include)

if (MSVC)
    target_compile_options(Market PRIVATE /W4)
    target_compile_definitions(Market PUBLIC NOMINMAX)
else ()
    target_compile_options(Market PRIVATE -Wall -Wextra -g3)
endif ()