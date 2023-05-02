cmake_minimum_required(VERSION 3.5)

project(SensorFusionCalibrateApp VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Os -Wa,-mbig-obj") # required for Eigen::EigenSolver
set(CMAKE_CXX_FLAGS_RELEASE "/MT")
set(CMAKE_CXX_FLAGS_DEBUG "/MTd")
set(CMAKE_INCLUDE_CURRENT_DIR ON)

list(APPEND CMAKE_PREFIX_PATH "C:\\qt-6.5.0-static")
find_package(Qt6 REQUIRED COMPONENTS Core Widgets SerialPort)
qt_standard_project_setup()

# enable FetchContent
include(FetchContent)

# declare Eigen3
FetchContent_Declare(Eigen3
    GIT_REPOSITORY https://gitlab.com/libeigen/eigen.git
    GIT_TAG        3.4.0
)
set(EIGEN_BUILD_DOC OFF)
set(BUILD_TESTING OFF)
set(EIGEN_BUILD_PKGCONFIG OFF)
FetchContent_MakeAvailable(Eigen3)


# declare GSL
FetchContent_Declare(GSL
    GIT_REPOSITORY https://github.com/microsoft/GSL.git
    GIT_TAG        v4.0.0
)
FetchContent_MakeAvailable(GSL)

# declare fmt
FetchContent_Declare(fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git
    GIT_TAG        9.1.0
)
FetchContent_MakeAvailable(fmt)

# declare catch2
FetchContent_Declare(Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v3.3.2
)
FetchContent_MakeAvailable(Catch2)

# declare libunicode
FetchContent_Declare(libunicode
    GIT_REPOSITORY https://github.com/contour-terminal/libunicode.git
    GIT_TAG        v0.3.0
)
FetchContent_MakeAvailable(libunicode)

add_executable(SensorFusionCalibrateApp
	main.cpp

    data/data_types.h
    data/point.h
    data/point.cpp
    data/calibration_result.h
    data/calibration_result.cpp

    virtual_terminal_emulator/parser/parser_events_interface.h
    virtual_terminal_emulator/parser/parser_state.h
    virtual_terminal_emulator/parser/parser_action.h
    virtual_terminal_emulator/parser/parser.h
    virtual_terminal_emulator/parser/parser.cpp
    virtual_terminal_emulator/parser/parser_extension.h

    virtual_terminal_emulator/utils/vt_assert.h
    virtual_terminal_emulator/utils/vt_boxed.h
    virtual_terminal_emulator/utils/vt_times.h
    virtual_terminal_emulator/utils/vt_sort.h
    virtual_terminal_emulator/utils/vt_escape.h
    virtual_terminal_emulator/utils/vt_indexed.h

    virtual_terminal_emulator/control_code.h
    virtual_terminal_emulator/vt_type.h
    virtual_terminal_emulator/vt_type.cpp
    virtual_terminal_emulator/functions.h
    virtual_terminal_emulator/functions.cpp
    virtual_terminal_emulator/sequence_parameters.h
    virtual_terminal_emulator/sequence_parameters_builder.h
    virtual_terminal_emulator/sequence.h
    virtual_terminal_emulator/sequence.cpp
    virtual_terminal_emulator/sequence_handler_interface.h
    virtual_terminal_emulator/sequencer.h
    virtual_terminal_emulator/sequencer.cpp

    datasource/line_parser.h
    datasource/line_parser.cpp
    datasource/data_source_interface.h

    datasource/file/csv_file_config.h
    datasource/file/csv_file_config.cpp
    datasource/file/csv_file.h
    datasource/file/csv_file.cpp

    datasource/serial/serialport_config.h
    datasource/serial/serialport_config.cpp
    datasource/serial/serialport_id_match.h
    datasource/serial/serialport_id_match.cpp
    datasource/serial/serialport_id_matcher.h
    datasource/serial/serialport_id_matcher.cpp
    datasource/serial/serialport.h
    datasource/serial/serialport.cpp

    app/application.h
    app/application.cpp
    app/settings.h
    app/settings.cpp
    app/calibrate.h
    app/calibrate.cpp

    widget/calibration_result.h
    widget/calibration_result.cpp
    widget/calibration_result.ui

    widget/csv_file_control.h
    widget/csv_file_control.cpp
    widget/csv_file_control.ui

    widget/serialport_control.h
    widget/serialport_control.cpp
    widget/serialport_control.ui

    widget/serialport_terminal.h
    widget/serialport_terminal.cpp

    widget/main_window.h
    widget/main_window.cpp
    widget/main_window.ui

    resources/resources.qrc
)

target_link_libraries(SensorFusionCalibrateApp PRIVATE Qt6::Core)
target_link_libraries(SensorFusionCalibrateApp PRIVATE Qt6::Widgets)
target_link_libraries(SensorFusionCalibrateApp PRIVATE Qt6::SerialPort)
target_link_libraries(SensorFusionCalibrateApp PRIVATE Eigen3::Eigen)
target_link_libraries(SensorFusionCalibrateApp PRIVATE Microsoft.GSL::GSL)
target_link_libraries(SensorFusionCalibrateApp PRIVATE unicode::core)
target_link_libraries(SensorFusionCalibrateApp PRIVATE fmt::fmt)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(SensorFusionCalibrateApp)
endif()
