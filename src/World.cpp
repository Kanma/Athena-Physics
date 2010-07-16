/**	@file	World.cpp
	@author	Philip Abbet

	Implementation of the class 'Athena::Physics::World'
*/

#include <Athena-Physics/World.h>
#include <Athena-Physics/Body.h>
#include <Athena-Physics/GhostObject.h>
#include <Athena-Physics/Conversions.h>
#include <Athena-Physics/CollisionManager.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>

using namespace Athena;
using namespace Athena::Physics;
using namespace Athena::Entities;
using namespace Athena::Utils;
using namespace std;


/************************************** CONSTANTS **************************************/

///< Name of the type of component
const std::string World::TYPE           = "Athena/Physics/World";
const std::string World::DEFAULT_NAME   = "PhysicalWorld";


/***************************** CONSTRUCTION / DESTRUCTION ******************************/

World::World(ComponentsList* pList)
: PhysicalComponent(DEFAULT_NAME, pList), m_type(WORLD_RIGID_BODY), m_pWorld(0),
  m_pDispatcher(0), m_pBroadphase(0), m_pConstraintSolver(0), m_pCollisionConfiguration(0),
  m_pCollisionManager(&CollisionManager::DefaultManager)
{
    assert(pList);
    assert(pList->getScene());
    assert(!pList->getEntity());

    pList->getScene()->_setMainComponent(this);
}

//-----------------------------------------------------------------------

World::~World()
{
    delete m_pWorld;
    delete m_pConstraintSolver;
    delete m_pBroadphase;
    delete m_pDispatcher;
    delete m_pCollisionConfiguration;

    m_pList->getScene()->_resetMainComponent(COMP_PHYSICAL);
}

//-----------------------------------------------------------------------

World* World::create(const std::string& strName, ComponentsList* pList)
{
	return new World(pList);
}

//-----------------------------------------------------------------------

World* World::cast(Component* pComponent)
{
	return dynamic_cast<World*>(pComponent);
}


/**************************************** METHODS **************************************/

void World::setWorldType(tType type)
{
    assert(!m_pWorld);

    m_type = type;
    createWorld();
}


//-----------------------------------------------------------------------

void World::setGravity(const Math::Vector3& gravity)
{
    if (!m_pWorld)
        createWorld();

    m_pWorld->setGravity(toBullet(gravity));
}

//-----------------------------------------------------------------------

Math::Vector3 World::getGravity()
{
    if (!m_pWorld)
        createWorld();

    return fromBullet(m_pWorld->getGravity());
}

//-----------------------------------------------------------------------

unsigned int World::stepSimulation(Math::Real timeStep, unsigned int nbMaxSubSteps,
    	                           Math::Real fixedTimeStep)
{
    if (!m_pWorld)
        createWorld();

    CollisionManager::_CurrentManager = m_pCollisionManager;

    bool bResult = m_pWorld->stepSimulation(timeStep, nbMaxSubSteps, fixedTimeStep);

    CollisionManager::_CurrentManager = 0;

    return bResult;
}

//-----------------------------------------------------------------------

bool World::getContacts(PhysicalComponent* pComponent1, PhysicalComponent* pComponent2,
                        tContactPointsList &contactPoints)
{
    assert(pComponent1);
    assert(pComponent2);
    
    contactPoints.clear();

    btCollisionObject* pObject1;
    btCollisionObject* pObject2;
    
    if (Body::cast(pComponent1))
        pObject1 = Body::cast(pComponent1)->getRigidBody();
    else if (GhostObject::cast(pComponent1))
        pObject1 = GhostObject::cast(pComponent1)->getGhostObject();
    else
        return false;

    if (Body::cast(pComponent2))
        pObject2 = Body::cast(pComponent2)->getRigidBody();
    else if (GhostObject::cast(pComponent2))
        pObject2 = GhostObject::cast(pComponent2)->getGhostObject();
    else
        return false;

    btBroadphasePair* pPairs = m_pWorld->getPairCache()->getOverlappingPairArrayPtr();
    unsigned int nbPairs = m_pWorld->getPairCache()->getNumOverlappingPairs();
    btBroadphasePair* pPair = pPairs;
    unsigned int i;
    for (i = 0; i < nbPairs; ++i)
    {
        if (((pPair->m_pProxy0->m_clientObject == pObject1) && (pPair->m_pProxy1->m_clientObject == pObject2)) ||
            ((pPair->m_pProxy0->m_clientObject == pObject2) && (pPair->m_pProxy1->m_clientObject == pObject1)))
            break;
        
        ++pPair;
    }
    
    if (i == nbPairs)
        return false;

    if (!pPair->m_algorithm)
        return false;
    
    btManifoldArray manifolds;
    pPair->m_algorithm->getAllContactManifolds(manifolds);

	for (int j = 0; j < manifolds.size(); ++j)
    {
        btPersistentManifold* pManifold = manifolds[j];
        for (int p = 0; p < pManifold->getNumContacts(); ++p)
            contactPoints.push_back(pManifold->getContactPoint(p));
    }
    
    return (pPair->m_pProxy0->m_clientObject == pObject1);
}

//-----------------------------------------------------------------------

void World::createWorld()
{
    assert(!m_pWorld);

	// Collision configuration contains default setup for memory, collision setup
	m_pCollisionConfiguration = new btDefaultCollisionConfiguration();

	// Use the default collision dispatcher
	m_pDispatcher = new	btCollisionDispatcher(m_pCollisionConfiguration);
    dynamic_cast<btCollisionDispatcher*>(m_pDispatcher)->setNearCallback(&CollisionManager::customNearCallback);

	m_pBroadphase = new btDbvtBroadphase();
    m_pBroadphase->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());

	// The default constraint solver
	m_pConstraintSolver = new btSequentialImpulseConstraintSolver();

    switch (m_type)
    {
        case WORLD_RIGID_BODY:
            m_pWorld = new btDiscreteDynamicsWorld(m_pDispatcher, m_pBroadphase, m_pConstraintSolver, m_pCollisionConfiguration);
            break;

        case WORLD_SOFT_BODY:
            m_pWorld = new btSoftRigidDynamicsWorld(m_pDispatcher, m_pBroadphase, m_pConstraintSolver, m_pCollisionConfiguration);
            break;
    }
    
    m_pWorld->getPairCache()->setOverlapFilterCallback(m_pCollisionManager);
}

//-----------------------------------------------------------------------

void World::addRigidBody(Body* pBody)
{
    // Assertions
    assert(pBody);
    
    if (!m_pWorld)
        createWorld();

    m_pWorld->addRigidBody(pBody->getRigidBody());
}

//-----------------------------------------------------------------------

void World::removeRigidBody(Body* pBody)
{
    // Assertions
    assert(pBody);
    assert(m_pWorld);
    
    m_pWorld->removeRigidBody(pBody->getRigidBody());
}

//-----------------------------------------------------------------------

void World::addGhostObject(GhostObject* pGhostObject)
{
    // Assertions
    assert(pGhostObject);
    
    if (!m_pWorld)
        createWorld();

    m_pWorld->addCollisionObject(pGhostObject->getGhostObject());
}

//-----------------------------------------------------------------------

void World::removeGhostObject(GhostObject* pGhostObject)
{
    // Assertions
    assert(pGhostObject);
    assert(m_pWorld);
    
    m_pWorld->removeCollisionObject(pGhostObject->getGhostObject());
}


/***************************** MANAGEMENT OF THE PROPERTIES ****************************/

Utils::PropertiesList* World::getProperties() const
{
	// Call the base class implementation
	PropertiesList* pProperties = Component::getProperties();

	// Create the category belonging to this type
	pProperties->selectCategory(TYPE, false);

	// Type
	switch (m_type)
    {
        case WORLD_RIGID_BODY:
    	    pProperties->set("type", new Variant("RIGID_BODY"));
            break;

        case WORLD_SOFT_BODY:
	        pProperties->set("type", new Variant("SOFT_BODY"));
            break;
    } 

    // Gravity
    if (m_pWorld)
	    pProperties->set("gravity", new Variant(fromBullet(m_pWorld->getGravity())));

	// Returns the list
	return pProperties;
}

//-----------------------------------------------------------------------

bool World::setProperty(const std::string& strCategory, const std::string& strName,
								Utils::Variant* pValue)
{
	assert(!strCategory.empty());
	assert(!strName.empty());
	assert(pValue);

	if (strCategory == TYPE)
		return World::setProperty(strName, pValue);

	return Component::setProperty(strCategory, strName, pValue);
}

//-----------------------------------------------------------------------

bool World::setProperty(const std::string& strName, Utils::Variant* pValue)
{
	// Assertions
	assert(!strName.empty());
	assert(pValue);

	if (strName == "type")
	{
	    if (pValue->toString() == "RIGID_BODY")
		    setWorldType(WORLD_RIGID_BODY);
	    else if (pValue->toString() == "SOFT_BODY")
		    setWorldType(WORLD_SOFT_BODY);
	}

	// Gravity
	else if (strName == "gravity")
	{
		setGravity(pValue->toVector3());
	}

	// Destroy the value
	delete pValue;

	return true;
}
