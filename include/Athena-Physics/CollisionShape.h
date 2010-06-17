/** @file	CollisionShape.h
	@author	Philip Abbet

	Declaration of the class 'Athena::Physics::CollisionShape'
*/

#ifndef _ATHENA_PHYSICS_COLLISIONSHAPE_H_
#define _ATHENA_PHYSICS_COLLISIONSHAPE_H_

#include <Athena-Physics/Prerequisites.h>
#include <Athena-Physics/PhysicalComponent.h>

namespace Athena {
namespace Physics {


//---------------------------------------------------------------------------------------
/// @brief	Base class for all the collision shapes
///
/// Collision shapes are used to specify the shape of a body in the physical simulation.
/// See Body for a detailed explanation.
//---------------------------------------------------------------------------------------
class ATHENA_SYMBOL CollisionShape: public PhysicalComponent
{
    friend class Body;

    
	//_____ Construction / Destruction __________
public:
    //-----------------------------------------------------------------------------------
    /// @brief	Constructor
    /// @param	strName		Name of the component
    //-----------------------------------------------------------------------------------
	CollisionShape(const std::string& strName, Entities::ComponentsList* pList);

    //-----------------------------------------------------------------------------------
    /// @brief	Destructor
    //-----------------------------------------------------------------------------------
	virtual ~CollisionShape();

    //-----------------------------------------------------------------------------------
    /// @brief	Create a new component (Component creation method)
    ///
    /// @param	strName	Name of the component
    /// @param	pList	List to which the component must be added
    /// @return			The new component
    //-----------------------------------------------------------------------------------
	static CollisionShape* create(const std::string& strName, Entities::ComponentsList* pList);

    //-----------------------------------------------------------------------------------
    /// @brief	Cast a component to a CollisionShape
    ///
    /// @param	pComponent	The component
    /// @return				The component, 0 if it isn't castable to a CollisionShape
    //-----------------------------------------------------------------------------------
	static CollisionShape* cast(Entities::Component* pComponent);


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
	/// @brief	Returns the Bullet's rigid body
	//-----------------------------------------------------------------------------------
    inline btCollisionShape* getCollisionShape() const
    {
        return m_pCollisionShape;
    }


protected:
    void setBody(Body* pBody)
    {
        m_pBody = pBody;
    }
    

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
	static const std::string TYPE;	        ///< Name of the type of component


    //_____ Attributes __________
protected:
    btCollisionShape*   m_pCollisionShape;  ///< The Bullet shape
    Body*               m_pBody;            ///< The rigid body using this shape (if any)
};

}
}

#endif
