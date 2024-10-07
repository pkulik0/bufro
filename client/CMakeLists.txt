cmake_minimum_required(VERSION 3.20)

project(bufro)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
find_package(Qt6 COMPONENTS Core Widgets Network NetworkAuth REQUIRED)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(DEBUG)
endif()

find_package(Protobuf REQUIRED)
find_package(absl REQUIRED)
add_subdirectory(QHotkey)

add_custom_target(
        update_version
        COMMAND ${CMAKE_COMMAND}
        -D IN_DIR=${CMAKE_SOURCE_DIR}/src
        -D OUT_DIR=${CMAKE_BINARY_DIR}/generated
        -P ${CMAKE_SOURCE_DIR}/update_version.cmake
)

file(GLOB_RECURSE SOURCES src/*.cxx src/*.hxx)
file(GLOB_RECURSE PROTO_SOURCES src/*.pb.cc src/*.pb.h)

qt_add_executable(bufro ${SOURCES} ${PROTO_SOURCES} resources.qrc)
add_dependencies(bufro update_version)

target_include_directories(bufro PRIVATE ${CMAKE_BINARY_DIR}/generated)
target_link_libraries(bufro PRIVATE Qt6::Core Qt6::Widgets Qt6::Network Qt6::NetworkAuth qhotkey ${Protobuf_LIBRARIES} absl::base absl::strings absl::log absl::check)