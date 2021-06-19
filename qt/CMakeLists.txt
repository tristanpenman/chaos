cmake_minimum_required(VERSION 3.0)

# Add folder where are supportive functions
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Include Qt basic functions
include(QtCommon)

# Basic information about project

project(Chaos VERSION 1.0)

# Set PROJECT_VERSION_PATCH and PROJECT_VERSION_TWEAK to 0 if not present, needed by add_project_meta
fix_project_version()

# Set additional project information
set(COPYRIGHT "Copyright (c) 2021 Tristan Penman")
set(IDENTIFIER "com.tristanpenman.chaos")

set(SOURCE_FILES
    src/main.cpp
    src/GameFactory.cpp
    src/LoadLevelDialog.cpp
    src/Rom.cpp
    src/SonicReader.cpp
    src/Window.cpp
    src/games/Sonic2.cpp
    src/games/Sonic3.cpp
)

include_directories(SYSTEM)

add_project_meta(META_FILES_TO_INCLUDE)

find_package(Qt6 COMPONENTS Widgets REQUIRED)

add_executable(${PROJECT_NAME} ${OS_BUNDLE} # Expands to WIN32 or MACOS_BUNDLE depending on OS
    ${SOURCE_FILES} ${META_FILES_TO_INCLUDE} ${RESOURCE_FILES}
)

target_link_libraries(${PROJECT_NAME} Qt6::Widgets)
