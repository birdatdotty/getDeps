cmake_minimum_required(VERSION 3.5)

project(getDeps LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#find_package(Qt5Core qml)
find_package(Qt5 COMPONENTS Core Qml REQUIRED)

add_subdirectory(Console)
add_subdirectory(Deps)

include_directories(Console Deps)

add_executable(getDeps
  main.cpp
)
target_link_libraries(getDeps Qt5::Core Qt5::Qml Console Deps)

install(TARGETS ${PROJECT_NAME} DESTINATION bin)
install(FILES example.js example-fedora.js  DESTINATION bin
	PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ)
