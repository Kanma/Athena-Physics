cmake_minimum_required(VERSION 2.6)
project(ATHENA_PHYSICS)


##########################################################################################
# Settings

if (NOT DEFINED ATHENA_PHYSICS_GRAPHICS_SUPPORT)
    set(ATHENA_PHYSICS_GRAPHICS_SUPPORT OFF CACHE BOOL "Enable Athena-Graphics support")
endif()


##########################################################################################
# Library version

set(ATHENA_PHYSICS_VERSION_MAJOR 1)
set(ATHENA_PHYSICS_VERSION_MINOR 1)
set(ATHENA_PHYSICS_VERSION_PATCH 2)
set(ATHENA_PHYSICS_VERSION_SUFFIX "")


##########################################################################################
# XMake-related settings

# Set the output paths
if (NOT DEFINED XMAKE_BINARY_DIR)
    set(XMAKE_BINARY_DIR "${ATHENA_PHYSICS_BINARY_DIR}")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${XMAKE_BINARY_DIR}/bin")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${XMAKE_BINARY_DIR}/lib")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${XMAKE_BINARY_DIR}/bin")
endif()

# Set the dependencies path
if (NOT DEFINED XMAKE_DEPENDENCIES_DIR)
    set(XMAKE_DEPENDENCIES_DIR "${ATHENA_PHYSICS_SOURCE_DIR}/dependencies")
endif()


##########################################################################################
# XMake importation

if (NOT EXISTS "${XMAKE_DEPENDENCIES_DIR}/XMake/XMake.cmake")
    message(FATAL_ERROR
"Missing dependency: XMake
Athena-Physics requires the following dependencies:
   - XMake
   - Athena-Math
   - Athena-Core
   - Athena-Entities
   - UnitTest++
   - Bullet
They are provided as GIT submodules of this repository.
Did you forgot to execute the following commands?
   git submodule init
   git submodule update")
endif()

include("${XMAKE_DEPENDENCIES_DIR}/XMake/XMake.cmake")


##########################################################################################
# Process subdirectories

add_subdirectory(dependencies)

if (DEFINED ATHENA_SCRIPTING_ENABLED AND ATHENA_SCRIPTING_ENABLED)
    set(ATHENA_PHYSICS_SCRIPTING ON)
else()
    set(ATHENA_PHYSICS_SCRIPTING OFF)
endif()

if (ATHENA_PHYSICS_GRAPHICS_SUPPORT AND NOT EXISTS "${XMAKE_DEPENDENCIES_DIR}/Ogre/CMakeLists.txt")
    set(ATHENA_PHYSICS_GRAPHICS_SUPPORT OFF)
endif()

add_subdirectory(include)
add_subdirectory(src)

if (DEFINED ATHENA_SCRIPTING_ENABLED AND ATHENA_SCRIPTING_ENABLED)
    add_subdirectory(scripting)
endif()
