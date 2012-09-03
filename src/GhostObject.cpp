/** @file   GhostObject.cpp
    @author Philip Abbet

    Implementation of the class 'Athena::Physics::GhostObject'
*/

#include <Athena-Physics/GhostObject.h>
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
const std::string GhostObject::TYPE = "Athena/Physics/GhostObject";


/***************************** CONSTRUCTION / DESTRUCTION ******************************/

GhostObject::GhostObject(const std::string& strName, ComponentsList* pList)
: CollisionObject(strName, pList), m_pGhostObject(0), m_pShape(0)
{
    m_pGhostObject = new btGhostObject();
    m_pGhostObject->setCollisionFlags(m_pGhostObject->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
    m_pGhostObject->setUserPointer(this);

    // Use the transforms of the entity by default
    setTransforms(0);
}

//-----------------------------------------------------------------------

GhostObject::~GhostObject()
{
    if (m_pShape)
        getWorld()->removeGhostObject(this);

    delete m_pGhostObject;
}

//-----------------------------------------------------------------------

GhostObject* GhostObject::create(const std::string& strName, ComponentsList* pList)
{
    return new GhostObject(strName, pList);
}

//-----------------------------------------------------------------------

GhostObject* GhostObject::cast(Component* pComponent)
{
    return dynamic_cast<GhostObject*>(pComponent);
}


/*************************************** METHODS ***************************************/

void GhostObject::setCollisionShape(CollisionShape* pShape)
{
    assert(m_pGhostObject);

    if (pShape == m_pShape)
        return;

    if (m_pGhostObject->getCollisionShape())
        getWorld()->removeGhostObject(this);

    // Unlink from the current shape
    if (m_pShape)
    {
        removeLinkTo(m_pShape);
        m_pShape = 0;
    }

    m_pShape = pShape;

    if (m_pShape)
    {
        // Link with the new shape
        addLinkTo(m_pShape);

        m_pGhostObject->setCollisionShape(m_pShape->getCollisionShape());
        getWorld()->addGhostObject(this);
    }
    else
    {
        m_pGhostObject->setCollisionShape(0);
    }
}

//-----------------------------------------------------------------------

PhysicalComponent* GhostObject::getOverlappingObject(unsigned int index)
{
    assert(m_pGhostObject);

    btCollisionObject* pObject = m_pGhostObject->getOverlappingObject(index);
    if (!pObject)
        return 0;

    return static_cast<PhysicalComponent*>(pObject->getUserPointer());
}

//-----------------------------------------------------------------------

void GhostObject::onTransformsChanged()
{
    PhysicalComponent::onTransformsChanged();

    Transforms* pTransforms = getTransforms();
    if (pTransforms)
    {
        m_pGhostObject->setWorldTransform(btTransform(toBullet(pTransforms->getWorldOrientation()),
                                                      toBullet(pTransforms->getWorldPosition())));
    }
    else
    {
        m_pGhostObject->setWorldTransform(btTransform(toBullet(Quaternion::IDENTITY),
                                                      toBullet(Vector3::ZERO)));
    }
}


/*********************************** LINKS MANAGEMENT **********************************/

void GhostObject::mustUnlinkComponent(Component* pComponent)
{
    // Assertions
    assert(pComponent);

    bool bMustUpdate = false;

    if (m_pShape == pComponent)
    {
        m_pShape = 0;
        bMustUpdate = true;
    }

    CollisionObject::mustUnlinkComponent(pComponent);

    if (bMustUpdate)
        setCollisionShape(0);
}


/***************************** MANAGEMENT OF THE PROPERTIES ****************************/

Utils::PropertiesList* GhostObject::getProperties() const
{
    // Call the base class implementation
    PropertiesList* pProperties = CollisionObject::getProperties();

    // Create the category belonging to this type
    pProperties->selectCategory(TYPE, false);

    // Shape
    if (m_pShape)
        pProperties->set("shape", new Variant(m_pShape->getID().toString()));

    // Returns the list
    return pProperties;
}

//-----------------------------------------------------------------------

bool GhostObject::setProperty(const std::string& strCategory, const std::string& strName,
                                Utils::Variant* pValue)
{
    assert(!strCategory.empty());
    assert(!strName.empty());
    assert(pValue);

    if (strCategory == TYPE)
        return GhostObject::setProperty(strName, pValue);

    return CollisionObject::setProperty(strCategory, strName, pValue);
}

//-----------------------------------------------------------------------

bool GhostObject::setProperty(const std::string& strName, Utils::Variant* pValue)
{
    // Assertions
    assert(!strName.empty());
    assert(pValue);

    // Declarations
    bool bUsed = true;

    // Shape
    if (strName == "shape")
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
