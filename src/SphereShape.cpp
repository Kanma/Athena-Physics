/**	@file	SphereShape.cpp
	@author	Philip Abbet

	Implementation of the class 'Athena::Physics::SphereShape'
*/

#include <Athena-Physics/SphereShape.h>
#include <Athena-Physics/Body.h>
#include <Athena-Physics/Conversions.h>

using namespace Athena;
using namespace Athena::Physics;
using namespace Athena::Entities;
using namespace Athena::Utils;
using namespace Athena::Math;
using namespace std;


/************************************** CONSTANTS **************************************/

///< Name of the type of component
const std::string SphereShape::TYPE = "Athena/Physics/SphereShape";


/***************************** CONSTRUCTION / DESTRUCTION ******************************/

SphereShape::SphereShape(const std::string& strName, ComponentsList* pList)
: CollisionShape(strName, pList)
{
    m_pCollisionShape = new btSphereShape(0.5f);
}

//-----------------------------------------------------------------------

SphereShape::~SphereShape()
{
}

//-----------------------------------------------------------------------

SphereShape* SphereShape::create(const std::string& strName, ComponentsList* pList)
{
	return new SphereShape(strName, pList);
}

//-----------------------------------------------------------------------

SphereShape* SphereShape::cast(Component* pComponent)
{
	return dynamic_cast<SphereShape*>(pComponent);
}


/*********************************** METHODS **********************************/

void SphereShape::setRadius(const Math::Real& radius)
{
    assert(radius > 0.0f);

    Body* pBody = m_pBody;
    if (pBody)
        pBody->setCollisionShape(0);

    delete m_pCollisionShape;
    m_pCollisionShape = new btSphereShape(radius);

    if (pBody)
        pBody->setCollisionShape(this);
}

//-----------------------------------------------------------------------

Math::Real SphereShape::getRadius() const
{
    return getSphereShape()->getRadius();
}


/***************************** MANAGEMENT OF THE PROPERTIES ****************************/

Utils::PropertiesList* SphereShape::getProperties() const
{
	// Call the base class implementation
	PropertiesList* pProperties = Component::getProperties();

	// Create the category belonging to this type
	pProperties->selectCategory(TYPE, false);

	// Radius
    pProperties->set("radius", new Variant(getRadius()));

	// Returns the list
	return pProperties;
}

//-----------------------------------------------------------------------

bool SphereShape::setProperty(const std::string& strCategory, const std::string& strName,
								Utils::Variant* pValue)
{
	assert(!strCategory.empty());
	assert(!strName.empty());
	assert(pValue);

	if (strCategory == TYPE)
		return SphereShape::setProperty(strName, pValue);

	return Component::setProperty(strCategory, strName, pValue);
}

//-----------------------------------------------------------------------

bool SphereShape::setProperty(const std::string& strName, Utils::Variant* pValue)
{
	// Assertions
	assert(!strName.empty());
	assert(pValue);

    // Radius
	if (strName == "radius")
	{
	    setRadius(pValue->toFloat());
    }

	// Destroy the value
	delete pValue;

	return true;
}
