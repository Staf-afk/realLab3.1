cmake_minimum_required(VERSION 3.16)
set(CMAKE_MAKE_PROGRAM "C:/Qt/Tools/mingw1310_64/bin/mingw32-make.exe" CACHE FILEPATH "Path to mingw32-make")
set(CMAKE_C_COMPILER "C:/Qt/Tools/mingw1310_64/bin/gcc.exe")
set(CMAKE_CXX_COMPILER "C:/Qt/Tools/mingw1310_64/bin/g++.exe")

project(realLab3_1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH "C:/Qt/6.11.1/mingw_64" CACHE PATH "Path to Qt6")

find_package(Qt6 REQUIRED COMPONENTS Widgets)
qt_standard_project_setup()

add_executable(console_app 
    tests/main.cpp 
    src/BinaryTree.tpp 
)
target_include_directories(console_app PRIVATE include)

add_executable(gui_app 
    gui/main.cpp 
    src/BinaryTree.tpp 
)
target_include_directories(gui_app PRIVATE include)
target_link_libraries(gui_app PRIVATE Qt6::Widgets)