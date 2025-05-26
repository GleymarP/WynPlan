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



set(THIRD_PARTY_DIR ${CMAKE_CURRENT_BINARY_DIR}/third_party)
file(MAKE_DIRECTORY ${THIRD_PARTY_DIR})


set(DESIGNAR_DIR ${THIRD_PARTY_DIR}/DeSiGNAR)
if(NOT EXISTS ${DESIGNAR_DIR})
    execute_process(
        COMMAND git clone "https://github.com/R3mmurd/DeSiGNAR.git" DeSiGNAR
        WORKING_DIRECTORY ${THIRD_PARTY_DIR}
    )
endif()


set(PROJECT_SOURCES
    src/main.cpp
    resources/flujograma.json
    resources/teachers.json
)

include_directories(
    include
   ${DESIGNAR_DIR}/include
)

file(GLOB DESIGNAR_SRC
    "${DESIGNAR_DIR}/src/*.cpp"
)

add_library(designar STATIC ${DESIGNAR_SRC})
target_include_directories(designar PUBLIC "${DESIGNAR_DIR}/include")

qt_add_executable(WynPlan
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
    include/definitions.h
    resources.qrc
    src/definitions.cpp
    include/networkgraph.h
    src/networkgraph.cpp
    include/homepage.h
    src/homepage.cpp
    ui/homepage.ui
    include/studyplanwindow.h
    src/studyplanwindow.cpp
    ui/studyplanwindow.ui
    include/schedulewindow.h
    src/schedulewindow.cpp
    ui/schedulewindow.ui
    include/teacherswindow.h
    src/teacherswindow.cpp
    ui/teacherswindow.ui
    include/sectionwindow.h
    src/sectionwindow.cpp
    ui/sectionwindow.ui
    include/teachereditordialog.h
    src/teachereditordialog.cpp
    ui/teachereditordialog.ui
    include/teacherdialog.h
    src/teacherdialog.cpp
    ui/teacherdialog.ui
    include/sectiondialog.h
    src/sectiondialog.cpp
    ui/sectiondialog.ui
    resources/assign.json
    include/inicialconfig.h
    src/inicialconfig.cpp
    ui/inicialconfig.ui
    include/addsubjectdialog.h
    src/addsubjectdialog.cpp
    ui/addsubjectdialog.ui
)

target_link_libraries(WynPlan PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    designar
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
