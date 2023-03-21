cmake_minimum_required(VERSION 3.5)

project(SensorFusionCalibrate VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Os -Wa,-mbig-obj") # required for Eigen::EigenSolver

set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets SerialPort)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets SerialPort)

# enable FetchContent
include(FetchContent)

# declare Eigen
FetchContent_Declare(Eigen3
    GIT_REPOSITORY https://gitlab.com/libeigen/eigen.git
    GIT_TAG        3.4.0
)

set(EIGEN_BUILD_DOC OFF)
# note: To disable eigen tests,
# you should put this code in a add_subdirectory to avoid to change
# BUILD_TESTING for your own project too since variables are directory
# scoped
set(BUILD_TESTING OFF)
set(EIGEN_BUILD_PKGCONFIG OFF)

# make available
FetchContent_MakeAvailable(Eigen3)

set(PROJECT_SOURCES
        main.cpp

        data/data_types.h
        data/point.h
        data/point.cpp
        data/calibration_result.h
        data/calibration_result.cpp
        data/csv_file_config.h
        data/csv_file_config.cpp
        data/serialport_config.h
        data/serialport_config.cpp
        data/serialport_id_match.h
        data/serialport_id_match.cpp

        terminal/ansi/ansi.h
        terminal/ansi/ansi.cpp

        datasource/line_parser.h
        datasource/line_parser.cpp
        datasource/data_source_interface.h

        datasource/file/csv_file.h
        datasource/file/csv_file.cpp

        datasource/serial/id_matcher.h
        datasource/serial/id_matcher.cpp
        datasource/serial/serialport.h
        datasource/serial/serialport.cpp

        app/application.h
        app/application.cpp
        app/settings.h
        app/settings.cpp
        app/calibrate.h
        app/calibrate.cpp

        widget/csv_file_control.cpp
        widget/csv_file_control.h
        widget/csv_file_control.ui

        widget/serialport_control.cpp
        widget/serialport_control.h
        widget/serialport_control.ui

        widget/main_window.cpp
        widget/main_window.h
        widget/main_window.ui


        resources/resources.qrc
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(SensorFusionCalibrate
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET SensorFusionCalibrate APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(SensorFusionCalibrate SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(SensorFusionCalibrate
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(SensorFusionCalibrate PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(SensorFusionCalibrate PRIVATE Qt${QT_VERSION_MAJOR}::SerialPort)
target_link_libraries(SensorFusionCalibrate PRIVATE Eigen3::Eigen)

set_target_properties(SensorFusionCalibrate PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS SensorFusionCalibrate
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(SensorFusionCalibrate)
endif()
