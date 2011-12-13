/**	@file	CollisionShape.cpp
	@author	Philip Abbet

	Implementation of the class 'Athena::Physics::CollisionShape'
*/

#include <Athena-Physics/CollisionShape.h>
#include <Athena-Physics/World.h>
#include <Athena-Physics/Conversions.h>
#include <Athena-Entities/Transforms.h>
#include <Athena-Math/MathUtils.h>

using namespace Athena;
using namespace Athena::Physics;
using namespace Athena::Entities;
using namespace Athena::Utils;
using namespace Athena::Math;
using namespace std;


/************************************** CONSTANTS **************************************/

///< Name of the type of component
const std::string CollisionShape::TYPE = "Athena/Physics/CollisionShape";


/***************************** CONSTRUCTION / DESTRUCTION ******************************/

CollisionShape::CollisionShape(const std::string& strName, ComponentsList* pList)
: PhysicalComponent(strName, pList), m_pCollisionShape(0), m_pBody(0)
{
}

//-----------------------------------------------------------------------

CollisionShape::~CollisionShape()
{
    assert(!m_pBody);
    
    delete m_pCollisionShape;
}

//-----------------------------------------------------------------------

CollisionShape* CollisionShape::create(const std::string& strName, ComponentsList* pList)
{
	return new CollisionShape(strName, pList);
}

//-----------------------------------------------------------------------

CollisionShape* CollisionShape::cast(Component* pComponent)
{
	return dynamic_cast<CollisionShape*>(pComponent);
}


/***************************** MANAGEMENT OF THE PROPERTIES ****************************/

Utils::PropertiesList* CollisionShape::getProperties() const
{
	// Call the base class implementation
	PropertiesList* pProperties = PhysicalComponent::getProperties();

	// Create the category belonging to this type
	pProperties->selectCategory(TYPE, false);

	// Returns the list
	return pProperties;
}

//-----------------------------------------------------------------------

bool CollisionShape::setProperty(const std::string& strCategory, const std::string& strName,
								Utils::Variant* pValue)
{
	assert(!strCategory.empty());
	assert(!strName.empty());
	assert(pValue);

	if (strCategory == TYPE)
		return CollisionShape::setProperty(strName, pValue);

	return PhysicalComponent::setProperty(strCategory, strName, pValue);
}

//-----------------------------------------------------------------------

bool CollisionShape::setProperty(const std::string& strName, Utils::Variant* pValue)
{
	// Assertions
	assert(!strName.empty());
	assert(pValue);

	// Destroy the value
	delete pValue;

	return true;
}
