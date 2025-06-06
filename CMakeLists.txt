cmake_minimum_required(VERSION 3.16)

project(MushcFlow VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS "${CMAKE_SOURCE_DIR}/uis")

file(GLOB_RECURSE HEADERS "headers/*.hpp")
file(GLOB_RECURSE SOURCES "scrs/*.cpp")
file(GLOB_RECURSE UIS "uis/*.ui")
file(GLOB_RECURSE RESOURCES "resources/*.qrc")

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS Multimedia)
find_package(Qt6 REQUIRED COMPONENTS Concurrent)

set(PROJECT_SOURCES
        ${HEADERS}
        ${SOURCES}
        ${UIS}
        ${RESOURCES}
        icon.rc
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(MushcFlow
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}

        #logs.hpp logs.cpp
        #first_open_config.hpp first_open_config.cpp first_open_config.ui
        #downloader.hpp downloader.cpp downloader.ui song_handler.cpp
        #song_handler.hpp
        #playlist.hpp playlist.cpp playlist.ui
        #config_file_handler.hpp config_file_handler.cpp
        #Images.qrc
    )

else()
    if(ANDROID)
        add_library(MushcFlow SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(MushcFlow
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(MushcFlow PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(MushcFlow PRIVATE Qt6::Multimedia)
target_link_libraries(MushcFlow PRIVATE Qt6::Concurrent)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.MushcFlow)
endif()
set_target_properties(MushcFlow PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS MushcFlow
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(MushcFlow)
endif()
