/**	@file	PrimitiveShape.cpp
	@author	Philip Abbet

	Implementation of the class 'Athena::Physics::PrimitiveShape'
*/

#include <Athena-Physics/PrimitiveShape.h>
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
const std::string PrimitiveShape::TYPE = "Athena/Physics/PrimitiveShape";


/***************************** CONSTRUCTION / DESTRUCTION ******************************/

PrimitiveShape::PrimitiveShape(const std::string& strName, ComponentsList* pList)
: CollisionShape(strName, pList)
{
}

//-----------------------------------------------------------------------

PrimitiveShape::~PrimitiveShape()
{
}

//-----------------------------------------------------------------------

PrimitiveShape* PrimitiveShape::create(const std::string& strName, ComponentsList* pList)
{
	return new PrimitiveShape(strName, pList);
}

//-----------------------------------------------------------------------

PrimitiveShape* PrimitiveShape::cast(Component* pComponent)
{
	return dynamic_cast<PrimitiveShape*>(pComponent);
}


/*********************************** METHODS **********************************/

void PrimitiveShape::createBox(const Math::Vector3& size)
{
    // Assertions
    assert(size.x > 0.0f);
    assert(size.y > 0.0f);
    assert(size.z > 0.0f);

    Body* pBody = m_pBody;
    if (pBody)
        pBody->setCollisionShape(0);

    delete m_pCollisionShape;
    m_pCollisionShape = new btBoxShape(toBullet(size * 0.5f));

    m_shape = SHAPE_BOX;

    if (pBody)
        pBody->setCollisionShape(this);
}

//-----------------------------------------------------------------------

void PrimitiveShape::createCapsule(const Math::Real& radius, const Math::Real& height,
                                   tAxis axis)
{
    // Assertions
    assert(radius > 0.0f);
    assert(height > 0.0f);

    Body* pBody = m_pBody;
    if (pBody)
        pBody->setCollisionShape(0);

    delete m_pCollisionShape;

    switch (axis)
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

    m_shape = SHAPE_CAPSULE;
    m_axis = axis;

    if (pBody)
        pBody->setCollisionShape(this);
}

//-----------------------------------------------------------------------

void PrimitiveShape::createCone(const Math::Real& radius, const Math::Real& height,
                                tAxis axis)
{
    // Assertions
    assert(radius > 0.0f);
    assert(height > 0.0f);

    Body* pBody = m_pBody;
    if (pBody)
        pBody->setCollisionShape(0);

    delete m_pCollisionShape;
    
    switch (axis)
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

    m_shape = SHAPE_CONE;
    m_axis = axis;

    if (pBody)
        pBody->setCollisionShape(this);
}

//-----------------------------------------------------------------------

void PrimitiveShape::createCylinder(const Math::Real& radius, const Math::Real& height,
                                    tAxis axis)
{
    // Assertions
    assert(radius > 0.0f);
    assert(height > 0.0f);

    Body* pBody = m_pBody;
    if (pBody)
        pBody->setCollisionShape(0);

    delete m_pCollisionShape;
    
    switch (axis)
    {
        case AXIS_X:
            m_pCollisionShape = new btCylinderShapeX(btVector3(height, radius, radius));
            break;

        case AXIS_Y:
            m_pCollisionShape = new btCylinderShape(btVector3(radius, height, radius));
            break;

        case AXIS_Z:
            m_pCollisionShape = new btCylinderShapeZ(btVector3(radius, radius, height));
            break;
    }

    m_shape = SHAPE_CYLINDER;
    m_axis = axis;

    if (pBody)
        pBody->setCollisionShape(this);
}

//-----------------------------------------------------------------------

void PrimitiveShape::createSphere(const Math::Real& radius)
{
    // Assertions
    assert(radius > 0.0f);

    Body* pBody = m_pBody;
    if (pBody)
        pBody->setCollisionShape(0);

    delete m_pCollisionShape;
    m_pCollisionShape = new btSphereShape(radius);

    m_shape = SHAPE_SPHERE;

    if (pBody)
        pBody->setCollisionShape(this);
}

//-----------------------------------------------------------------------

Math::Vector3 PrimitiveShape::getSize() const
{
    if (!m_pCollisionShape)
        return Vector3::ZERO;
    
    if (m_shape == SHAPE_BOX)
        return fromBullet(dynamic_cast<btBoxShape*>(m_pCollisionShape)->getHalfExtentsWithMargin()) * 2.0f;
    
    return Vector3::ZERO;
}

//-----------------------------------------------------------------------

Math::Real PrimitiveShape::getRadius() const
{
    if (!m_pCollisionShape)
        return 0.0f;
    
    switch (m_shape)
    {
        case SHAPE_CAPSULE:
            return dynamic_cast<btCapsuleShape*>(m_pCollisionShape)->getRadius();

        case SHAPE_CONE:
            return dynamic_cast<btConeShape*>(m_pCollisionShape)->getRadius();

        case SHAPE_CYLINDER:
            return dynamic_cast<btCylinderShape*>(m_pCollisionShape)->getRadius();

        case SHAPE_SPHERE:
            return dynamic_cast<btSphereShape*>(m_pCollisionShape)->getRadius();
    }
    
    return 0.0f;
}

//-----------------------------------------------------------------------

Math::Real PrimitiveShape::getHeight() const
{
    if (!m_pCollisionShape)
        return 0.0f;
    
    switch (m_shape)
    {
        case SHAPE_CAPSULE:
            return dynamic_cast<btCapsuleShape*>(m_pCollisionShape)->getHalfHeight();

        case SHAPE_CONE:
            return dynamic_cast<btConeShape*>(m_pCollisionShape)->getHeight();

        case SHAPE_CYLINDER:
            switch (m_axis)
            {
                case AXIS_X:
                    return dynamic_cast<btCylinderShape*>(m_pCollisionShape)->getHalfExtentsWithMargin().getX();

                case AXIS_Y:
                    return dynamic_cast<btCylinderShape*>(m_pCollisionShape)->getHalfExtentsWithMargin().getY();

                case AXIS_Z:
                    return dynamic_cast<btCylinderShape*>(m_pCollisionShape)->getHalfExtentsWithMargin().getZ();
            }
    }
    
    return 0.0f;
}


/***************************** MANAGEMENT OF THE PROPERTIES ****************************/

Utils::PropertiesList* PrimitiveShape::getProperties() const
{
	// Call the base class implementation
	PropertiesList* pProperties = Component::getProperties();

	// Create the category belonging to this type
	pProperties->selectCategory(TYPE, false);

    if (m_pCollisionShape)
    {
        Variant* pStruct = new Variant(Variant::STRUCT);
        
        switch (m_shape)
        {
            case SHAPE_BOX:
                pStruct->setField("type", new Variant("BOX"));
                pStruct->setField("size", new Variant(getSize()));
                break;

            case SHAPE_CAPSULE:
                pStruct->setField("type", new Variant("CAPSULE"));
                pStruct->setField("radius", new Variant(getRadius()));
                pStruct->setField("height", new Variant(getHeight()));
                break;

            case SHAPE_CONE:
                pStruct->setField("type", new Variant("CONE"));
                pStruct->setField("radius", new Variant(getRadius()));
                pStruct->setField("height", new Variant(getHeight()));
                break;

            case SHAPE_CYLINDER:
                pStruct->setField("type", new Variant("CYLINDER"));
                pStruct->setField("radius", new Variant(getRadius()));
                pStruct->setField("height", new Variant(getHeight()));
                break;

            case SHAPE_SPHERE:
                pStruct->setField("type", new Variant("SPHERE"));
                pStruct->setField("radius", new Variant(getRadius()));
                break;
        }

        if ((m_shape == SHAPE_CAPSULE) || (m_shape == SHAPE_CONE) || (m_shape == SHAPE_CYLINDER))
        {
            switch (m_axis)
            {
                case AXIS_X:
                    pStruct->setField("axis", new Variant("X"));
                    break;

                case AXIS_Y:
                    pStruct->setField("axis", new Variant("Y"));
                    break;

                case AXIS_Z:
                    pStruct->setField("axis", new Variant("Z"));
                    break;
            }
        }
        
        pProperties->set("shape", pStruct);
    }

	// Returns the list
	return pProperties;
}

//-----------------------------------------------------------------------

bool PrimitiveShape::setProperty(const std::string& strCategory, const std::string& strName,
								Utils::Variant* pValue)
{
	assert(!strCategory.empty());
	assert(!strName.empty());
	assert(pValue);

	if (strCategory == TYPE)
		return PrimitiveShape::setProperty(strName, pValue);

	return Component::setProperty(strCategory, strName, pValue);
}

//-----------------------------------------------------------------------

bool PrimitiveShape::setProperty(const std::string& strName, Utils::Variant* pValue)
{
	// Assertions
	assert(!strName.empty());
	assert(pValue);

    // Shape
	if (strName == "shape")
	{
        string strType;
        Vector3 size(1.0f, 1.0f, 1.0f);
        float radius = 1.0f;
        float height = 1.0f;
        tAxis axis = AXIS_Y;
	    
        Variant* pField = pValue->getField("type");
        if (pField)
            strType = pField->toString();

        pField = pValue->getField("size");
        if (pField)
            size = pField->toVector3();

        pField = pValue->getField("radius");
        if (pField)
            radius = pField->toFloat();

        pField = pValue->getField("height");
        if (pField)
            height = pField->toFloat();

        pField = pValue->getField("axis");
        if (pField)
        {
            if (pField->toString() == "X")
                axis = AXIS_X;
            else if (pField->toString() == "Z")
                axis = AXIS_Z;
        }
        
        if (strType == "BOX")
            createBox(size);
        else if (strType == "CAPSULE")
            createCapsule(radius, height, axis);
        else if (strType == "CONE")
            createCone(radius, height, axis);
        else if (strType == "CYLINDER")
            createCylinder(radius, height, axis);
        else if (strType == "SPHERE")
            createSphere(radius);
    }

	// Destroy the value
	delete pValue;

	return true;
}
