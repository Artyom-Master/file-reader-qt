cmake_minimum_required(VERSION 3.16)

project(file-reader-qt VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)


qt_add_executable(appfile-reader-qt
    src/main.cpp
)

qt_add_qml_module(appfile-reader-qt
    URI file-reader-qt
    VERSION 1.0
    QML_FILES
        qml/Main.qml
        SOURCES src/controller.h src/controller.cpp
        SOURCES src/wordstatsmodel.h src/wordstatsmodel.cpp
        SOURCES src/filereaderworker.h src/filereaderworker.cpp
        QML_FILES
        QML_FILES qml/components/HistogramView.qml
        QML_FILES qml/components/HistogramBar.qml
        QML_FILES qml/views/MainView.qml
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appfile-reader-qt PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appfile-reader-qt
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appfile-reader-qt
    PRIVATE Qt6::Quick
)

include(GNUInstallDirs)
install(TARGETS appfile-reader-qt
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
