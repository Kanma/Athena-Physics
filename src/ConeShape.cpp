/**	@file	ConeShape.cpp
	@author	Philip Abbet

	Implementation of the class 'Athena::Physics::ConeShape'
*/

#include <Athena-Physics/ConeShape.h>
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
const std::string ConeShape::TYPE = "Athena/Physics/ConeShape";


/***************************** CONSTRUCTION / DESTRUCTION ******************************/

ConeShape::ConeShape(const std::string& strName, ComponentsList* pList)
: CollisionShape(strName, pList), m_axis(AXIS_Y)
{
    m_pCollisionShape = new btConeShape(0.25f, 1.0f);
}

//-----------------------------------------------------------------------

ConeShape::~ConeShape()
{
}

//-----------------------------------------------------------------------

ConeShape* ConeShape::create(const std::string& strName, ComponentsList* pList)
{
	return new ConeShape(strName, pList);
}

//-----------------------------------------------------------------------

ConeShape* ConeShape::cast(Component* pComponent)
{
	return dynamic_cast<ConeShape*>(pComponent);
}


/*********************************** METHODS **********************************/

void ConeShape::setDimensions(const Math::Real& radius, const Math::Real& height,
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
            m_pCollisionShape = new btConeShapeX(radius, height);
            break;

        case AXIS_Y:
            m_pCollisionShape = new btConeShape(radius, height);
            break;

        case AXIS_Z:
            m_pCollisionShape = new btConeShapeZ(radius, height);
            break;
    }

    if (pBody)
        pBody->setCollisionShape(this);
}

//-----------------------------------------------------------------------

Math::Real ConeShape::getRadius() const
{
    return getConeShape()->getRadius();
}

//-----------------------------------------------------------------------

Math::Real ConeShape::getHeight() const
{
    return getConeShape()->getHeight();
}


/***************************** MANAGEMENT OF THE PROPERTIES ****************************/

Utils::PropertiesList* ConeShape::getProperties() const
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

bool ConeShape::setProperty(const std::string& strCategory, const std::string& strName,
								Utils::Variant* pValue)
{
	assert(!strCategory.empty());
	assert(!strName.empty());
	assert(pValue);

	if (strCategory == TYPE)
		return ConeShape::setProperty(strName, pValue);

	return Component::setProperty(strCategory, strName, pValue);
}

//-----------------------------------------------------------------------

bool ConeShape::setProperty(const std::string& strName, Utils::Variant* pValue)
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
