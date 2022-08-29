cmake_minimum_required(VERSION 3.14)

project(SnapMdiArea LANGUAGES CXX)

# Expose public includes to other subprojects through cache variable.
set(${PROJECT_NAME}_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/src CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(example)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

add_library(SnapMdiArea STATIC
  src/SnapMdiArea_global.h
  src/snapmdiarea.cpp
  src/snapmdiarea.h
  src/windowwidget.cpp
  src/windowwidget.h
  src/windowselector.cpp
  src/windowselector.h
  src/overlaywidget.cpp
  src/overlaywidget.h
)

target_link_libraries(SnapMdiArea PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

target_compile_definitions(SnapMdiArea PRIVATE SNAPMDIAREA_LIBRARY)
