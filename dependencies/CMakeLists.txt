# Subdirectories to process
xmake_add_subdirectory(UnitTest++)
xmake_add_subdirectory(Athena-Math)
xmake_add_subdirectory(Athena-Core)
xmake_add_subdirectory(Athena-Entities)
xmake_add_subdirectory(Bullet)

if (ATHENA_PHYSICS_GRAPHICS_SUPPORT AND EXISTS "Ogre/CMakeLists.txt")
    xmake_add_subdirectory(Ogre)
endif()
