cmake_minimum_required(VERSION 3.16)

project(poker_server)
cmake_policy(SET CMP0167 OLD)
cmake_policy(SET CMP0135 NEW)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost REQUIRED COMPONENTS system thread json)
find_package(GTest REQUIRED)

file(GLOB_RECURSE LOGIC_SOURCES
        "./src/*.cpp"
)
list(REMOVE_ITEM LOGIC_SOURCES "${CMAKE_CURRENT_SOURCE_DIR} ./src/main.cpp")

add_library(poker_logic STATIC ${LOGIC_SOURCES})

target_include_directories(poker_logic PUBLIC ./include)

target_link_libraries(poker_logic PUBLIC
        Boost::system
        Boost::thread
        Boost::json
        ws2_32
        mswsock
)

add_executable(poker_server ./src/main.cpp)
target_link_libraries(poker_server PRIVATE poker_logic)

enable_testing()

file(GLOB_RECURSE TEST_SOURCES "./tests/*.cpp")

add_executable(unit_tests ${TEST_SOURCES})

target_link_libraries(unit_tests PRIVATE
        poker_logic
        GTest::gtest
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(unit_tests)