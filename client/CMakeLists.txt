cmake_minimum_required(VERSION 3.20)

project(bufro)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_AUTOMOC ON)
find_package(Qt6 COMPONENTS Core Widgets Network NetworkAuth REQUIRED)

find_package(Protobuf REQUIRED)
find_package(absl REQUIRED)
add_subdirectory(QHotkey)

add_executable(bufro main.cxx app.cxx app.hxx capture.cxx capture.hxx pb/bufro.pb.cc pb/bufro.pb.h
        network.hxx
        constants.hxx
        settings.hxx
        auth.cxx
        auth.hxx
)
target_link_libraries(bufro PRIVATE Qt6::Core Qt6::Widgets Qt6::Network Qt6::NetworkAuth qhotkey ${Protobuf_LIBRARIES} absl::base absl::strings absl::log absl::check)