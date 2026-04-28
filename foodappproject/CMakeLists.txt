cmake_minimum_required(VERSION 3.16)
project(foodappproject VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Sql Network WebEngineWidgets WebEngineCore)

set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    database.cpp
    database.h
    delivery.cpp
    delivery.h
    people.cpp
    people.h
    org.cpp
    org.h
)

qt_add_executable(foodappproject
    ${PROJECT_SOURCES}
)

target_link_libraries(foodappproject PRIVATE
    Qt6::Widgets
    Qt6::Sql
    Qt6::Network
    Qt6::WebEngineWidgets
Qt6::WebEngineCore
)

qt_finalize_executable(foodappproject)
