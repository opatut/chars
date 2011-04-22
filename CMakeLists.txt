set(PROJECT_NAME chars)
cmake_minimum_required(VERSION 2.8.0)

project(${PROJECT_NAME})

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake-extras/")

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)

find_package(YamlCpp REQUIRED)
find_package(OIS REQUIRED)
find_package(SFML REQUIRED)
find_package(OGRE REQUIRED)
find_package(Bullet REQUIRED)
find_package(MyGUI REQUIRED)
find_package(MyGUI_PLATFORM REQUIRED)
find_package(Boost COMPONENTS filesystem system python date_time REQUIRED)
find_package(MySQLpp REQUIRED)
find_package(OpenSSL REQUIRED)

#add_definitions(-Wall -Wextra -g -pedantic -fPIC -std=c++0x)
add_definitions(-g -pedantic -fPIC -std=c++0x)
set(CMAKE_BUILD_TYPE "Debug")

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

include_directories(
    "${CMAKE_SOURCE_DIR}/include/"
    ${YAMLCPP_INCLUDE_DIR}
    ${Boost_INCLUDE_DIRS}
    ${SFML_INCLUDE_DIRS}
    ${OGRE_INCLUDE_DIRS}
    ${MYGUI_INCLUDE_DIR}
    ${MYGUI_PLATFORM_INCLUDE_DIR}
    ${BULLET_INCLUDE_DIRS}
    ${OIS_INCLUDE_DIRS}
    ${MySQLpp_INCLUDE_DIR}
    "/usr/include/mysql/"
    "/usr/include/PagedGeometry"
    ${OPENSSL_INCLUDE_DIR}
    )

aux_source_directory(src/client client_sources)
aux_source_directory(src/client/render client_sources)
aux_source_directory(src/client/gamestate client_sources)

aux_source_directory(src/common common_sources)
aux_source_directory(src/common/config common_sources)
aux_source_directory(src/common/entities common_sources)
aux_source_directory(src/common/events common_sources)
aux_source_directory(src/common/util common_sources)
aux_source_directory(src/common/gamestate common_sources)
aux_source_directory(src/common/network common_sources)
aux_source_directory(src/common/log common_sources)
aux_source_directory(src/common/objects common_sources)

aux_source_directory(src/server server_sources)
aux_source_directory(src/server/database server_sources)
aux_source_directory(src/server/login server_sources)

include_directories(src/)

add_executable(${PROJECT_NAME}
    ${client_sources}
    ${common_sources}
    )

target_link_libraries(${PROJECT_NAME}
    ${YAMLCPP_LIBRARY}
    ${Boost_LIBRARIES}
    ${SFML_LIBRARY}
    ${OGRE_LIBRARIES}
    ${MYGUI_LIBRARIES}
    ${MYGUI_PLATFORM_LIBRARIES}
    #${BULLET_LIBRARIES}
    ${OIS_LIBRARIES}
    PagedGeometry
    ${OPENSSL_LIBRARY}
    )

add_executable(server
    ${server_sources}
    ${common_sources}
    )


target_link_libraries(server
    ${YAMLCPP_LIBRARY}
    ${Boost_LIBRARIES}
    ${SFML_LIBRARY}
    ${OGRE_LIBRARIES}
    ${MYGUI_LIBRARIES}
    ${MYGUI_PLATFORM_LIBRARIES}
    #${BULLET_LIBRARIES}
    #${OIS_LIBRARIES}
    PagedGeometry
    ${MySQLpp_LIBRARIES}
    ${OPENSSL_LIBRARY}
    )
