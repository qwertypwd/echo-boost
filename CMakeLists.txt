CMAKE_MINIMUM_REQUIRED(VERSION 3.21)
PROJECT(ClienServerEcn)

set (BOOST_ROOT "C:/boost_1_85_0/")
set (Boost_USE_STATIC_LIBS ON)
SET (CMAKE_CXX_STANDARD 17)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)

set (SERVER_SOURCES "./src/Server.cpp")
set (CLIENT_SOURCES "./src/Client.cpp")

set (TESTS_SOURCES "./src/test.cpp")
set (TEST test_${PROJECT})

find_package(Boost REQUIRED COMPONENTS system unit_test_framework)
FIND_PACKAGE(Threads REQUIRED)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})

if(Boost_FOUND)
    ADD_EXECUTABLE(Server ${SERVER_SOURCES})
    TARGET_LINK_LIBRARIES(Server PRIVATE Threads::Threads ${Boost_LIBRARIES} wsock32 ws2_32)

    add_executable (${TEST} ${TESTS_SOURCES})
    target_link_libraries (${TEST} ${PROJECT} ${Boost_LIBRARIES} wsock32 ws2_32)
    enable_testing ()
    add_test (${TEST} ${TEST})

    ADD_EXECUTABLE(Client ${CLIENT_SOURCES})
    TARGET_LINK_LIBRARIES(Client PRIVATE Threads::Threads ${Boost_LIBRARIES} wsock32 ws2_32)

else()
    message(STATUS "BOOST libs Not Found")
endif()


