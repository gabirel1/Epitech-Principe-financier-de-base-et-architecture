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

add_compile_definitions(MAX_EVENT_EPOLL=30)            # Maximum of event handle by epoll at once
add_compile_definitions(MAX_SOCKET=30)                 # Maximum of socket handle by epoll/socket at once
add_compile_definitions(N_THREAD_TP=1)                 # Default number of thread in a thread pool
add_compile_definitions(LOG_DFT_FILE="./default.log")  # Default log file
add_compile_definitions(MAX_RECV_SIZE=2048)            # Maximum of data received bu a socket
add_compile_definitions(MARKET_NAME="GOLD","EURO","USD")    # Server name (used for TargetCompId from client)

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

target_compile_definitions(MarketServer PRIVATE TS_SIZE_OB=1)                       # Thread Pool size from Order Book
target_compile_definitions(MarketServer PRIVATE TS_SIZE_ON=1)                       # Thread Pool size from Output network
target_compile_definitions(MarketServer PRIVATE NET_RECV_SIZE=4096)                 # Package max read size
target_compile_definitions(MarketServer PRIVATE PROVIDER_NAME="MyMarket")           # Server name (used for TargetCompId from client)
target_compile_definitions(MarketServer PRIVATE MARKET_NAME="GOLD","EURO","USD")    # Server name (used for TargetCompId from client)

# Client

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)


find_package(Qt5 REQUIRED COMPONENTS Core)

file(GLOB_RECURSE SRC_CLIENT
    include/Client/GUI/**.ui
    include/Client/**.h
    include/Client/**.hpp
    include/Client/**.inl
    src/Client/**.cpp
)

add_executable(MarketClient
    ${SRC_CLIENT}
)

target_link_libraries(MarketClient PRIVATE MarketCommon Qt${QT_VERSION_MAJOR}::Widgets)

target_include_directories(MarketClient PRIVATE include)

# Setup GTest
# include(FetchContent)

# set(GTEST_VERSION 1.14.0)
# FetchContent_Declare(
#   googletest
#   URL https://github.com/google/googletest/archive/refs/tags/v${GTEST_VERSION}.tar.gz
#   PREFIX ${CMAKE_BINARY_DIR}/gtest
#   CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/gtest
# )
# FetchContent_MakeAvailable(googletest)

# enable_testing()

# file(GLOB_RECURSE SRC_TESTS
#     test/**.cpp
# )

# add_executable(MarketTest
#     ${SRC_TESTS}
# )
# target_include_directories(MarketTest PRIVATE include)
# target_include_directories(MarketTest PRIVATE test)
# target_link_libraries(MarketTest MarketCommon GTest::gtest_main)
# target_compile_options(MarketTest PRIVATE -Wno-protected -Wno-private)

# include(GoogleTest)
# gtest_discover_tests(MarketTest)