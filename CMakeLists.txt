cmake_minimum_required(VERSION 3.15)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 0.0.${PATCH_VERSION})
project(problem VERSION ${PROJECT_VERSION})
configure_file(version.h.in version.h)

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


install(TARGETS server RUNTIME DESTINATION bin)
install(TARGETS client RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT agar10@yandex.ru)
include(CPack)