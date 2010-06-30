/** @file	Body.h
	@author	Philip Abbet

	Declaration of the class 'Athena::Physics::Body'
*/

#ifndef _ATHENA_PHYSICS_BODY_H_
#define _ATHENA_PHYSICS_BODY_H_

#include <Athena-Physics/Prerequisites.h>
#include <Athena-Physics/PhysicalComponent.h>
#include <Athena-Physics/Conversions.h>

namespace Athena {
namespace Physics {


//---------------------------------------------------------------------------------------
/// @brief	Represents a rigid body
///
/// There are 3 types of rigid bodies: 
///   - A) Dynamic rigid bodies, with positive mass. Motion is controlled by rigid body
///        dynamics.
///   - B) Fixed objects with zero mass. They are not moving (basically collision objects)
///   - C) Kinematic objects, which are objects without mass, but the user can move them.
///        This is an one-way interaction, the velocity is computed using the timestep
///        and previous and current world transform.
///
/// To be used in a physical simulation, an entity must have a rigid body component (Body),
/// with a collision shape component assigned to it (CollisionShape, see
/// Body::setCollisionShape).
///
/// The transforms of the body is the center of mass of the physical representation of the
/// entity. If the collision shape shouldn't be centered on the center of mass (the
/// default), use a CompoundShape component. The CompoundShape component can be used to
/// combine several primitive convex collision shapes.
///
/// Remember that the physical simulation will move the dynamic rigid bodies by changing
/// the (world) position and orientation of the transforms origin of the Body component,
/// not the one of the entity. You must then be carefull when choosing the layout of
/// components of an entity. The simplest setup is to use the transforms of the entity as
/// the transforms origin of the Body (the default). 
///
/// Dynamic rigid bodies are automatically deactivated when the velocity is below a 
/// threshold for a given time. Deactivated (sleeping) rigid bodies don't take any
/// processing time, except a minor broadphase collision detection impact (to allow
/// active objects to activate/wake up sleeping objects).
//---------------------------------------------------------------------------------------
class ATHENA_SYMBOL Body: public PhysicalComponent, public btMotionState
{
	//_____ Construction / Destruction __________
public:
    //-----------------------------------------------------------------------------------
    /// @brief	Constructor
    /// @param	strName		Name of the component
    //-----------------------------------------------------------------------------------
	Body(const std::string& strName, Entities::ComponentsList* pList);

    //-----------------------------------------------------------------------------------
    /// @brief	Destructor
    //-----------------------------------------------------------------------------------
	virtual ~Body();

    //-----------------------------------------------------------------------------------
    /// @brief	Create a new component (Component creation method)
    ///
    /// @param	strName	Name of the component
    /// @param	pList	List to which the component must be added
    /// @return			The new component
    //-----------------------------------------------------------------------------------
	static Body* create(const std::string& strName, Entities::ComponentsList* pList);

    //-----------------------------------------------------------------------------------
    /// @brief	Cast a component to a Body
    ///
    /// @param	pComponent	The component
    /// @return				The component, 0 if it isn't castable to a Body
    //-----------------------------------------------------------------------------------
	static Body* cast(Entities::Component* pComponent);


	//_____ Implementation of Component __________
public:
	//-----------------------------------------------------------------------------------
	/// @brief	Returns the type of the component
	/// @return	The type
	//-----------------------------------------------------------------------------------
	virtual const std::string getType() const { return TYPE; }


	//_____ Implementation of btMotionState __________
public:
	//-----------------------------------------------------------------------------------
	/// @brief	Retrieves the world transformations of the body (static and kinematic body)
	//-----------------------------------------------------------------------------------
    virtual void getWorldTransform(btTransform& worldTrans) const;

	//-----------------------------------------------------------------------------------
	/// @brief	Sets the world transformations of the body (dynamic body)
	//-----------------------------------------------------------------------------------
	virtual void setWorldTransform(const btTransform& worldTrans);


	//_____ Methods __________
public:
	//-----------------------------------------------------------------------------------
	/// @brief	Indicates that the rigid body is a kinematic one (or not)
	///
	/// @remark When bKinematic is false, if the body has a mass, it becomes a dynamic
	///         one, otherwise it becomes a static one.
	//-----------------------------------------------------------------------------------
    void setKinematic(bool bKinematic = true);

	//-----------------------------------------------------------------------------------
	/// @brief	Indicates if the body is a kinematic one
	//-----------------------------------------------------------------------------------
    inline bool isKinematic() const
    {
        return m_pBody->isKinematicObject();
    }

	//-----------------------------------------------------------------------------------
	/// @brief	Indicates if the body is a static one
	//-----------------------------------------------------------------------------------
    inline bool isStatic() const
    {
        return m_pBody->isStaticObject();
    }

	//-----------------------------------------------------------------------------------
	/// @brief	Indicates if the body is a dynamic one
	//-----------------------------------------------------------------------------------
    inline bool isDynamic() const
    {
        return !m_pBody->isStaticOrKinematicObject();
    }
    
	//-----------------------------------------------------------------------------------
	/// @brief	Set the mass of the body
	//-----------------------------------------------------------------------------------
    void setMass(Math::Real mass);

	//-----------------------------------------------------------------------------------
	/// @brief	Returns the mass of the body
	//-----------------------------------------------------------------------------------
    inline Math::Real getMass() const
    {
        return m_mass;
    }

	//-----------------------------------------------------------------------------------
	/// @brief	Set the collision shape
	//-----------------------------------------------------------------------------------
    void setCollisionShape(CollisionShape* pShape);

	//-----------------------------------------------------------------------------------
	/// @brief	Retrieve the collision shape
	//-----------------------------------------------------------------------------------
    inline CollisionShape* getCollisionShape() const
    {
        return m_pShape;
    }

	//-----------------------------------------------------------------------------------
	/// @brief	Enable/Disable the automatic deactivation of the body (when no movement
	///         is done for a few time)
    ///
    /// Enabled by default
	//-----------------------------------------------------------------------------------
    inline void enableDeactivation(bool bEnabled)
	{
        m_pBody->setActivationState(bEnabled ? WANTS_DEACTIVATION : DISABLE_DEACTIVATION);
	}

	//-----------------------------------------------------------------------------------
	/// @brief	Enable/Disable the simulation of the body
    ///
    /// Enabled by default
	//-----------------------------------------------------------------------------------
    inline void enableSimulation(bool bEnabled)
	{
        m_pBody->setActivationState(bEnabled ? WANTS_DEACTIVATION : DISABLE_SIMULATION);
	}

	//-----------------------------------------------------------------------------------
	/// @brief	Sets the linear velocity of the body
	//-----------------------------------------------------------------------------------
    inline void setLinearVelocity(const Math::Vector3& velocity)
	{
        m_pBody->setLinearVelocity(toBullet(velocity));
	}

	//-----------------------------------------------------------------------------------
	/// @brief	Returns the linear velocity of the body
	//-----------------------------------------------------------------------------------
    inline Math::Vector3 getLinearVelocity()
	{
        return fromBullet(m_pBody->getLinearVelocity());
	}

	//-----------------------------------------------------------------------------------
	/// @brief	Sets the angular velocity of the body
	//-----------------------------------------------------------------------------------
    inline void setAngularVelocity(const Math::Vector3& velocity)
	{
        m_pBody->setAngularVelocity(toBullet(velocity));
	}

	//-----------------------------------------------------------------------------------
	/// @brief	Returns the angular velocity of the body
	//-----------------------------------------------------------------------------------
    inline Math::Vector3 getAngularVelocity()
	{
        return fromBullet(m_pBody->getAngularVelocity());
	}

	//-----------------------------------------------------------------------------------
	/// @brief	Modify the angular freedom of movement of the body, per axis
	///
	/// 0.0 means no rotation at all. Default: 1.0
	//-----------------------------------------------------------------------------------
    inline void setAngularFactor(const Math::Vector3& factors)
	{
        m_pBody->setAngularFactor(toBullet(factors));
	}

	//-----------------------------------------------------------------------------------
	/// @brief	Modify the angular freedom of movement of the body, same value for all
	///         the axes
	///
	/// 0.0 means no rotation at all. Default: 1.0
	//-----------------------------------------------------------------------------------
    inline void setAngularFactor(Math::Real factor)
	{
        m_pBody->setAngularFactor(factor);
	}

	//-----------------------------------------------------------------------------------
	/// @brief	Returns the angular freedom of movement of the body, per axis
	///
	/// 0.0 means no rotation at all. Default: 1.0
	//-----------------------------------------------------------------------------------
	inline Math::Vector3 getAngularFactor() const
	{
		return fromBullet(m_pBody->getAngularFactor());
	}

	//-----------------------------------------------------------------------------------
	/// @brief	Returns the Bullet's rigid body
	//-----------------------------------------------------------------------------------
    inline btRigidBody* getRigidBody() const
    {
        return m_pBody;
    }

protected:
    void updateBody();

	//-----------------------------------------------------------------------------------
	/// @brief	Called when the transforms affecting this component have changed
	///
	/// Can be called when the component isn't affected by any transforms anymore
	/// (getTransforms() returns 0).
	//-----------------------------------------------------------------------------------
	virtual void onTransformsChanged();


	//_____ Slots __________
protected:
	void onCollisionShapeDestroyed(Utils::Variant* pValue);


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
	static const std::string TYPE;	///< Name of the type of component


    //_____ Attributes __________
protected:
    btRigidBody*    m_pBody;        ///< The body
    Math::Real      m_mass;         ///< The mass of the body
    CollisionShape* m_pShape;       ///< The collision shape
};

}
}

#endif
