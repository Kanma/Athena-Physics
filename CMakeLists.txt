cmake_minimum_required(VERSION 2.6)
project(ATHENA_PHYSICS)

# Settings
if (NOT DEFINED ATHENA_PHYSICS_SCRIPTING)
    set(ATHENA_PHYSICS_SCRIPTING OFF CACHE BOOL "Enable scripting support (needs Athena-Scripting)")
endif()

if (NOT DEFINED ATHENA_PHYSICS_STATIC)
    set(ATHENA_PHYSICS_STATIC OFF CACHE BOOL "Build Athena-Physics as a static library")
endif()

if (NOT DEFINED ATHENA_PHYSICS_GRAPHICS_SUPPORT)
    set(ATHENA_PHYSICS_GRAPHICS_SUPPORT OFF CACHE BOOL "Enable Athena-Graphics support")
endif()

# Set the output paths
if (NOT DEFINED ATHENA_BINARY_DIR)
    set(ATHENA_BINARY_DIR "${ATHENA_PHYSICS_BINARY_DIR}")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${ATHENA_PHYSICS_BINARY_DIR}/bin")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${ATHENA_PHYSICS_BINARY_DIR}/lib")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${ATHENA_PHYSICS_BINARY_DIR}/bin")
endif()

# Set the dependencies path
if (NOT DEFINED ATHENA_DEPENDENCIES_DIR)
    set(ATHENA_DEPENDENCIES_DIR "${ATHENA_PHYSICS_SOURCE_DIR}/dependencies")
endif()

# Setup the dependencies
if (NOT DEFINED ATHENA_MATH_STATIC)
    set(ATHENA_MATH_STATIC ON)
endif()

if (NOT DEFINED ATHENA_CORE_STATIC)
    set(ATHENA_CORE_STATIC ON)
endif()

if (NOT DEFINED ATHENA_ENTITIES_STATIC)
    set(ATHENA_ENTITIES_STATIC ON)
endif()

if (ATHENA_PHYSICS_GRAPHICS_SUPPORT AND NOT EXISTS "${ATHENA_DEPENDENCIES_DIR}/ogre/CMakeLists.txt")
    set(ATHENA_PHYSICS_GRAPHICS_SUPPORT OFF)
endif()

# Import XMake
if (NOT EXISTS "${ATHENA_DEPENDENCIES_DIR}/xmake/XMake.cmake")
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

include("${ATHENA_DEPENDENCIES_DIR}/xmake/XMake.cmake")

# Process the dependencies
add_subdirectory(dependencies)

# XMake settings
if (ATHENA_PHYSICS_STATIC)
    xmake_init_static(ATHENA_PHYSICS)
    xmake_add_link_paths(ATHENA_PHYSICS "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}")
    xmake_add_link_targets(ATHENA_PHYSICS Athena-Physics "${XMAKE_ATHENA_ENTITIES_LINK_TARGETS}" "${XMAKE_BULLET_LINK_TARGETS}")
else()
    xmake_init_dynamic(ATHENA_PHYSICS)
    xmake_add_link_paths(ATHENA_PHYSICS "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
    xmake_add_link_targets(ATHENA_PHYSICS Athena-Physics)
endif()

xmake_add_include_paths(ATHENA_PHYSICS ${ATHENA_PHYSICS_SOURCE_DIR}/include
                                       ${XMAKE_ATHENA_ENTITIES_INCLUDE_PATHS}
                                       ${XMAKE_BULLET_INCLUDE_PATHS})

# Subdirectories to process
add_subdirectory(include)
add_subdirectory(src)
