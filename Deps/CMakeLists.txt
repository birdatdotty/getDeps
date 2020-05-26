cmake_minimum_required(VERSION 3.5)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


project(Deps LANGUAGES CXX)

find_package(Qt5 COMPONENTS Core REQUIRED)

add_library(Deps STATIC
	Deps.cpp
	Dll.cc
)

target_link_libraries(Deps PRIVATE Qt5::Core)

target_compile_definitions(Deps PRIVATE CMAKE_LIBRARY_TEST_LIBRARY)

