cmake_minimum_required (VERSION 3.21)
project(MyApp)

find_package (Qt6 REQUIRED COMPONENTS Widgets Sql)

add_executable (myapp mainmainwindow.cpp)
target_link_libraries (myapp PRIVATE Qt6::Widgets Qt6::Sql)
