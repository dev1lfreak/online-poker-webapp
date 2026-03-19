cmake_minimum_required(VERSION 3.16)

project(poker_server)
cmake_policy(SET CMP0167 OLD)
cmake_policy(SET CMP0135 NEW)

set(CMAKE_CXX_STANDARD 17)

find_package(Boost REQUIRED COMPONENTS system thread asio json unit_test_framework)

include_directories(server/include)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp")
list(FILTER SOURCES EXCLUDE REGEX "src/main.cpp")

add_executable(poker_server src/main.cpp ${SOURCES})

target_link_libraries(poker_server
        Boost::system
        Boost::thread
        Boost::json
        ws2_32
)