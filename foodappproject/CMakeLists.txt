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

# Pass the map HTML folder path to the Qt app at compile time
target_compile_definitions(foodappproject PRIVATE
    MAP_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/map"
)

# Build the tracking server as a separate binary
find_package(Threads REQUIRED)
add_executable(map_server map/server.cpp)
target_compile_features(map_server PRIVATE cxx_std_17)
target_link_libraries(map_server PRIVATE
    Threads::Threads
    ssl
    crypto
    boost_system
)
set_target_properties(map_server PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)
