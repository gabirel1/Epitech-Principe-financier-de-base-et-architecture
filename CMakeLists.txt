cmake_minimum_required(VERSION 3.22.1)

project(Epifinance)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY lib)

add_compile_options(-Wall -Wextra -g3)

# Common library

file(GLOB_RECURSE SRC_COMMON
    include/Common/**.hpp
    include/Common/**.inl
    src/Common/**.cpp
)

add_library(MarketCommon SHARED
    ${SRC_COMMON}
)

target_include_directories(MarketCommon PRIVATE include)

add_compile_definitions(MarketCommon PRIVATE MAX_EVENT_EPOLL=30)       # Maximum of event handle by epoll at once
add_compile_definitions(MarketCommon PRIVATE MAX_SOCKET=30)            # Maximum of socket handle by epoll/socket at once
add_compile_definitions(MarketCommon PRIVATE N_THREAD_TP=1)            # Default number of thread in a thread pool
add_compile_definitions(MarketCommon PRIVATE LOG_DFT_FILE="./default.log") # Default log file

# Server

file(GLOB_RECURSE SRC_SERVER
    include/Server/**.hpp
    include/Server/**.inl
    src/Server/**.cpp
)

add_executable(MarketServer
    ${SRC_SERVER}
)

target_link_libraries(MarketServer PRIVATE MarketCommon)

target_include_directories(MarketServer PRIVATE include)

target_compile_definitions(MarketServer PRIVATE TS_SIZE_OB=1)             # Thread Pool size from Order Book
target_compile_definitions(MarketServer PRIVATE TS_SIZE_ON=1)             # Thread Pool size from Output network
target_compile_definitions(MarketServer PRIVATE NET_RECV_SIZE=4096)       # Package max read size

# Client

file(GLOB_RECURSE SRC_CLIENT
    include/Client/**.hpp
    include/Client/**.inl
    src/Client/**.cpp
)

add_executable(MarketClient
    ${SRC_CLIENT}
)

target_link_libraries(MarketClient PRIVATE MarketCommon)

target_include_directories(MarketClient PRIVATE include)
