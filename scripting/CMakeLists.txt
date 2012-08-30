# Setup the search paths
xmake_import_search_paths(ATHENA_SCRIPTING)
xmake_import_search_paths(ATHENA_PHYSICS)


# List the source files
set(SRCS module.cpp
         CollisionShape.cpp
         PhysicalComponent.cpp
         World.cpp
)

set(JS_SRCS ""
)


# Declaration of the module
xmake_create_dynamic_library(ATHENA_PHYSICS_SCRIPTING_MODULE Athena-Physics-Scripting-module
                            "${ATHENA_PHYSICS_VERSION_MAJOR}.${ATHENA_PHYSICS_VERSION_MINOR}.${ATHENA_PHYSICS_VERSION_PATCH}"
                            "${ATHENA_PHYSICS_VERSION_MAJOR}.${ATHENA_PHYSICS_VERSION_MINOR}.${ATHENA_PHYSICS_VERSION_PATCH}"
                             ${SRCS})

xmake_project_link(ATHENA_PHYSICS_SCRIPTING_MODULE ATHENA_PHYSICS ATHENA_SCRIPTING)

create_scripting_module(ATHENA_PHYSICS_SCRIPTING_MODULE "Athena.Physics" "${JS_SRCS}")

# Disable some warnings in Visual Studio
xmake_disable_vs_warning(ATHENA_PHYSICS_SCRIPTING_MODULE 4244 NO)
