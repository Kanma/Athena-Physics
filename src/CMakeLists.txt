# List the source files
set(SRCS module.cpp
         Body.cpp
         CollisionManager.cpp
         CollisionObject.cpp
         CollisionShape.cpp
         Conversions.cpp
         CompoundShape.cpp
         GhostObject.cpp
         PhysicalComponent.cpp
         PrimitiveShape.cpp
         StaticTriMeshShape.cpp
         World.cpp
)


# List the include paths
set(INCLUDE_PATHS "${ATHENA_PHYSICS_SOURCE_DIR}/include"
                  "${XMAKE_BINARY_DIR}/include")

include_directories(${INCLUDE_PATHS})

xmake_import_search_paths(ATHENA_ENTITIES)
xmake_import_search_paths(BULLET)


# Declaration of the library
if (NOT ATHENA_PHYSICS_STATIC)
    xmake_create_dynamic_library(ATHENA_PHYSICS Athena-Physics 1.0.0 1.0.0 ${SRCS})
else()
    xmake_create_static_library(ATHENA_PHYSICS Athena-Physics ${SRCS})
endif()

xmake_project_link(ATHENA_PHYSICS ATHENA_ENTITIES BULLET)

xmake_add_to_list_property(ATHENA_PHYSICS COMPILE_DEFINITIONS "ATHENA_EXPORTS")


# Exportations
xmake_export_include_paths(ATHENA_PHYSICS ${INCLUDE_PATHS})
