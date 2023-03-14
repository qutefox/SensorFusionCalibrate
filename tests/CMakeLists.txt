cmake_minimum_required(VERSION 3.5)

project(matrices LANGUAGES CXX)

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

add_executable(matrices ${SOURCES})

# add_executable(matrices tst_matrices.cpp)
add_test(NAME matrices COMMAND matrices)

target_link_libraries(matrices PRIVATE Qt${QT_VERSION_MAJOR}::Test)

