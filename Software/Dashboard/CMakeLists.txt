cmake_minimum_required(VERSION 3.16)

project(Dashboard VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Gui Qml Quick QuickControls2 SerialPort)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(appDashboard
    src/main.cpp
    src/usbdashreceiver.cpp
    src/usbdashreceiver.h
    assets.qrc
)

set_source_files_properties(qml/AppTheme.qml PROPERTIES QT_QML_SINGLETON_TYPE TRUE)

qt_add_qml_module(appDashboard
    URI Dashboard
    VERSION 1.0
    QML_FILES
        qml/Main.qml
        qml/HomePage.qml
        qml/DebugPage.qml
        qml/AppTheme.qml
        qml/PageElement.qml
        QML_FILES qml/LogPage.qml
        QML_FILES qml/SpeedGauge.qml
        QML_FILES qml/RpmGauge.qml
)

target_include_directories(appDashboard PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/../shared
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appDashboard PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appDashboard
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appDashboard
    PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Qml
        Qt6::Quick
        Qt6::QuickControls2
        Qt6::SerialPort
)

include(GNUInstallDirs)
install(TARGETS appDashboard
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
