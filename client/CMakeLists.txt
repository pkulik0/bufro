cmake_minimum_required(VERSION 3.20)

project(bufro)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_MACOSX_BUNDLE TRUE)
set(MACOSX_EXECUTABLE_NAME ${PROJECT_NAME})
set(MACOSX_BUNDLE_GUI_IDENTIFIER "com.bufro.client")
set(MACOSX_BUNDLE_BUNDLE_NAME "${PROJECT_NAME}")
set(MACOSX_BUNDLE_ICON_FILE "app_icon.icns")
set(BUNDLE_ICON ${CMAKE_CURRENT_SOURCE_DIR}/assets/app_icon.icns)
set_source_files_properties(${BUNDLE_ICON} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(DEBUG)
endif()

find_package(PkgConfig REQUIRED)
pkg_check_modules(PROTOBUF REQUIRED IMPORTED_TARGET protobuf)
message(STATUS "Protobuf version: ${PROTOBUF_VERSION}")
pkg_check_modules(OPENCV REQUIRED IMPORTED_TARGET opencv4)
message(STATUS "OpenCV version: ${OPENCV_VERSION}")
pkg_check_modules(ABSL REQUIRED IMPORTED_TARGET absl_base absl_strings absl_log absl_check)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
find_package(Qt6 COMPONENTS Core Widgets Network NetworkAuth Multimedia REQUIRED)
message(STATUS "Qt6 found: ${Qt6_VERSION}")

find_package(absl REQUIRED)
add_subdirectory(QHotkey)

file(GLOB_RECURSE SOURCES src/*.cxx src/*.hxx)
file(GLOB_RECURSE PROTO_SOURCES src/*.pb.cc src/*.pb.h)

qt_add_executable(bufro MANUAL_FINALIZATION ${SOURCES} ${PROTO_SOURCES} resources.qrc ${BUNDLE_ICON})
target_include_directories(bufro PRIVATE ${CMAKE_BINARY_DIR}/generated)
target_link_libraries(bufro PRIVATE
        Qt6::Core Qt6::Widgets Qt6::Network Qt6::NetworkAuth Qt6::Multimedia
        PkgConfig::OPENCV
        PkgConfig::PROTOBUF
        PkgConfig::ABSL
        qhotkey
)

set_target_properties(${PROJECT_NAME} PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_INFO_PLIST ${CMAKE_SOURCE_DIR}/Info.plist
)

qt_add_ios_ffmpeg_libraries(bufro)

add_custom_target(
        update_version
        COMMAND ${CMAKE_COMMAND}
        -D IN_DIR=${CMAKE_SOURCE_DIR}/src
        -D OUT_DIR=${CMAKE_BINARY_DIR}/generated
        -P ${CMAKE_SOURCE_DIR}/update_version.cmake
)
add_dependencies(bufro update_version)

qt_finalize_target(bufro)