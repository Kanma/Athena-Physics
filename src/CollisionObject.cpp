/**	@file	CollisionObject.cpp
	@author	Philip Abbet

	Implementation of the class 'Athena::Physics::CollisionObject'
*/

#include <Athena-Physics/CollisionObject.h>
#include <Athena-Physics/World.h>
#include <Athena-Physics/Conversions.h>
#include <Athena-Math/MathUtils.h>

using namespace Athena;
using namespace Athena::Physics;
using namespace Athena::Entities;
using namespace Athena::Utils;
using namespace std;


/************************************** CONSTANTS **************************************/

///< Name of the type of component
const std::string CollisionObject::TYPE = "Athena/Physics/CollisionObject";


/***************************** CONSTRUCTION / DESTRUCTION ******************************/

CollisionObject::CollisionObject(const std::string& strName, ComponentsList* pList)
: PhysicalComponent(strName, pList), m_collisionGroup(255)
{
}

//-----------------------------------------------------------------------

CollisionObject::~CollisionObject()
{
}

//-----------------------------------------------------------------------

CollisionObject* CollisionObject::create(const std::string& strName, ComponentsList* pList)
{
	return new CollisionObject(strName, pList);
}

//-----------------------------------------------------------------------

CollisionObject* CollisionObject::cast(Component* pComponent)
{
	return dynamic_cast<CollisionObject*>(pComponent);
}


/***************************** MANAGEMENT OF THE PROPERTIES ****************************/

Utils::PropertiesList* CollisionObject::getProperties() const
{
	// Call the base class implementation
	PropertiesList* pProperties = PhysicalComponent::getProperties();

	// Create the category belonging to this type
	pProperties->selectCategory(TYPE, false);

    // Group
    pProperties->set("collision-group", new Variant(m_collisionGroup));

	// Returns the list
	return pProperties;
}

//-----------------------------------------------------------------------

bool CollisionObject::setProperty(const std::string& strCategory, const std::string& strName,
								Utils::Variant* pValue)
{
	assert(!strCategory.empty());
	assert(!strName.empty());
	assert(pValue);

	if (strCategory == TYPE)
		return CollisionObject::setProperty(strName, pValue);

	return PhysicalComponent::setProperty(strCategory, strName, pValue);
}

//-----------------------------------------------------------------------

bool CollisionObject::setProperty(const std::string& strName, Utils::Variant* pValue)
{
	// Assertions
	assert(!strName.empty());
	assert(pValue);

	// Group
	if (strName == "collision-group")
        setCollisionGroup(pValue->toUChar());

	// Destroy the value
	delete pValue;

	return true;
}
