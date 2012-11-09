# File generation
configure_file(${ATHENA_PHYSICS_SOURCE_DIR}/src/module.cpp.in
               ${XMAKE_BINARY_DIR}/generated/Athena-Physics/module.cpp)


# List the headers files
set(HEADERS ${XMAKE_BINARY_DIR}/include/Athena-Physics/Config.h
            ../include/Athena-Physics/Body.h
            ../include/Athena-Physics/CollisionManager.h
            ../include/Athena-Physics/CollisionObject.h
            ../include/Athena-Physics/CollisionShape.h
            ../include/Athena-Physics/CompoundShape.h
            ../include/Athena-Physics/Conversions.h
            ../include/Athena-Physics/GhostObject.h
            ../include/Athena-Physics/PhysicalComponent.h
            ../include/Athena-Physics/Prerequisites.h
            ../include/Athena-Physics/PrimitiveShape.h
            ../include/Athena-Physics/StaticTriMeshShape.h
            ../include/Athena-Physics/World.h
)


# List the source files
set(SRCS ${XMAKE_BINARY_DIR}/generated/Athena-Physics/module.cpp
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

if (DEFINED ATHENA_SCRIPTING_ENABLED AND ATHENA_SCRIPTING_ENABLED)
    set(HEADERS ${HEADERS} ../include/Athena-Physics/Scripting.h)
    set(SRCS ${SRCS} Scripting.cpp)
endif()


# List the include paths
set(INCLUDE_PATHS "${ATHENA_PHYSICS_SOURCE_DIR}/include"
                  "${XMAKE_BINARY_DIR}/include")

include_directories(${INCLUDE_PATHS})

xmake_import_search_paths(ATHENA_ENTITIES)
xmake_import_search_paths(BULLET)

if (DEFINED ATHENA_SCRIPTING_ENABLED AND ATHENA_SCRIPTING_ENABLED)
    xmake_import_search_paths(ATHENA_SCRIPTING)
endif()


# Declaration of the library
xmake_create_dynamic_library(ATHENA_PHYSICS Athena-Physics
                             "${ATHENA_PHYSICS_VERSION_MAJOR}.${ATHENA_PHYSICS_VERSION_MINOR}.${ATHENA_PHYSICS_VERSION_PATCH}"
                             "${ATHENA_PHYSICS_VERSION_MAJOR}.${ATHENA_PHYSICS_VERSION_MINOR}.${ATHENA_PHYSICS_VERSION_PATCH}"
                             ${HEADERS} ${SRCS})

xmake_add_to_list_property(ATHENA_PHYSICS COMPILE_DEFINITIONS "ATHENA_PHYSICS_EXPORTS")

xmake_project_link(ATHENA_PHYSICS ATHENA_ENTITIES BULLET)

if (DEFINED ATHENA_SCRIPTING_ENABLED AND ATHENA_SCRIPTING_ENABLED)
    xmake_project_link(ATHENA_PHYSICS ATHENA_SCRIPTING)
endif()


# Exportations
xmake_export_include_paths(ATHENA_PHYSICS ${INCLUDE_PATHS})
