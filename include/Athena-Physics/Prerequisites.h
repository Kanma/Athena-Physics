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


//----------------------------------------------------------------------------------------
/// @brief	Main namespace. All the components of the Athena engine belongs to this
///			namespace
//----------------------------------------------------------------------------------------
namespace Athena
{
	//------------------------------------------------------------------------------------
	/// @brief	Contains all the physics-related classes
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
    	/// @brief	Represents a collision group (bitwise mask, 31 possible values)
    	//------------------------------------------------------------------------------------
        typedef unsigned int tCollisionGroup;

    	//------------------------------------------------------------------------------------
    	/// @brief	Initialize the Physics module
    	//------------------------------------------------------------------------------------
        extern void initialize();
    }
}

#endif
