cmake_minimum_required(VERSION 3.22.1)

project(Epifinance)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY lib)

file(GLOB_RECURSE SRC_MARKET
    include/**.hpp
    include/**.inl
    src/**.cpp
)

add_executable(Market
    ${SRC_MARKET}
)

target_include_directories(Market PRIVATE include)

target_compile_definitions(Market PRIVATE TS_SIZE_OB=1)             # Thread Pool size from Order Book
target_compile_definitions(Market PRIVATE TS_SIZE_ON=1)             # Thread Pool size from Output network
target_compile_definitions(Market PRIVATE NET_RECV_SIZE=4096)       # Package max read size
target_compile_definitions(Market PRIVATE MAX_EVENT_EPOLL=30)       # Maximum of event handle by epoll at once
target_compile_definitions(Market PRIVATE MAX_SOCKET=30)            # Maximum of socket handle by epoll/socket at once
target_compile_definitions(Market PRIVATE N_THREAD_TP=1)            # Default number of thread in a thread pool
target_compile_definitions(Market PRIVATE LOG_DFT_FILE="./default.log") # Default log file

if (MSVC)
    target_compile_options(Market PRIVATE /W4)
    target_compile_definitions(Market PUBLIC NOMINMAX)
else ()
    target_compile_options(Market PRIVATE -Wall -Wextra -g3)
endif ()
