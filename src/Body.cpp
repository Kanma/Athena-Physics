/**	@file	Body.cpp
	@author	Philip Abbet

	Implementation of the class 'Athena::Physics::Body'
*/

#include <Athena-Physics/Body.h>
#include <Athena-Physics/World.h>
#include <Athena-Physics/CollisionShape.h>
#include <Athena-Physics/Conversions.h>
#include <Athena-Entities/Transforms.h>
#include <Athena-Entities/Signals.h>
#include <Athena-Math/MathUtils.h>
#include <Athena-Core/Signals/SignalsList.h>

using namespace Athena;
using namespace Athena::Physics;
using namespace Athena::Entities;
using namespace Athena::Utils;
using namespace Athena::Math;
using namespace Athena::Signals;
using namespace std;


/************************************** CONSTANTS **************************************/

///< Name of the type of component
const std::string Body::TYPE = "Athena/Physics/Body";


/***************************** CONSTRUCTION / DESTRUCTION ******************************/

Body::Body(const std::string& strName, ComponentsList* pList)
: PhysicalComponent(strName, pList), m_pBody(0), m_mass(0.0f), m_pShape(0)
{
    btRigidBody::btRigidBodyConstructionInfo info(0.0f, this, 0);
    m_pBody = new btRigidBody(info);
}

//-----------------------------------------------------------------------

Body::~Body()
{
    if (m_pShape)
        getWorld()->removeRigidBody(this);

    delete m_pBody;
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
    Transforms* pTransforms = getTransforms();
    if (pTransforms)
    {
        worldTrans = btTransform(toBullet(pTransforms->getWorldOrientation()),
                                 toBullet(pTransforms->getWorldPosition()));
    }
}

//-----------------------------------------------------------------------

void Body::setWorldTransform(const btTransform& worldTrans)
{
    Transforms* pTransforms = getTransforms();
    if (pTransforms)
    {
        pTransforms->translate(fromBullet(worldTrans.getOrigin()) - pTransforms->getWorldPosition(), Transforms::TS_WORLD);
        pTransforms->rotate(pTransforms->getWorldOrientation().rotationTo(fromBullet(worldTrans.getRotation())), Transforms::TS_WORLD);
    }
}


/*************************************** METHODS ***************************************/

void Body::setKinematic(bool bKinematic)
{
    assert(m_pBody);

    if (bKinematic == isKinematic())
        return;

    if (bKinematic)
    {
		m_pBody->setCollisionFlags(m_pBody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
	    m_pBody->setActivationState(DISABLE_DEACTIVATION);
	}
	else
	{
		m_pBody->setCollisionFlags(m_pBody->getCollisionFlags() & ~btCollisionObject::CF_KINEMATIC_OBJECT);
	    m_pBody->setActivationState(WANTS_DEACTIVATION);
    }

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

void Body::setCollisionShape(CollisionShape* pShape)
{
    assert(!pShape || (pShape->getTransforms() == getTransforms()));
    
    if (pShape == m_pShape)
        return;
        
	// Unregister to the signals of the previous shape
	if (m_pShape)
	{
		SignalsList* pSignals = m_pShape->getSignalsList();
		pSignals->disconnect(SIGNAL_COMPONENT_DESTROYED, this, &Body::onCollisionShapeDestroyed);
        
        m_pShape->setBody(0);
    }
    
    m_pShape = pShape;
    
    // Register to the signals of the new origin
	if (m_pShape)
	{
		SignalsList* pSignals = m_pShape->getSignalsList();
		pSignals->connect(SIGNAL_COMPONENT_DESTROYED, this, &Body::onCollisionShapeDestroyed);

        m_pShape->setBody(this);
	}

    updateBody();
}

//-----------------------------------------------------------------------

void Body::updateBody()
{
    assert(m_pBody);

    if (m_pBody->getCollisionShape())
        getWorld()->removeRigidBody(this);

    if (m_pShape)
        m_pBody->setCollisionShape(m_pShape->getCollisionShape());
    else
        m_pBody->setCollisionShape(0);

    if (!m_pShape || isKinematic() || MathUtils::RealEqual(m_mass, 0.0f, 1e-6f))
    {
        m_pBody->setMassProps(0.0f, btVector3(0.0f, 0.0f, 0.0f));
    }
    else
    {
        btVector3 inertia;
        m_pShape->getCollisionShape()->calculateLocalInertia(m_mass, inertia);
        m_pBody->setMassProps(m_mass, inertia);
    }

    if (m_pShape)
        getWorld()->addRigidBody(this);
}

//-----------------------------------------------------------------------

void Body::onTransformsChanged()
{
    PhysicalComponent::onTransformsChanged();

    // If we don't do that, the position of the body isn't changed
    if (isStatic())
        m_pBody->setMotionState(m_pBody->getMotionState());
}


/**************************************** SLOTS ****************************************/

void Body::onCollisionShapeDestroyed(Utils::Variant* pValue)
{
    setCollisionShape(0);
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

    // Shape
    if (m_pShape)
        pProperties->set("shape", new Variant(m_pShape->getID().toString()));

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

    // Declarations
    bool bUsed = true;

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

	// Shape
	else if (strName == "shape")
	{
        tComponentID id(pValue->toString());

        if (id.type == COMP_PHYSICAL)
        {
            CollisionShape* pShape = CollisionShape::cast(m_pList->getComponent(id));
            if (pShape)
                setCollisionShape(pShape);
            else
                bUsed = false;
        }
        else
        {
            setCollisionShape(0);
        }
	}

	// Destroy the value
	delete pValue;

	return bUsed;
}
