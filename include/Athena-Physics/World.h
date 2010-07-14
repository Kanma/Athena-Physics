/** @file	World.h
	@author	Philip Abbet

	Declaration of the class 'Athena::Physics::World'
*/

#ifndef _ATHENA_PHYSICS_WORLD_H_
#define _ATHENA_PHYSICS_WORLD_H_

#include <Athena-Physics/Prerequisites.h>
#include <Athena-Physics/PhysicalComponent.h>
#include <BulletSoftBody/btSoftRigidDynamicsWorld.h>

namespace Athena {
namespace Physics {


//---------------------------------------------------------------------------------------
/// @brief	Represents a physical world, used to perform a physical simulation
///
/// There can be only one physical world per scene, and it MUST be a component of the
/// scene itself (not of an entity). Additionally, the name of the world component will
/// always be equal to World::DEFAULT_NAME.
//---------------------------------------------------------------------------------------
class ATHENA_SYMBOL World: public PhysicalComponent
{
    friend class Body;
    friend class GhostObject;
    
    
    //_____ Internal types __________
public:
    //-----------------------------------------------------------------------------------
    /// @brief	The available type of worlds. Determines the kind of physical simulation
    ///         performed
    //-----------------------------------------------------------------------------------
    enum tType
    {
        WORLD_RIGID_BODY,   ///< Rigid body simulation (the default)
        WORLD_SOFT_BODY,    ///< Soft body simulation
    };
    
    
    typedef std::vector<btManifoldPoint>                tContactPointsList;
    typedef Utils::VectorIterator<tContactPointsList>   tContactPointsIterator;
    typedef tContactPointsList::iterator                tContactPointsNativeIterator;
    

	//_____ Construction / Destruction __________
public:
    //-----------------------------------------------------------------------------------
    /// @brief	Constructor
    //-----------------------------------------------------------------------------------
	World(Entities::ComponentsList* pList);

    //-----------------------------------------------------------------------------------
    /// @brief	Destructor
    //-----------------------------------------------------------------------------------
	virtual ~World();

    //-----------------------------------------------------------------------------------
    /// @brief	Create a new component (Component creation method)
    ///
    /// @param	strName	Name of the component (not used, kept for compatibility)
    /// @param	pList	List to which the component must be added
    /// @return			The new component
    //-----------------------------------------------------------------------------------
	static World* create(const std::string& strName, Entities::ComponentsList* pList);

    //-----------------------------------------------------------------------------------
    /// @brief	Cast a component to a World
    ///
    /// @param	pComponent	The component
    /// @return				The component, 0 if it isn't castable to a World
    //-----------------------------------------------------------------------------------
	static World* cast(Entities::Component* pComponent);


	//_____ Implementation of Component __________
public:
	//-----------------------------------------------------------------------------------
	/// @brief	Returns the type of the component
	/// @return	The type
	//-----------------------------------------------------------------------------------
	virtual const std::string getType() const { return TYPE; }


	//_____ Methods __________
public:
	//-----------------------------------------------------------------------------------
	/// @brief	Set the type of the world
	//-----------------------------------------------------------------------------------
    void setWorldType(tType type);

	//-----------------------------------------------------------------------------------
	/// @brief	Returns the type of the world
	//-----------------------------------------------------------------------------------
    inline tType getWorldType() const
    {
        return m_type;
    }

	//-----------------------------------------------------------------------------------
	/// @brief	Set the gravity
	//-----------------------------------------------------------------------------------
	void setGravity(const Math::Vector3& gravity);

	//-----------------------------------------------------------------------------------
	/// @brief	Retrieve the gravity
	//-----------------------------------------------------------------------------------
	Math::Vector3 getGravity();

	//-----------------------------------------------------------------------------------
	/// @brief	Proceeds the simulation over 'timeStep' seconds
    ///
    /// By default, the timestep is divided in constant substeps of each 'fixedTimeStep'.
    /// In order to keep the simulation real-time, the maximum number of substeps can be
    /// clamped to 'nbMaxSubSteps'. You can disable subdividing the timestep/substepping
    /// by passing nbMaxSubSteps=0 as second argument to stepSimulation, but in that case
    /// you have to keep the timeStep constant.
    ///
    /// @return The number of substeps simulated
    //-----------------------------------------------------------------------------------
	unsigned int stepSimulation(Math::Real timeStep, unsigned int nbMaxSubSteps = 1,
	                            Math::Real fixedTimeStep = Math::Real(1.0 / 60.0));

	//-----------------------------------------------------------------------------------
	/// @brief	Returns the Bullet's rigid body simulation world
	//-----------------------------------------------------------------------------------
    inline btDiscreteDynamicsWorld* getRigidBodyWorld()
    {
        if (!m_pWorld)
            createWorld();

        return m_pWorld;
    }

	//-----------------------------------------------------------------------------------
	/// @brief	Returns the Bullet's soft body simulation world (can be 0, see
	///         getWorldType())
	//-----------------------------------------------------------------------------------
    inline btSoftRigidDynamicsWorld* getSoftBodyWorld() const
    {
        return dynamic_cast<btSoftRigidDynamicsWorld*>(m_pWorld);
    }

	//-----------------------------------------------------------------------------------
	/// @brief	Returns the Collision Manager used by this world
	//-----------------------------------------------------------------------------------
    inline CollisionManager* getCollisionManager() const
    {
        return m_pCollisionManager;
    }

	//-----------------------------------------------------------------------------------
	/// @brief	Returns a list of all the contact points between two physical components
	///
	/// @param  pComponent1     The first component
	/// @param  pComponent2     The second component
	/// @retval contacts        An array of the contact points between the two components
	///
	/// Usage example:
	/// @code
	/// World::tContactPointsList contacts;
	/// pWorld->getContacts(pComponent1, pComponent2, contacts);
	/// for (int j = 0; j < contacts.size(); ++j)
    /// {
    ///     const btManifoldPoint& pt = contacts[j];
    ///
    ///		const btVector3& ptA = pt.getPositionWorldOnA();
    ///		const btVector3& ptB = pt.getPositionWorldOnB();
    ///		const btVector3& normalOnB = pt.m_normalWorldOnB;
    ///		...
    ///	}
	/// @endcode
	//-----------------------------------------------------------------------------------
    void getContacts(PhysicalComponent* pComponent1, PhysicalComponent* pComponent2,
                     tContactPointsList &contactPoints);

protected:
    void createWorld();
    void addRigidBody(Body* pBody);
    void removeRigidBody(Body* pBody);
    void addGhostObject(GhostObject* pGhostObject);
    void removeGhostObject(GhostObject* pGhostObject);
    

	//_____ Management of the properties __________
public:
    //-----------------------------------------------------------------------------------
    /// @brief	Returns a list containing the properties of the component
    ///
    /// Used in the serialization mecanism of the components
    /// @remark	Must be overriden by each component type. Each implementation must first call
    ///			its base class one, and add a new category (named after the component's type)
    ///			AT THE BEGINNING of the obtained list, containing the properties related to
    ///			this type.
    /// @return	The list of properties
    //-----------------------------------------------------------------------------------
	virtual Utils::PropertiesList* getProperties() const;
	
    //-----------------------------------------------------------------------------------
    /// @brief	Set the value of a property of the component
    ///
    /// Used in the deserialization mecanism of the parts
    ///	@param	strCategory		The category of the property
    ///	@param	strName			The name of the property
    ///	@param	pValue			The value of the property
    ///	@return					'true' if the property was used, 'false' if a required object
    ///							is missing
    /// @remark	Must be overriden by each component type. Each implementation must test if the
    ///			property's category is the one of the component's type, and if so process the
    ///			property's value. Otherwise, it must call its base class implementation.
    //-----------------------------------------------------------------------------------
	virtual bool setProperty(const std::string& strCategory, const std::string& strName,
							 Utils::Variant* pValue);

    //-----------------------------------------------------------------------------------
    /// @brief	Set the value of a property of the component
    ///
    /// Used in the deserialization mecanism of the parts
    ///	@param	strName		The name of the property
    ///	@param	pValue		The value of the property
    ///	@return				'true' if the property was used, 'false' if a required object
    ///						is missing
    //-----------------------------------------------------------------------------------
	bool setProperty(const std::string& strName, Utils::Variant* pValue);


	//_____ Constants __________
public:
	static const std::string TYPE;		    ///< Name of the type of component
	static const std::string DEFAULT_NAME;	///< Default name of the world


    //_____ Attributes __________
protected:
    tType                       m_type;                     ///< Type of world
    btDiscreteDynamicsWorld*    m_pWorld;                   ///< The world doing the simulation
    btDispatcher*               m_pDispatcher;
    btBroadphaseInterface*      m_pBroadphase;
    btConstraintSolver*         m_pConstraintSolver;
    btCollisionConfiguration*   m_pCollisionConfiguration;
    CollisionManager*           m_pCollisionManager;
};

}
}

#endif
