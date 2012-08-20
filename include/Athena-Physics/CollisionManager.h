/** @file   CollisionManager.h
    @author Philip Abbet

    Declaration of the class 'Athena::Physics::CollisionManager'
*/

#ifndef _ATHENA_PHYSICS_COLLISIONMANAGER_H_
#define _ATHENA_PHYSICS_COLLISIONMANAGER_H_

#include <Athena-Physics/Prerequisites.h>
#include <Athena-Physics/PhysicalComponent.h>
#include <BulletCollision/BroadphaseCollision/btOverlappingPairCache.h>
#include <BulletCollision/BroadphaseCollision/btDispatcher.h>
#include <BulletCollision/CollisionDispatch/btCollisionDispatcher.h>

namespace Athena {
namespace Physics {


//---------------------------------------------------------------------------------------
/// @brief  The object in charge of allowing or not the collisions between the different
///         'collision groups'
///
/// Each collision object is assigned to a 'collision group'. If the application don't
/// do it, a default collision group is used, that will collide with everything.
///
/// The Collision Manager provides a signal for each enabled collision pair, fired when
/// a collision occurs.
///
/// By default, all the Worlds shares the same Collision Manager. A World might choose to
/// use its own Collision Manager though.
//---------------------------------------------------------------------------------------
class ATHENA_PHYSICS_SYMBOL CollisionManager: public btOverlapFilterCallback
{
    //_____ Internal types __________
public:
    //-----------------------------------------------------------------------------------
    /// @brief  Interface to implement to make your own collision filter
    //-----------------------------------------------------------------------------------
    class ICollisionFilter
    {
        //_____ Construction / Destruction __________
    public:
        //-------------------------------------------------------------------------------
        /// @brief  Constructor
        //-------------------------------------------------------------------------------
        ICollisionFilter()
        {
        }

        //-------------------------------------------------------------------------------
        /// @brief  Destructor
        //-------------------------------------------------------------------------------
        virtual ~ICollisionFilter()
        {
        }


        //_____ Methods to implement __________
    public:
        //-------------------------------------------------------------------------------
        /// @brief  Called for each collision pair during the narrowphase
        ///
        /// @param  pComponent1     First collision object of the collision pair
        /// @param  pComponent2     Second collision object of the collision pair
        /// @return                 'true' if a collision must happen
        //-------------------------------------------------------------------------------
        virtual bool needsCollision(CollisionObject* pComponent1,
                                    CollisionObject* pComponent2) = 0;
    };


    //_____ Construction / Destruction __________
public:
    //-----------------------------------------------------------------------------------
    /// @brief  Constructor
    //-----------------------------------------------------------------------------------
    CollisionManager();

    //-----------------------------------------------------------------------------------
    /// @brief  Destructor
    //-----------------------------------------------------------------------------------
    virtual ~CollisionManager();


    //_____ Implementation of btOverlapFilterCallback __________
public:
    //-----------------------------------------------------------------------------------
    /// @brief  Called during the broadphase collision detection to filter out unwanted
    ///         collisions
    ///
    /// @param  pProxy1     Proxy for the first object
    /// @param  pProxy2     Proxy for the second object
    /// @return             'true' if a collision is needed
    //-----------------------------------------------------------------------------------
    virtual bool needBroadphaseCollision(btBroadphaseProxy* pProxy1, btBroadphaseProxy* pProxy2) const;


    //_____ Methods __________
public:
    //-----------------------------------------------------------------------------------
    /// @brief  Enable the collisions between two groups
    ///
    /// @param  group1          The first group
    /// @param  group2          The second group
    /// @param  bEnableFilter   Indicates if the collision filter must be used for this
    ///                         group combination (default: no)
    //-----------------------------------------------------------------------------------
    void enableCollision(tCollisionGroup group1, tCollisionGroup group2,
                         bool bEnableFilter = false);

    //-----------------------------------------------------------------------------------
    /// @brief  Sets the collision filter
    //-----------------------------------------------------------------------------------
    inline void setFilter(ICollisionFilter* pFilter)
    {
        m_pFilter = pFilter;
    }

    //-----------------------------------------------------------------------------------
    /// @brief  Returns the collision filter
    //-----------------------------------------------------------------------------------
    inline ICollisionFilter* getFilter() const
    {
        return m_pFilter;
    }


    //_____ Static methods __________
public:
    //-----------------------------------------------------------------------------------
    /// @brief  Called for each collision pair during the narrowphase
    ///
    /// @param  collisionPair   The collision pair
    /// @param  dispatcher      The dispatcher
    /// @param  dispatchInfo    Some infos about the dispatcher
    //-----------------------------------------------------------------------------------
    static void customNearCallback(btBroadphasePair& collisionPair,
                                   btCollisionDispatcher& dispatcher,
                                   const btDispatcherInfo& dispatchInfo);


private:
    static tCollisionGroup getGroupOfCollisionObject(btCollisionObject* pObject);
    static CollisionObject* getComponentOfCollisionObject(btCollisionObject* pObject, tCollisionGroup &group);


    //_____ Constants __________
private:
    static const unsigned int NB_GROUPS = 32;                                       ///< Maximum number of collision groups
    static const unsigned int NB_PAIRS  = (NB_GROUPS * NB_GROUPS + NB_GROUPS) / 2;  ///< Maximum number of collision pairs


    //_____ Internal types __________
private:
    enum tPairState
    {
        PAIR_DISABLED,
        PAIR_ENABLED,
        PAIR_ENABLED_WITH_FILTER,
    };


    //_____ Static attributes __________
public:
    static CollisionManager     DefaultManager;     ///< Default collision manager
    static CollisionManager*    _CurrentManager;    ///< Current collision manager (internal, do not use)


    //_____ Attributes __________
private:
    tPairState          m_collisionPairs[NB_PAIRS]; ///< Holds all the collision pair infos
    tPairState*         m_indexedPairs[NB_GROUPS];  ///< Used to quickly retrieve a collision pair infos
    ICollisionFilter*   m_pFilter;                  ///< The collision filter
};

}
}

#endif
