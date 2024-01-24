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

add_executable(client client.cpp)
target_include_directories(client PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(client PRIVATE ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})

set_target_properties(client PROPERTIES
                      CXX_STANDARD 17
		      CXX_STANDARD_REQUIRED ON
)
