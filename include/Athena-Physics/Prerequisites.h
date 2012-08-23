/** @file   Prerequisites.h
    @author Philip Abbet

    Declaration of the types of the Athena-Physics module
*/

#ifndef _ATHENA_PHYSICS_PREREQUISITES_H_
#define _ATHENA_PHYSICS_PREREQUISITES_H_

#include <Athena-Core/Prerequisites.h>
#include <Athena-Physics/Config.h>
#include <Bullet/btConfig.h>
#include <btBulletDynamicsCommon.h>


/// Used to export symbols from the library
#if (ATHENA_PLATFORM == ATHENA_PLATFORM_WIN32) && !ATHENA_PHYSICS_STATIC
#    ifdef ATHENA_PHYSICS_EXPORTS
#        define ATHENA_PHYSICS_SYMBOL   __declspec(dllexport)
#    else
#        define ATHENA_PHYSICS_SYMBOL   __declspec(dllimport)
#    endif
#else
#    define ATHENA_PHYSICS_SYMBOL
#endif


//----------------------------------------------------------------------------------------
/// @brief  Main namespace. All the components of the Athena engine belongs to this
///         namespace
//----------------------------------------------------------------------------------------
namespace Athena
{
    //------------------------------------------------------------------------------------
    /// @brief  Contains all the physics-related classes
    //------------------------------------------------------------------------------------
    namespace Physics
    {
        class Body;
        class CollisionManager;
        class CollisionObject;
        class CollisionShape;
        class GhostObject;
        class PhysicalComponent;
        class World;

        class CompoundShape;
        class PrimitiveShape;
        class StaticTriMeshShape;

        //------------------------------------------------------------------------------------
        /// @brief  Represents a collision group (from 0 to 31)
        //------------------------------------------------------------------------------------
        typedef unsigned char tCollisionGroup;

        //------------------------------------------------------------------------------------
        /// @brief  Initialize the Physics module
        //------------------------------------------------------------------------------------
        extern void initialize();

        ATHENA_PHYSICS_SYMBOL extern const char* VERSION;
    }
}

#endif
