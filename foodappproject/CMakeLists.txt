cmake_minimum_required(VERSION 3.16)

project(foodappproject VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. Find Qt 6 and the SQL component
find_package(Qt6 REQUIRED COMPONENTS Widgets Sql)

# 2. Add your source files
set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
)

# 3. Create the app (Simplified)
qt_add_executable(foodappproject
    ${PROJECT_SOURCES}
)

# 4. Link the libraries (The most important part)
target_link_libraries(foodappproject PRIVATE
    Qt6::Widgets
    Qt6::Sql
)

# 5. Finalize the executable
qt_finalize_executable(foodappproject)
