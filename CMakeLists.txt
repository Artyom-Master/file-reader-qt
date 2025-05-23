cmake_minimum_required(VERSION 3.16)

project(file-reader-qt VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)

qt_add_executable(file-reader-qt
    src/main.cpp
)

qt_add_qml_module(file-reader-qt
    URI file-reader-qt
    VERSION 1.0
    QML_FILES
        qml/Main.qml
        qml/components/HistogramView.qml
        qml/components/HistogramBar.qml
        qml/views/MainView.qml
    SOURCES
        src/controller.h src/controller.cpp
        src/wordscountermodel.h src/wordscountermodel.cpp
        src/filereaderworker.h src/filereaderworker.cpp
        src/wordscounterworker.h src/wordscounterworker.cpp
        src/abstractworker/abstractworker.h src/abstractworker/abstractworker.cpp
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(file-reader-qt PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appfile-reader-qt
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(file-reader-qt PRIVATE Qt6::Quick)

include(GNUInstallDirs)
install(TARGETS file-reader-qt
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
add_subdirectory(tests/tst_filereaderworker)
add_subdirectory(tests/tst_controller)
add_subdirectory(tests/tst_wordscountermodel)
add_subdirectory(tests/tst_wordscounterworker)
