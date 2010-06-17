/**	@file	BoxShape.cpp
	@author	Philip Abbet

	Implementation of the class 'Athena::Physics::BoxShape'
*/

#include <Athena-Physics/BoxShape.h>
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
const std::string BoxShape::TYPE = "Athena/Physics/BoxShape";


/***************************** CONSTRUCTION / DESTRUCTION ******************************/

BoxShape::BoxShape(const std::string& strName, ComponentsList* pList)
: CollisionShape(strName, pList)
{
    m_pCollisionShape = new btBoxShape(btVector3(0.5f, 0.5f, 0.5f));
}

//-----------------------------------------------------------------------

BoxShape::~BoxShape()
{
}

//-----------------------------------------------------------------------

BoxShape* BoxShape::create(const std::string& strName, ComponentsList* pList)
{
	return new BoxShape(strName, pList);
}

//-----------------------------------------------------------------------

BoxShape* BoxShape::cast(Component* pComponent)
{
	return dynamic_cast<BoxShape*>(pComponent);
}


/*********************************** METHODS **********************************/

void BoxShape::setSize(const Math::Vector3& halfExtents)
{
    assert(halfExtents.x > 0.0f);
    assert(halfExtents.y > 0.0f);
    assert(halfExtents.z > 0.0f);

    Body* pBody = m_pBody;
    if (pBody)
        pBody->setCollisionShape(0);

    delete m_pCollisionShape;
    m_pCollisionShape = new btBoxShape(toBullet(halfExtents));

    if (pBody)
        pBody->setCollisionShape(this);
}

//-----------------------------------------------------------------------

Math::Vector3 BoxShape::getSize() const
{
    return fromBullet(getBoxShape()->getHalfExtentsWithMargin());
}


/***************************** MANAGEMENT OF THE PROPERTIES ****************************/

Utils::PropertiesList* BoxShape::getProperties() const
{
	// Call the base class implementation
	PropertiesList* pProperties = Component::getProperties();

	// Create the category belonging to this type
	pProperties->selectCategory(TYPE, false);

	// Half-extents
    pProperties->set("half-extents", new Variant(getSize()));

	// Returns the list
	return pProperties;
}

//-----------------------------------------------------------------------

bool BoxShape::setProperty(const std::string& strCategory, const std::string& strName,
								Utils::Variant* pValue)
{
	assert(!strCategory.empty());
	assert(!strName.empty());
	assert(pValue);

	if (strCategory == TYPE)
		return BoxShape::setProperty(strName, pValue);

	return Component::setProperty(strCategory, strName, pValue);
}

//-----------------------------------------------------------------------

bool BoxShape::setProperty(const std::string& strName, Utils::Variant* pValue)
{
	// Assertions
	assert(!strName.empty());
	assert(pValue);

    // Half-extents
	if (strName == "half-extents")
	{
	    setSize(pValue->toVector3());
    }

	// Destroy the value
	delete pValue;

	return true;
}
