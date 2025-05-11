cmake_minimum_required(VERSION 3.16)
project(WynPlan VERSION 0.1 LANGUAGES CXX)

# Configuración básica Qt
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/ui)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(DESIGNAR_DIR "${CMAKE_CURRENT_SOURCE_DIR}/resources/DeSiGNAR-2.0.0")
set(DESIGNAR_BUILD_DIR "${DESIGNAR_DIR}/build")
set(DESIGNAR_LIB_NAME "Designar")

find_library(DESIGNAR_LIB
    NAMES ${DESIGNAR_LIB_NAME}
    PATHS "${DESIGNAR_BUILD_DIR}"
    NO_DEFAULT_PATH
)

if(DESIGNAR_LIB)
    message(STATUS "Found DeSiGNAR library: ${DESIGNAR_LIB}")
    set(DESIGNAR_INCLUDE_DIR "${DESIGNAR_DIR}/include")
else()
    message(STATUS "DeSiGNAR library not found, compiling it...")

    file(MAKE_DIRECTORY ${DESIGNAR_BUILD_DIR})

    execute_process(
        COMMAND ${CMAKE_COMMAND} -S ${DESIGNAR_DIR} -B ${DESIGNAR_BUILD_DIR}
        WORKING_DIRECTORY ${DESIGNAR_DIR}
        RESULT_VARIABLE result
    )

    if(NOT result EQUAL 0)
        message(FATAL_ERROR "Failed to configure DeSiGNAR")
    endif()

    execute_process(
        COMMAND ${CMAKE_COMMAND} --build ${DESIGNAR_BUILD_DIR} --config Release
        WORKING_DIRECTORY ${DESIGNAR_DIR}
        RESULT_VARIABLE result
    )

    if(NOT result EQUAL 0)
        message(FATAL_ERROR "Failed to build DeSiGNAR")
    endif()

    find_library(DESIGNAR_LIB
        NAMES ${DESIGNAR_LIB_NAME}
        PATHS "${DESIGNAR_BUILD_DIR}"
        NO_DEFAULT_PATH
        REQUIRED
    )

    set(DESIGNAR_INCLUDE_DIR "${DESIGNAR_DIR}/include")
    message(STATUS "Successfully built DeSiGNAR: ${DESIGNAR_LIB}")
endif()

set(PROJECT_SOURCES
    src/main.cpp
    src/mainwindow.cpp
    include/mainwindow.h
    ui/mainwindow.ui
    resources/flujograma.json
    resources/teachers.json
    src/definitions.cpp
)

include_directories(
    include
    ${DESIGNAR_INCLUDE_DIR}
)

qt_add_executable(WynPlan
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
    include/definitions.h

    resources.qrc
    src/definitions.cpp
    include/schedulesolver.h
    include/networkgraph.h
    src/networkgraph.cpp
    include/menuwindow.h
    src/menuwindow.cpp
    include/homepage.h
    src/homepage.cpp
    ui/homepage.ui
)

target_link_libraries(WynPlan PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    ${DESIGNAR_LIB}
)

if(${QT_VERSION} VERSION_LESS 6.1.0)
    set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.WynPlan)
endif()

set_target_properties(WynPlan PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(WynPlan)
endif()
