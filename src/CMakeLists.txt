cmake_minimum_required(VERSION 3.16)
project(random-numbers-player LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Charts Core Gui)

qt_add_executable(random-numbers-player
    mainwindow.cpp mainwindow.h
    numbergenerator.cpp numbergenerator.h
    main.cpp
)

target_link_libraries(random-numbers-player PUBLIC
    Qt::Charts
    Qt::Core
    Qt::Gui
)
