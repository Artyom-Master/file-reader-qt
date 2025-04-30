cmake_minimum_required(VERSION 3.16)

project(tst_wordscountermodel LANGUAGES CXX)

enable_testing()

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Test)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Test)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(../../src/)

add_executable(tst_wordscountermodel tst_wordscountermodel.cpp
    ../../src/wordscountermodel.cpp)
add_test(NAME tst_wordscountermodel COMMAND tst_wordscountermodel)

target_link_libraries(tst_wordscountermodel PRIVATE Qt${QT_VERSION_MAJOR}::Test)

