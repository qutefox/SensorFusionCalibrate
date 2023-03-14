cmake_minimum_required(VERSION 3.5)

project(SensorFusionCalibrateTests LANGUAGES CXX)

enable_testing()

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Test)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Test)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB SOURCES
    ../src/math/*.cpp
    ./*.cpp
)

add_executable(SensorFusionCalibrateTests ${SOURCES})

# enable FetchContent
include(FetchContent)

# declare Eigen
FetchContent_Declare(Eigen
    GIT_REPOSITORY https://gitlab.com/libeigen/eigen.git
    GIT_TAG        3.4.0
)

# make available
FetchContent_MakeAvailable(Eigen)

# add_executable(SensorFusionCalibrateTests tst_matrices.cpp)
add_test(NAME SensorFusionCalibrateTests COMMAND SensorFusionCalibrateTests)

target_link_libraries(SensorFusionCalibrateTests PRIVATE Qt${QT_VERSION_MAJOR}::Test)

