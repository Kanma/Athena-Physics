/** @file	PhysicalComponent.h
	@author	Philip Abbet

	Declaration of the class 'Athena::Physics::PhysicalComponent'
*/

#ifndef _ATHENA_PHYSICS_PHYSICALCOMPONENT_H_
#define _ATHENA_PHYSICS_PHYSICALCOMPONENT_H_

#include <Athena-Physics/Prerequisites.h>
#include <Athena-Entities/Component.h>
#include <Athena-Entities/Entity.h>
#include <Athena-Entities/Scene.h>


namespace Athena {
namespace Physics {


//---------------------------------------------------------------------------------------
/// @brief	Base class for all the physical components of an entity
//---------------------------------------------------------------------------------------
class ATHENA_PHYSICS_SYMBOL PhysicalComponent: public Entities::Component
{
	//_____ Construction / Destruction __________
public:
    //-----------------------------------------------------------------------------------
    /// @brief	Constructor
    /// @param	strName		Name of the component
    //-----------------------------------------------------------------------------------
	PhysicalComponent(const std::string& strName, Entities::ComponentsList* pList);

    //-----------------------------------------------------------------------------------
    /// @brief	Create a new component (Component creation method)
    ///
    /// @param	strName	Name of the component
    /// @param	pList	List to which the component must be added
    /// @return			The new component
    //-----------------------------------------------------------------------------------
	static PhysicalComponent* create(const std::string& strName, Entities::ComponentsList* pList);

    //-----------------------------------------------------------------------------------
    /// @brief	Cast a component to a PhysicalComponent
    ///
    /// @param	pComponent	The component
    /// @return				The component, 0 if it isn't castable to a PhysicalComponent
    //-----------------------------------------------------------------------------------
	static PhysicalComponent* cast(Entities::Component* pComponent);

protected:
    //-----------------------------------------------------------------------------------
    /// @brief	Destructor
    //-----------------------------------------------------------------------------------
	virtual ~PhysicalComponent();


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
	/// @brief	Returns the physical world this component lives in
	/// @return	The physical world
	/// @remark It is assumed that the scene has only one physical world, with the
	/// default name ('PhysicalWorld')
	//-----------------------------------------------------------------------------------
    World* getWorld() const;


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
	static const std::string TYPE;		///< Name of the type of component
};

}
}

#endif
