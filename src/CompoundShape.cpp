/**	@file	CompoundShape.cpp
	@author	Philip Abbet

	Implementation of the class 'Athena::Physics::CompoundShape'
*/

#include <Athena-Physics/CompoundShape.h>
#include <Athena-Physics/Body.h>
#include <Athena-Physics/Conversions.h>
#include <Athena-Core/Utils/StringConverter.h>
#include <Athena-Core/Utils/StringUtils.h>

using namespace Athena;
using namespace Athena::Physics;
using namespace Athena::Entities;
using namespace Athena::Utils;
using namespace Athena::Math;
using namespace std;


/************************************** CONSTANTS **************************************/

///< Name of the type of component
const std::string CompoundShape::TYPE = "Athena/Physics/CompoundShape";


/***************************** CONSTRUCTION / DESTRUCTION ******************************/

CompoundShape::CompoundShape(const std::string& strName, ComponentsList* pList)
: CollisionShape(strName, pList)
{
    m_pCollisionShape = new btCompoundShape();
}

//-----------------------------------------------------------------------

CompoundShape::~CompoundShape()
{
}

//-----------------------------------------------------------------------

CompoundShape* CompoundShape::create(const std::string& strName, ComponentsList* pList)
{
	return new CompoundShape(strName, pList);
}

//-----------------------------------------------------------------------

CompoundShape* CompoundShape::cast(Component* pComponent)
{
	return dynamic_cast<CompoundShape*>(pComponent);
}


/*********************************** METHODS **********************************/

void CompoundShape::addBox(const Math::Vector3& size, const Math::Vector3& position,
                           const Math::Quaternion& orientation)
{
    // Assertions
    assert(size.x > 0.0f);
    assert(size.y > 0.0f);
    assert(size.z > 0.0f);
    assert(m_pCollisionShape);

    Body* pBody = m_pBody;
    if (pBody)
        pBody->setCollisionShape(0);

    btBoxShape* pShape = new btBoxShape(toBullet(size * 0.5f));
    dynamic_cast<btCompoundShape*>(m_pCollisionShape)->addChildShape(
        btTransform(toBullet(orientation), toBullet(position)), pShape);

    if (pBody)
        pBody->setCollisionShape(this);
}

//-----------------------------------------------------------------------

void CompoundShape::addCapsule(const Math::Real& radius, const Math::Real& height,
                               PrimitiveShape::tAxis axis, const Math::Vector3& position,
                               const Math::Quaternion& orientation)
{
    // Assertions
    assert(radius > 0.0f);
    assert(height > 0.0f);
    assert(m_pCollisionShape);

    Body* pBody = m_pBody;
    if (pBody)
        pBody->setCollisionShape(0);

    btCapsuleShape* pShape;
    switch (axis)
    {
        case PrimitiveShape::AXIS_X:
            pShape = new btCapsuleShapeX(radius, height);
            break;

        case PrimitiveShape::AXIS_Y:
            pShape = new btCapsuleShape(radius, height);
            break;

        case PrimitiveShape::AXIS_Z:
            pShape = new btCapsuleShapeZ(radius, height);
            break;
    }

    dynamic_cast<btCompoundShape*>(m_pCollisionShape)->addChildShape(
        btTransform(toBullet(orientation), toBullet(position)), pShape);

    if (pBody)
        pBody->setCollisionShape(this);
}

//-----------------------------------------------------------------------

void CompoundShape::addCone(const Math::Real& radius, const Math::Real& height,
                            PrimitiveShape::tAxis axis, const Math::Vector3& position,
                            const Math::Quaternion& orientation)
{
    // Assertions
    assert(radius > 0.0f);
    assert(height > 0.0f);
    assert(m_pCollisionShape);

    Body* pBody = m_pBody;
    if (pBody)
        pBody->setCollisionShape(0);

    btConeShape* pShape;
    switch (axis)
    {
        case PrimitiveShape::AXIS_X:
            pShape = new btConeShapeX(radius, height);
            break;

        case PrimitiveShape::AXIS_Y:
            pShape = new btConeShape(radius, height);
            break;

        case PrimitiveShape::AXIS_Z:
            pShape = new btConeShapeZ(radius, height);
            break;
    }

    dynamic_cast<btCompoundShape*>(m_pCollisionShape)->addChildShape(
        btTransform(toBullet(orientation), toBullet(position)), pShape);

    if (pBody)
        pBody->setCollisionShape(this);
}

//-----------------------------------------------------------------------

void CompoundShape::addCylinder(const Math::Real& radius, const Math::Real& height,
                                PrimitiveShape::tAxis axis, const Math::Vector3& position,
                                const Math::Quaternion& orientation)
{
    // Assertions
    assert(radius > 0.0f);
    assert(height > 0.0f);
    assert(m_pCollisionShape);

    Body* pBody = m_pBody;
    if (pBody)
        pBody->setCollisionShape(0);

    btCylinderShape* pShape;
    switch (axis)
    {
        case PrimitiveShape::AXIS_X:
            pShape = new btCylinderShapeX(btVector3(height, radius, radius));
            break;

        case PrimitiveShape::AXIS_Y:
            pShape = new btCylinderShape(btVector3(radius, height, radius));
            break;

        case PrimitiveShape::AXIS_Z:
            pShape = new btCylinderShapeZ(btVector3(radius, radius, height));
            break;
    }

    dynamic_cast<btCompoundShape*>(m_pCollisionShape)->addChildShape(
        btTransform(toBullet(orientation), toBullet(position)), pShape);

    if (pBody)
        pBody->setCollisionShape(this);
}

//-----------------------------------------------------------------------

void CompoundShape::addSphere(const Math::Real& radius, const Math::Vector3& position,
                              const Math::Quaternion& orientation)
{
    // Assertions
    assert(radius > 0.0f);
    assert(m_pCollisionShape);

    Body* pBody = m_pBody;
    if (pBody)
        pBody->setCollisionShape(0);

    btSphereShape* pShape = new btSphereShape(radius);

    dynamic_cast<btCompoundShape*>(m_pCollisionShape)->addChildShape(
        btTransform(toBullet(orientation), toBullet(position)), pShape);

    if (pBody)
        pBody->setCollisionShape(this);
}

//-----------------------------------------------------------------------

void CompoundShape::removeChildShape(unsigned int childIndex) const
{
    // Assertions
    assert(childIndex < getNbChildShapes());
    assert(m_pCollisionShape);

    dynamic_cast<btCompoundShape*>(m_pCollisionShape)->removeChildShapeByIndex(childIndex);
}

//-----------------------------------------------------------------------

Math::Vector3 CompoundShape::getChildSize(unsigned int childIndex) const
{
    // Assertions
    assert(childIndex < getNbChildShapes());
    assert(m_pCollisionShape);
    
    btBoxShape* pShape = dynamic_cast<btBoxShape*>(
                            dynamic_cast<btCompoundShape*>(m_pCollisionShape)->getChildShape(childIndex));
    if (pShape)
        return fromBullet(pShape->getHalfExtentsWithMargin()) * 2.0f;
    
    return Vector3::ZERO;
}

//-----------------------------------------------------------------------

Math::Real CompoundShape::getChildRadius(unsigned int childIndex) const
{
    // Assertions
    assert(childIndex < getNbChildShapes());
    assert(m_pCollisionShape);

    btCollisionShape* pChild =
        dynamic_cast<btCompoundShape*>(m_pCollisionShape)->getChildShape(childIndex);
    
    switch (pChild->getShapeType())
    {
        case CAPSULE_SHAPE_PROXYTYPE:
            return dynamic_cast<btCapsuleShape*>(pChild)->getRadius();

        case CONE_SHAPE_PROXYTYPE:
            return dynamic_cast<btConeShape*>(pChild)->getRadius();

        case CYLINDER_SHAPE_PROXYTYPE:
            return dynamic_cast<btCylinderShape*>(pChild)->getRadius();

        case SPHERE_SHAPE_PROXYTYPE:
            return dynamic_cast<btSphereShape*>(pChild)->getRadius();
    }
    
    return 0.0f;
}

//-----------------------------------------------------------------------

Math::Real CompoundShape::getChildHeight(unsigned int childIndex) const
{
    // Assertions
    assert(childIndex < getNbChildShapes());
    assert(m_pCollisionShape);

    btCollisionShape* pChild =
        dynamic_cast<btCompoundShape*>(m_pCollisionShape)->getChildShape(childIndex);
    
    switch (pChild->getShapeType())
    {
        case CAPSULE_SHAPE_PROXYTYPE:
            return dynamic_cast<btCapsuleShape*>(pChild)->getHalfHeight();

        case CONE_SHAPE_PROXYTYPE:
            return dynamic_cast<btConeShape*>(pChild)->getHeight();

        case CYLINDER_SHAPE_PROXYTYPE:
        {
            btCylinderShape* pCylinder = dynamic_cast<btCylinderShape*>(pChild);
            switch (pCylinder->getUpAxis())
            {
                case 0:
                    return pCylinder->getHalfExtentsWithMargin().getX();

                case 1:
                    return pCylinder->getHalfExtentsWithMargin().getY();

                case 2:
                    return pCylinder->getHalfExtentsWithMargin().getZ();
            }
        }
    }
    
    return 0.0f;
}

//-----------------------------------------------------------------------

PrimitiveShape::tAxis CompoundShape::getChildAxis(unsigned int childIndex) const
{
    // Assertions
    assert(childIndex < getNbChildShapes());
    assert(m_pCollisionShape);

    int axis = 1;

    btCollisionShape* pChild =
        dynamic_cast<btCompoundShape*>(m_pCollisionShape)->getChildShape(childIndex);

    switch (pChild->getShapeType())
    {
        case CAPSULE_SHAPE_PROXYTYPE:
            axis = dynamic_cast<btCapsuleShape*>(pChild)->getUpAxis();
            break;

        case CONE_SHAPE_PROXYTYPE:
            axis = dynamic_cast<btConeShape*>(pChild)->getConeUpIndex();
            break;

        case CYLINDER_SHAPE_PROXYTYPE:
            axis = dynamic_cast<btCylinderShape*>(pChild)->getUpAxis();
            break;
    }

    switch (axis)
    {
        case 0: return PrimitiveShape::AXIS_X;
        case 1: return PrimitiveShape::AXIS_Y;
        case 2: return PrimitiveShape::AXIS_Z;
    }
    
    return PrimitiveShape::AXIS_Y;
}

//-----------------------------------------------------------------------

PrimitiveShape::tShape CompoundShape::getChildShape(unsigned int childIndex) const
{
    // Assertions
    assert(childIndex < getNbChildShapes());
    assert(m_pCollisionShape);

    btCollisionShape* pChild =
        dynamic_cast<btCompoundShape*>(m_pCollisionShape)->getChildShape(childIndex);
    
    switch (pChild->getShapeType())
    {
        case BOX_SHAPE_PROXYTYPE:       return PrimitiveShape::SHAPE_BOX;
        case CAPSULE_SHAPE_PROXYTYPE:   return PrimitiveShape::SHAPE_CAPSULE;
        case CONE_SHAPE_PROXYTYPE:      return PrimitiveShape::SHAPE_CONE;
        case CYLINDER_SHAPE_PROXYTYPE:  return PrimitiveShape::SHAPE_CYLINDER;
        case SPHERE_SHAPE_PROXYTYPE:    return PrimitiveShape::SHAPE_SPHERE;
    }
    
    return PrimitiveShape::SHAPE_BOX;
}

//-----------------------------------------------------------------------

void CompoundShape::getChildTransforms(unsigned int childIndex, Math::Vector3 &position,
                                       Math::Quaternion &orientation) const
{
    // Assertions
    assert(childIndex < getNbChildShapes());
    assert(m_pCollisionShape);

    btTransform transform =
        dynamic_cast<btCompoundShape*>(m_pCollisionShape)->getChildTransform(childIndex);
    
    position = fromBullet(transform.getOrigin());
    orientation = fromBullet(transform.getRotation());
}


/***************************** MANAGEMENT OF THE PROPERTIES ****************************/

Utils::PropertiesList* CompoundShape::getProperties() const
{
    // Assertions
    assert(m_pCollisionShape);

	// Call the base class implementation
	PropertiesList* pProperties = Component::getProperties();

	// Create the category belonging to this type
	pProperties->selectCategory(TYPE, false);

    for (unsigned int i = 0; i < getNbChildShapes(); ++i)
    {
        btCollisionShape* pChild =
            dynamic_cast<btCompoundShape*>(m_pCollisionShape)->getChildShape(i);
 
        Variant* pStruct = new Variant(Variant::STRUCT);
        
        switch (pChild->getShapeType())
        {
            case BOX_SHAPE_PROXYTYPE:
                pStruct->setField("type", new Variant("BOX"));
                pStruct->setField("size", new Variant(getChildSize(i)));
                break;

            case CAPSULE_SHAPE_PROXYTYPE:
                pStruct->setField("type", new Variant("CAPSULE"));
                pStruct->setField("radius", new Variant(getChildRadius(i)));
                pStruct->setField("height", new Variant(getChildHeight(i)));
                break;

            case CONE_SHAPE_PROXYTYPE:
                pStruct->setField("type", new Variant("CONE"));
                pStruct->setField("radius", new Variant(getChildRadius(i)));
                pStruct->setField("height", new Variant(getChildHeight(i)));
                break;

            case CYLINDER_SHAPE_PROXYTYPE:
                pStruct->setField("type", new Variant("CYLINDER"));
                pStruct->setField("radius", new Variant(getChildRadius(i)));
                pStruct->setField("height", new Variant(getChildHeight(i)));
                break;

            case SPHERE_SHAPE_PROXYTYPE:
                pStruct->setField("type", new Variant("SPHERE"));
                pStruct->setField("radius", new Variant(getChildRadius(i)));
                break;
        }

        if ((pChild->getShapeType() == CAPSULE_SHAPE_PROXYTYPE) ||
            (pChild->getShapeType() == CONE_SHAPE_PROXYTYPE) ||
            (pChild->getShapeType() == CYLINDER_SHAPE_PROXYTYPE))
        {
            switch (getChildAxis(i))
            {
                case PrimitiveShape::AXIS_X:
                    pStruct->setField("axis", new Variant("X"));
                    break;

                case PrimitiveShape::AXIS_Y:
                    pStruct->setField("axis", new Variant("Y"));
                    break;

                case PrimitiveShape::AXIS_Z:
                    pStruct->setField("axis", new Variant("Z"));
                    break;
            }
        }
        
        Vector3 position;
        Quaternion orientation;
        getChildTransforms(i, position, orientation);

        pStruct->setField("position", new Variant(position));
        pStruct->setField("orientation", new Variant(orientation));
        
        pProperties->set("shape_" + StringConverter::toString(i), pStruct);
    }

	// Returns the list
	return pProperties;
}

//-----------------------------------------------------------------------

bool CompoundShape::setProperty(const std::string& strCategory, const std::string& strName,
								Utils::Variant* pValue)
{
	assert(!strCategory.empty());
	assert(!strName.empty());
	assert(pValue);

	if (strCategory == TYPE)
		return CompoundShape::setProperty(strName, pValue);

	return Component::setProperty(strCategory, strName, pValue);
}

//-----------------------------------------------------------------------

bool CompoundShape::setProperty(const std::string& strName, Utils::Variant* pValue)
{
	// Assertions
	assert(!strName.empty());
	assert(pValue);

    // Shape
	if (StringUtils::startsWith(strName, "shape_"))
	{
        string strType;
        Vector3 size(1.0f, 1.0f, 1.0f);
        float radius = 1.0f;
        float height = 1.0f;
        PrimitiveShape::tAxis axis = PrimitiveShape::AXIS_Y;
        Vector3 position(Vector3::ZERO);
        Quaternion orientation(Quaternion::IDENTITY);
	    
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
                axis = PrimitiveShape::AXIS_X;
            else if (pField->toString() == "Z")
                axis = PrimitiveShape::AXIS_Z;
        }

        pField = pValue->getField("position");
        if (pField)
            position = pField->toVector3();

        pField = pValue->getField("orientation");
        if (pField)
            orientation = pField->toQuaternion();
        
        if (strType == "BOX")
            addBox(size, position, orientation);
        else if (strType == "CAPSULE")
            addCapsule(radius, height, axis, position, orientation);
        else if (strType == "CONE")
            addCone(radius, height, axis, position, orientation);
        else if (strType == "CYLINDER")
            addCylinder(radius, height, axis, position, orientation);
        else if (strType == "SPHERE")
            addSphere(radius, position, orientation);
    }

	// Destroy the value
	delete pValue;

	return true;
}
