/**	@file	module.cpp
	@author	Philip Abbet

	Implementation of the module-level functions
*/

#include <Athena-Physics/Prerequisites.h>
#include <Athena-Physics/Body.h>
#include <Athena-Physics/BoxShape.h>
#include <Athena-Physics/CapsuleShape.h>
#include <Athena-Physics/CollisionShape.h>
#include <Athena-Physics/ConeShape.h>
#include <Athena-Physics/CylinderShape.h>
#include <Athena-Physics/PhysicalComponent.h>
#include <Athena-Physics/SphereShape.h>
#include <Athena-Physics/StaticTriMeshShape.h>
#include <Athena-Physics/World.h>
#include <Athena-Entities/ComponentsManager.h>

using namespace Athena::Entities;


namespace Athena {
namespace Physics {


void initialize()
{
    assert(ComponentsManager::getSingletonPtr);
    
    ComponentsManager* pComponentsManager = ComponentsManager::getSingletonPtr();
    
    pComponentsManager->registerType(Body::TYPE,                (ComponentCreationMethod*) &Body::create);
    pComponentsManager->registerType(BoxShape::TYPE,            (ComponentCreationMethod*) &BoxShape::create);
    pComponentsManager->registerType(CapsuleShape::TYPE,        (ComponentCreationMethod*) &CapsuleShape::create);
    pComponentsManager->registerType(CollisionShape::TYPE,      (ComponentCreationMethod*) &CollisionShape::create);
    pComponentsManager->registerType(ConeShape::TYPE,           (ComponentCreationMethod*) &ConeShape::create);
    pComponentsManager->registerType(CylinderShape::TYPE,       (ComponentCreationMethod*) &CylinderShape::create);
    pComponentsManager->registerType(PhysicalComponent::TYPE,   (ComponentCreationMethod*) &PhysicalComponent::create);
    pComponentsManager->registerType(SphereShape::TYPE,         (ComponentCreationMethod*) &SphereShape::create);
    pComponentsManager->registerType(StaticTriMeshShape::TYPE,  (ComponentCreationMethod*) &StaticTriMeshShape::create);
    pComponentsManager->registerType(World::TYPE,               (ComponentCreationMethod*) &World::create);
}

}
}
