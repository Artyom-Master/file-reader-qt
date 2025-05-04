cmake_minimum_required(VERSION 3.16)

project(tst_controller LANGUAGES CXX)

enable_testing()

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Test)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Test StateMachine)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(../../src/ ../../src/abstractworker)

add_executable(tst_controller tst_controller.cpp
    ../../src/controller.cpp
    ../../src/filereaderworker.cpp
    ../../src/wordscountermodel.cpp
    ../../src/wordscounterworker.cpp
    ../../src/abstractworker/abstractworker.cpp)

add_test(NAME tst_controller COMMAND tst_controller)

target_link_libraries(tst_controller PRIVATE Qt${QT_VERSION_MAJOR}::Test Qt${QT_VERSION_MAJOR}::StateMachine)

