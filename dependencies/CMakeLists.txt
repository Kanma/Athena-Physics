# Subdirectories to process
add_subdirectory(UnitTest++)
add_subdirectory(Athena-Math)
add_subdirectory(Athena-Core)
add_subdirectory(Athena-Entities)
add_subdirectory(bullet)

if (ATHENA_PHYSICS_GRAPHICS_SUPPORT AND EXISTS "ogre/CMakeLists.txt")
    add_subdirectory(ogre)
endif()
