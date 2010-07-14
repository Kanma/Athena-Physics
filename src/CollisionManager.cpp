/**	@file	CollisionManager.cpp
	@author	Philip Abbet

	Implementation of the class 'Athena::Physics::CollisionManager'
*/

#include <Athena-Physics/CollisionManager.h>
#include <Athena-Physics/Body.h>
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
    Body* pBody1 = dynamic_cast<Body*>(static_cast<btRigidBody*>(pProxy1->m_clientObject)->getMotionState());
    Body* pBody2 = dynamic_cast<Body*>(static_cast<btRigidBody*>(pProxy2->m_clientObject)->getMotionState());
    
    if ((pBody1->getCollisionGroup() == -1) || (pBody2->getCollisionGroup() == -1))
        return true;

    tPairState state;
    
    if (pBody1->getCollisionGroup() <= pBody2->getCollisionGroup())
        state = m_indexedPairs[pBody1->getCollisionGroup()][pBody2->getCollisionGroup()];
    else
        state = m_indexedPairs[pBody2->getCollisionGroup()][pBody1->getCollisionGroup()];

    return (state != PAIR_DISABLED);
}


/************************************* METHODS ****************************************/

void CollisionManager::enableCollision(tCollisionGroup group1, tCollisionGroup group2,
                                       bool bEnableFilter)
{
    tPairState* pState;
    
    if (group1 <= group2)
        pState = &m_indexedPairs[group1][group2];
    else
        pState = &m_indexedPairs[group2][group1];
    
    *pState = (bEnableFilter ? PAIR_ENABLED_WITH_FILTER : PAIR_ENABLED);
}


/********************************* STATIC METHODS **************************************/

void CollisionManager::customNearCallback(btBroadphasePair& collisionPair,
                                          btCollisionDispatcher& dispatcher,
                                          const btDispatcherInfo& dispatchInfo)
{
    if (CollisionManager::_CurrentManager)
    {
        Body* pBody1 = dynamic_cast<Body*>(static_cast<btRigidBody*>(collisionPair.m_pProxy0->m_clientObject)->getMotionState());
        Body* pBody2 = dynamic_cast<Body*>(static_cast<btRigidBody*>(collisionPair.m_pProxy1->m_clientObject)->getMotionState());

        if ((pBody1->getCollisionGroup() == -1) || (pBody2->getCollisionGroup() == -1))
        {
            dispatcher.defaultNearCallback(collisionPair, dispatcher, dispatchInfo);
            return;
        }

        tPairState state;

        if (pBody1->getCollisionGroup() <= pBody2->getCollisionGroup())
            state = CollisionManager::_CurrentManager->m_indexedPairs[pBody1->getCollisionGroup()][pBody2->getCollisionGroup()];
        else
            state = CollisionManager::_CurrentManager->m_indexedPairs[pBody2->getCollisionGroup()][pBody1->getCollisionGroup()];

        assert(state != PAIR_DISABLED);

        bool bContinue = (state == PAIR_ENABLED) || !CollisionManager::_CurrentManager->m_pFilter ||
                         CollisionManager::_CurrentManager->m_pFilter->needsCollision(pBody1, pBody2);

        if (bContinue)
            dispatcher.defaultNearCallback(collisionPair, dispatcher, dispatchInfo);
    }
}
