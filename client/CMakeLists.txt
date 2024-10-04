cmake_minimum_required(VERSION 3.20)

project(bufro)
set(CMAKE_CXX_STANDARD 20)

find_package(Qt6 COMPONENTS Core Widgets REQUIRED)

add_executable(bufro main.cxx app.cxx app.hxx
        capture.cxx
        capture.hxx)
target_link_libraries(bufro PRIVATE Qt6::Core Qt6::Widgets)
