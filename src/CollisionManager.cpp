/** @file   CollisionManager.cpp
    @author Philip Abbet

    Implementation of the class 'Athena::Physics::CollisionManager'
*/

#include <Athena-Physics/CollisionManager.h>
#include <Athena-Physics/Body.h>
#include <Athena-Physics/CollisionObject.h>
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

    if ((group1 == 255) || (group2 == 255))
        return true;

    tPairState state;

    if (group1 <= group2)
        state = m_indexedPairs[group1][group2];
    else
        state = m_indexedPairs[group2][group1];

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
        tCollisionGroup group1, group2;

        CollisionObject* pComponent1 = getComponentOfCollisionObject((btCollisionObject*) collisionPair.m_pProxy0->m_clientObject, group1);
        CollisionObject* pComponent2 = getComponentOfCollisionObject((btCollisionObject*) collisionPair.m_pProxy1->m_clientObject, group2);

        if ((group1 == 255) || (group2 == 255))
        {
            dispatcher.defaultNearCallback(collisionPair, dispatcher, dispatchInfo);
            return;
        }

        tPairState state;

        if (group1 <= group2)
            state = CollisionManager::_CurrentManager->m_indexedPairs[group1][group2];
        else
            state = CollisionManager::_CurrentManager->m_indexedPairs[group2][group1];

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
    return static_cast<CollisionObject*>(pObject->getUserPointer())->getCollisionGroup();
}

//-----------------------------------------------------------------------

CollisionObject* CollisionManager::getComponentOfCollisionObject(btCollisionObject* pObject,
                                                                 tCollisionGroup &group)
{
    CollisionObject* pCollisionObject = static_cast<CollisionObject*>(pObject->getUserPointer());
    group = pCollisionObject->getCollisionGroup();
    return pCollisionObject;
}
