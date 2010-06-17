/**	@file	Body.cpp
	@author	Philip Abbet

	Implementation of the class 'Athena::Physics::Body'
*/

#include <Athena-Physics/Body.h>
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
const std::string Body::TYPE = "Athena/Physics/Body";


/***************************** CONSTRUCTION / DESTRUCTION ******************************/

Body::Body(const std::string& strName, ComponentsList* pList)
: PhysicalComponent(strName, pList), m_pBody(0), m_mass(0.0f)
{
    btRigidBody::btRigidBodyConstructionInfo info(0.0f, this, 0);
    m_pBody = new btRigidBody(info);
    getWorld()->addRigidBody(this);
}

//-----------------------------------------------------------------------

Body::~Body()
{
    if (m_pBody)
    {
        getWorld()->removeRigidBody(this);
        delete m_pBody;
    }
}

//-----------------------------------------------------------------------

Body* Body::create(const std::string& strName, ComponentsList* pList)
{
	return new Body(strName, pList);
}

//-----------------------------------------------------------------------

Body* Body::cast(Component* pComponent)
{
	return dynamic_cast<Body*>(pComponent);
}


/**************************** IMPLEMENTATION OF btMotionState **************************/

void Body::getWorldTransform(btTransform& worldTrans) const
{
    if (m_pTransformsOrigin)
    {
        worldTrans = btTransform(toBullet(m_pTransformsOrigin->getWorldOrientation()),
                                 toBullet(m_pTransformsOrigin->getWorldPosition()));
    }
}

//-----------------------------------------------------------------------

void Body::setWorldTransform(const btTransform& worldTrans)
{
    if (m_pTransformsOrigin)
    {
        m_pTransformsOrigin->translate(fromBullet(worldTrans.getOrigin()) - m_pTransformsOrigin->getWorldPosition(), Transforms::TS_WORLD);
        m_pTransformsOrigin->rotate(fromBullet(worldTrans.getRotation()).Inverse() * m_pTransformsOrigin->getWorldOrientation(), Transforms::TS_WORLD);
    }
}


/*************************************** METHODS ***************************************/

void Body::setKinematic(bool bKinematic)
{
    assert(m_pBody);

    if (bKinematic == isKinematic())
        return;

    if (bKinematic)
		m_pBody->setCollisionFlags(m_pBody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
	else
		m_pBody->setCollisionFlags(m_pBody->getCollisionFlags() & ~btCollisionObject::CF_KINEMATIC_OBJECT);

    updateBody();
}


//-----------------------------------------------------------------------

void Body::setMass(Math::Real mass)
{
    assert(m_pBody);

    m_mass = mass;
    
    if (isKinematic())
        return;

    updateBody();
}

//-----------------------------------------------------------------------

void Body::updateBody()
{
    assert(m_pBody);

    getWorld()->removeRigidBody(this);

//    if (!getCollisionShape() || isKinematic() || MathUtils::RealEqual(m_mass, 0.0f, 1e-6f))
    if (isKinematic() || MathUtils::RealEqual(m_mass, 0.0f, 1e-6f))
    {
        m_pBody->setMassProps(0.0f, btVector3(0.0f, 0.0f, 0.0f));
    }
    else
    {
        btVector3 inertia;
//        getCollisionShape()->calculateLocalInertia(m_mass, inertia);
        m_pBody->setMassProps(m_mass, inertia);
    }

    getWorld()->addRigidBody(this);
}


/***************************** MANAGEMENT OF THE PROPERTIES ****************************/

Utils::PropertiesList* Body::getProperties() const
{
	// Call the base class implementation
	PropertiesList* pProperties = Component::getProperties();

	// Create the category belonging to this type
	pProperties->selectCategory(TYPE, false);

	// Type
	if (isKinematic())
	    pProperties->set("type", new Variant("KINEMATIC"));
	else if (isStatic())
	    pProperties->set("type", new Variant("STATIC"));
	else
	    pProperties->set("type", new Variant("DYNAMIC"));

    // Mass
    pProperties->set("mass", new Variant(m_mass));

	// Returns the list
	return pProperties;
}

//-----------------------------------------------------------------------

bool Body::setProperty(const std::string& strCategory, const std::string& strName,
								Utils::Variant* pValue)
{
	assert(!strCategory.empty());
	assert(!strName.empty());
	assert(pValue);

	if (strCategory == TYPE)
		return Body::setProperty(strName, pValue);

	return Component::setProperty(strCategory, strName, pValue);
}

//-----------------------------------------------------------------------

bool Body::setProperty(const std::string& strName, Utils::Variant* pValue)
{
	// Assertions
	assert(!strName.empty());
	assert(pValue);

    // Type
	if (strName == "type")
	{
	    setKinematic(pValue->toString() == "KINEMATIC");
    }

	// Mass
	else if (strName == "mass")
	{
		setMass(pValue->toFloat());
	}

	// Destroy the value
	delete pValue;

	return true;
}
