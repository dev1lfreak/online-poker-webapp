cmake_minimum_required(VERSION 3.16)

project(poker_client_qt LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Network Test)

set(CLIENT_SOURCES
    src/main.cpp
    src/ClientController.cpp
    src/net/MessageCodec.cpp
    src/net/NetworkWorker.cpp
    src/ui/ConnectionWindow.cpp
    src/ui/MainMenuWindow.cpp
    src/ui/WaitingRoomWindow.cpp
    src/ui/TableWindow.cpp
)

set(CLIENT_HEADERS
    src/ClientController.hpp
    src/net/MessageCodec.hpp
    src/net/NetworkWorker.hpp
    src/ui/ConnectionWindow.hpp
    src/ui/MainMenuWindow.hpp
    src/ui/WaitingRoomWindow.hpp
    src/ui/TableWindow.hpp
)

add_executable(poker_client
    ${CLIENT_SOURCES}
    ${CLIENT_HEADERS}
)

target_link_libraries(poker_client PRIVATE
    Qt6::Widgets
    Qt6::Network
)

target_include_directories(poker_client PRIVATE src)

enable_testing()

add_executable(client_protocol_tests
    tests/ClientProtocolTest.cpp
    src/net/MessageCodec.cpp
    src/net/MessageCodec.hpp
)

target_link_libraries(client_protocol_tests PRIVATE
    Qt6::Test
    Qt6::Network
)

target_include_directories(client_protocol_tests PRIVATE src)

add_executable(client_network_tests
    tests/NetworkWorkerTest.cpp
    src/net/MessageCodec.cpp
    src/net/NetworkWorker.cpp
    src/net/MessageCodec.hpp
    src/net/NetworkWorker.hpp
)

target_link_libraries(client_network_tests PRIVATE
    Qt6::Test
    Qt6::Network
)

target_include_directories(client_network_tests PRIVATE src)

add_test(NAME client_protocol_tests COMMAND client_protocol_tests)
add_test(NAME client_network_tests COMMAND client_network_tests)
