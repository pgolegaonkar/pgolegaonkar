cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu11 -pthread")
set(HELPER_FILES ClientServer.c ClientServer.h)
set(Server
        ${HELPER_FILES}
        Server.c)
set(Client
        ${HELPER_FILES}
        Client.c)
add_executable(Client ${Client})
add_executable(Server ${Server})
target_link_libraries(Server pthread)
target_link_libraries(Client pthread)