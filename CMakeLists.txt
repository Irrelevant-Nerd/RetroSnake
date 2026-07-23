cmake_minimum_required(VERSION 3.10)

project(RetroSnake LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SFML_DIR "C:/SFML-3.1.0_mingw64/lib/cmake/SFML")

find_package(SFML 3.1 COMPONENTS Graphics Window System Audio Network REQUIRED)

file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS src/*.cpp)

add_executable(RetroSnake ${SRC_FILES})

target_include_directories(RetroSnake PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(RetroSnake PRIVATE
    SFML::Graphics
    SFML::Window
    SFML::System
    SFML::Audio
    SFML::Network
)
