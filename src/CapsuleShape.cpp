/**	@file	CapsuleShape.cpp
	@author	Philip Abbet

	Implementation of the class 'Athena::Physics::CapsuleShape'
*/

#include <Athena-Physics/CapsuleShape.h>
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
const std::string CapsuleShape::TYPE = "Athena/Physics/CapsuleShape";


/***************************** CONSTRUCTION / DESTRUCTION ******************************/

CapsuleShape::CapsuleShape(const std::string& strName, ComponentsList* pList)
: CollisionShape(strName, pList), m_axis(AXIS_Y)
{
    m_pCollisionShape = new btCapsuleShape(0.25f, 0.5f);
}

//-----------------------------------------------------------------------

CapsuleShape::~CapsuleShape()
{
}

//-----------------------------------------------------------------------

CapsuleShape* CapsuleShape::create(const std::string& strName, ComponentsList* pList)
{
	return new CapsuleShape(strName, pList);
}

//-----------------------------------------------------------------------

CapsuleShape* CapsuleShape::cast(Component* pComponent)
{
	return dynamic_cast<CapsuleShape*>(pComponent);
}


/*********************************** METHODS **********************************/

void CapsuleShape::setDimensions(const Math::Real& radius, const Math::Real& height,
                                 tAxis axis)
{
    assert(radius > 0.0f);
    assert(height > 0.0f);

    Body* pBody = m_pBody;
    if (pBody)
        pBody->setCollisionShape(0);

    delete m_pCollisionShape;
    
    switch (m_axis)
    {
        case AXIS_X:
            m_pCollisionShape = new btCapsuleShapeX(radius, height);
            break;

        case AXIS_Y:
            m_pCollisionShape = new btCapsuleShape(radius, height);
            break;

        case AXIS_Z:
            m_pCollisionShape = new btCapsuleShapeZ(radius, height);
            break;
    }

    if (pBody)
        pBody->setCollisionShape(this);
}

//-----------------------------------------------------------------------

Math::Real CapsuleShape::getRadius() const
{
    return getCapsuleShape()->getRadius();
}

//-----------------------------------------------------------------------

Math::Real CapsuleShape::getHeight() const
{
    return getCapsuleShape()->getHalfHeight();
}


/***************************** MANAGEMENT OF THE PROPERTIES ****************************/

Utils::PropertiesList* CapsuleShape::getProperties() const
{
	// Call the base class implementation
	PropertiesList* pProperties = Component::getProperties();

	// Create the category belonging to this type
	pProperties->selectCategory(TYPE, false);

	// Radius
    pProperties->set("radius", new Variant(getRadius()));

	// Height
    pProperties->set("height", new Variant(getHeight()));

	// Axis
	switch (m_axis)
	{
        case AXIS_X:
            pProperties->set("axis", new Variant("X"));
            break;

        case AXIS_Y:
            pProperties->set("axis", new Variant("Y"));
            break;

        case AXIS_Z:
            pProperties->set("axis", new Variant("Z"));
            break;
    }
    
	// Returns the list
	return pProperties;
}

//-----------------------------------------------------------------------

bool CapsuleShape::setProperty(const std::string& strCategory, const std::string& strName,
								Utils::Variant* pValue)
{
	assert(!strCategory.empty());
	assert(!strName.empty());
	assert(pValue);

	if (strCategory == TYPE)
		return CapsuleShape::setProperty(strName, pValue);

	return Component::setProperty(strCategory, strName, pValue);
}

//-----------------------------------------------------------------------

bool CapsuleShape::setProperty(const std::string& strName, Utils::Variant* pValue)
{
	// Assertions
	assert(!strName.empty());
	assert(pValue);

    // Radius
	if (strName == "radius")
	{
	    setDimensions(pValue->toFloat(), getHeight(), m_axis);
    }

    // Height
	else if (strName == "height")
	{
	    setDimensions(getRadius(), pValue->toFloat(), m_axis);
    }

    // Axis
	else if (strName == "axis")
	{
	    if (pValue->toString() == "X")
            m_axis = AXIS_X;
	    else if (pValue->toString() == "Z")
            m_axis = AXIS_Z;
        else
            m_axis = AXIS_Y;
	    
	    setDimensions(getRadius(), getHeight(), m_axis);
    }

	// Destroy the value
	delete pValue;

	return true;
}
