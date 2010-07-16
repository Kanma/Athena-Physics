/**	@file	CollisionManager.cpp
	@author	Philip Abbet

	Implementation of the class 'Athena::Physics::CollisionManager'
*/

#include <Athena-Physics/CollisionManager.h>
#include <Athena-Physics/Body.h>
#include <Athena-Physics/GhostObject.h>
#include <Athena-Physics/Conversions.h>

using namespace Athena;
using namespace Athena::Physics;
using namespace Athena::Entities;
using namespace Athena::Signals;
using namespace std;


/********************************** STATIC ATTRIBUTES **********************************/

CollisionManager CollisionManager::DefaultManager;
CollisionManager* CollisionManager::_CurrentManager = 0;


/***************************** CONSTRUCTION / DESTRUCTION ******************************/

CollisionManager::CollisionManager()
: m_pFilter(0)
{
    unsigned int offset = 0;
    for (unsigned int i = 0; i < NB_GROUPS; ++i)
    {
        m_indexedPairs[i] = &m_collisionPairs[offset];
        offset += NB_GROUPS - i;
    }
}

//-----------------------------------------------------------------------

CollisionManager::~CollisionManager()
{
}


/*********************** IMPLEMENTATION OF btOverlapFilterCallback *********************/

bool CollisionManager::needBroadphaseCollision(btBroadphaseProxy* pProxy1,
                                               btBroadphaseProxy* pProxy2) const
{
    tCollisionGroup group1 = getGroupOfCollisionObject((btCollisionObject*) pProxy1->m_clientObject);
    tCollisionGroup group2 = getGroupOfCollisionObject((btCollisionObject*) pProxy2->m_clientObject);
    
    if ((group1 == -1) || (group2 == -1))
        return true;

    tPairState state;
    
    if (group1 <= group2)
        state = m_indexedPairs[1 << group1][1 << group2];
    else
        state = m_indexedPairs[1 << group2][1 << group1];

    return (state != PAIR_DISABLED);
}


/************************************* METHODS ****************************************/

void CollisionManager::enableCollision(tCollisionGroup group1, tCollisionGroup group2,
                                       bool bEnableFilter)
{
    tPairState* pState;
    
    if (group1 <= group2)
        pState = &m_indexedPairs[1 << group1][1 << group2];
    else
        pState = &m_indexedPairs[1 << group2][1 << group1];
    
    *pState = (bEnableFilter ? PAIR_ENABLED_WITH_FILTER : PAIR_ENABLED);
}


/********************************* STATIC METHODS **************************************/

void CollisionManager::customNearCallback(btBroadphasePair& collisionPair,
                                          btCollisionDispatcher& dispatcher,
                                          const btDispatcherInfo& dispatchInfo)
{
    if (CollisionManager::_CurrentManager)
    {
        tCollisionGroup group1, group2;
        
        PhysicalComponent* pComponent1 = getComponentOfCollisionObject((btCollisionObject*) collisionPair.m_pProxy0->m_clientObject, group1);
        PhysicalComponent* pComponent2 = getComponentOfCollisionObject((btCollisionObject*) collisionPair.m_pProxy1->m_clientObject, group2);

        if ((group1 == -1) || (group2 == -1))
        {
            dispatcher.defaultNearCallback(collisionPair, dispatcher, dispatchInfo);
            return;
        }

        tPairState state;

        if (group1 <= group2)
            state = CollisionManager::_CurrentManager->m_indexedPairs[1 << group1][1 << group2];
        else
            state = CollisionManager::_CurrentManager->m_indexedPairs[1 << group2][1 << group1];

        assert(state != PAIR_DISABLED);

        bool bContinue = (state == PAIR_ENABLED) || !CollisionManager::_CurrentManager->m_pFilter ||
                         CollisionManager::_CurrentManager->m_pFilter->needsCollision(pComponent1, pComponent2);

        if (bContinue)
            dispatcher.defaultNearCallback(collisionPair, dispatcher, dispatchInfo);
    }
}

//-----------------------------------------------------------------------

tCollisionGroup CollisionManager::getGroupOfCollisionObject(btCollisionObject* pObject)
{
    btRigidBody* pRigidBody = btRigidBody::upcast(pObject);
    if (pRigidBody)
        return static_cast<Body*>(pRigidBody->getUserPointer())->getCollisionGroup();

    btGhostObject* pGhostObject = btGhostObject::upcast(pObject);
    if (pGhostObject)
        return static_cast<GhostObject*>(pGhostObject->getUserPointer())->getCollisionGroup();

    return -1;
}

//-----------------------------------------------------------------------

PhysicalComponent* CollisionManager::getComponentOfCollisionObject(btCollisionObject* pObject,
                                                                   tCollisionGroup &group)
{
    btRigidBody* pRigidBody = btRigidBody::upcast(pObject);
    if (pRigidBody)
    {
        Body* pBody = static_cast<Body*>(pRigidBody->getUserPointer());
        group = pBody->getCollisionGroup();
        return pBody;
    }

    btGhostObject* pGhostObject = btGhostObject::upcast(pObject);
    if (pGhostObject)
    {
        GhostObject* pGhost = static_cast<GhostObject*>(pGhostObject->getUserPointer());
        group = pGhost->getCollisionGroup();
        return pGhost;
    }

    group = -1;
    return 0;
}
