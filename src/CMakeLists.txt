# List the source files
set(SRCS Body.cpp
         CollisionShape.cpp
         Conversions.cpp
         PhysicalComponent.cpp
         PrimitiveShape.cpp
         StaticTriMeshShape.cpp
         World.cpp
)

# Precompiler definitions
add_definitions(-DATHENA_EXPORTS)

# List the include paths
xmake_include_directories(ATHENA_PHYSICS)

# Declaration of the library
if (NOT ATHENA_PHYSICS_STATIC)
    add_library(Athena-Physics SHARED ${SRCS} ${PLATFORM_SRCS})
    xmake_target_link_libraries(Athena-Physics ATHENA_ENTITIES)
    xmake_target_link_libraries(Athena-Physics BULLET)

    # Set the INSTALL_PATH so that the dynamic library can be local
    set_target_properties(Athena-Physics PROPERTIES BUILD_WITH_INSTALL_RPATH ON
                                                    INSTALL_NAME_DIR "@rpath"
    )
else()
    add_library(Athena-Physics STATIC ${SRCS} ${PLATFORM_SRCS})
endif()
