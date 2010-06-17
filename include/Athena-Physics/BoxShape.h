/** @file	BoxShape.h
	@author	Philip Abbet

	Declaration of the class 'Athena::Physics::BoxShape'
*/

#ifndef _ATHENA_PHYSICS_BOXSHAPE_H_
#define _ATHENA_PHYSICS_BOXSHAPE_H_

#include <Athena-Physics/Prerequisites.h>
#include <Athena-Physics/CollisionShape.h>

namespace Athena {
namespace Physics {


//---------------------------------------------------------------------------------------
/// @brief	Box primitive shape around the origin
///
/// The sides are specified by half extents, in local shape coordinates. By default, the
/// box has a 1x1x1 dimension.
//---------------------------------------------------------------------------------------
class ATHENA_SYMBOL BoxShape: public CollisionShape
{
	//_____ Construction / Destruction __________
public:
    //-----------------------------------------------------------------------------------
    /// @brief	Constructor
    /// @param	strName		Name of the component
    //-----------------------------------------------------------------------------------
	BoxShape(const std::string& strName, Entities::ComponentsList* pList);

    //-----------------------------------------------------------------------------------
    /// @brief	Destructor
    //-----------------------------------------------------------------------------------
	virtual ~BoxShape();

    //-----------------------------------------------------------------------------------
    /// @brief	Create a new component (Component creation method)
    ///
    /// @param	strName	Name of the component
    /// @param	pList	List to which the component must be added
    /// @return			The new component
    //-----------------------------------------------------------------------------------
	static BoxShape* create(const std::string& strName, Entities::ComponentsList* pList);

    //-----------------------------------------------------------------------------------
    /// @brief	Cast a component to a BoxShape
    ///
    /// @param	pComponent	The component
    /// @return				The component, 0 if it isn't castable to a BoxShape
    //-----------------------------------------------------------------------------------
	static BoxShape* cast(Entities::Component* pComponent);


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
	/// @brief	Set the size of the box (using half-extents)
	//-----------------------------------------------------------------------------------
    void setSize(const Math::Vector3& halfExtents);

	//-----------------------------------------------------------------------------------
	/// @brief	Returns the size of the box (half-extents)
	//-----------------------------------------------------------------------------------
    Math::Vector3 getSize() const;

	//-----------------------------------------------------------------------------------
	/// @brief	Returns the Bullet's box shape
	//-----------------------------------------------------------------------------------
    inline btBoxShape* getBoxShape() const
    {
        return (btBoxShape*) m_pCollisionShape;
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
	static const std::string TYPE;	///< Name of the type of component
};

}
}

#endif
