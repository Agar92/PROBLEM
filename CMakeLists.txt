cmake_minimum_required(VERSION 3.15)

project(Test LANGUAGES CXX VERSION 1.2.3)

find_package(Threads REQUIRED)
find_package(Boost REQUIRED COMPONENTS filesystem program_options REQUIRED)

add_executable(server server.cpp)
target_include_directories(server PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(server PRIVATE ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})

set_target_properties(server PROPERTIES
                      CXX_STANDARD 17
			    CXX_STANDARD_REQUIRED ON
)

add_executable(sync_tcp_client sync_tcp_client.cpp)
target_include_directories(sync_tcp_client PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(sync_tcp_client PRIVATE ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})

set_target_properties(sync_tcp_client PROPERTIES
                      CXX_STANDARD 17
			    CXX_STANDARD_REQUIRED ON
)

add_executable(async_tcp_client async_tcp_client.cpp)
target_include_directories(async_tcp_client PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(async_tcp_client PRIVATE ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})

set_target_properties(async_tcp_client PROPERTIES
                      CXX_STANDARD 17
			    CXX_STANDARD_REQUIRED ON
)
