cmake_minimum_required(VERSION 3.16)
project(Chaos VERSION 1.0)

set(COPYRIGHT "Copyright (c) 2021 Tristan Penman")
set(IDENTIFIER "com.tristanpenman.chaos")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCE_FILES
    src/main.cpp
    src/GameFactory.cpp   
    src/Rom.cpp
    src/SonicReader.cpp
    src/games/Sonic2.cpp
    src/games/Sonic3.cpp
    src/ui/LevelSelect.cpp
    src/ui/Window.cpp
)

find_package(Qt6 COMPONENTS Core REQUIRED)
find_package(Qt6 COMPONENTS Widgets REQUIRED)

if (WIN32)
    qt_add_executable(${PROJECT_NAME} ${SOURCE_FILES} "${CMAKE_CURRENT_SOURCE_DIR}/res/Chaos.rc")
elseif (APPLE)
    set(MACOSX_BUNDLE_ICON_FILE "Chaos.icns")

    set(APP_ICON_MACOS "${CMAKE_CURRENT_SOURCE_DIR}/res/Chaos.icns")
    set_source_files_properties(${APP_ICON_MACOS} PROPERTIES
        MACOSX_PACKAGE_LOCATION "Resources")

    qt_add_executable(${PROJECT_NAME} MACOSX_BUNDLE ${SOURCE_FILES} ${APP_ICON_MACOS})
else()
    qt_add_executable(${PROJECT_NAME} ${SOURCE_FILES})
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets)

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W3 /WX)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic -Werror)
endif()
