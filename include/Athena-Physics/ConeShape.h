/** @file	ConeShape.h
	@author	Philip Abbet

	Declaration of the class 'Athena::Physics::ConeShape'
*/

#ifndef _ATHENA_PHYSICS_SPHERESHAPE_H_
#define _ATHENA_PHYSICS_SPHERESHAPE_H_

#include <Athena-Physics/Prerequisites.h>
#include <Athena-Physics/CollisionShape.h>

namespace Athena {
namespace Physics {


//---------------------------------------------------------------------------------------
/// @brief	Represents a cone shape around an axis (X, Y or Z).
///
/// By default, the following values are used:
///   - axis:   Y
///   - radius: 0.25
///   - height: 1.0
//---------------------------------------------------------------------------------------
class ATHENA_SYMBOL ConeShape: public CollisionShape
{
    //_____ Internal types __________
public:
    enum tAxis
    {
        AXIS_X,
        AXIS_Y,
        AXIS_Z
    };
    
    
	//_____ Construction / Destruction __________
public:
    //-----------------------------------------------------------------------------------
    /// @brief	Constructor
    /// @param	strName		Name of the component
    //-----------------------------------------------------------------------------------
	ConeShape(const std::string& strName, Entities::ComponentsList* pList);

    //-----------------------------------------------------------------------------------
    /// @brief	Destructor
    //-----------------------------------------------------------------------------------
	virtual ~ConeShape();

    //-----------------------------------------------------------------------------------
    /// @brief	Create a new component (Component creation method)
    ///
    /// @param	strName	Name of the component
    /// @param	pList	List to which the component must be added
    /// @return			The new component
    //-----------------------------------------------------------------------------------
	static ConeShape* create(const std::string& strName, Entities::ComponentsList* pList);

    //-----------------------------------------------------------------------------------
    /// @brief	Cast a component to a ConeShape
    ///
    /// @param	pComponent	The component
    /// @return				The component, 0 if it isn't castable to a ConeShape
    //-----------------------------------------------------------------------------------
	static ConeShape* cast(Entities::Component* pComponent);


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
	/// @brief	Set the dimensions of the cone
	//-----------------------------------------------------------------------------------
    void setDimensions(const Math::Real& radius, const Math::Real& height,
                       tAxis axis = AXIS_Y);

	//-----------------------------------------------------------------------------------
	/// @brief	Returns the radius of the cone
	//-----------------------------------------------------------------------------------
    Math::Real getRadius() const;

	//-----------------------------------------------------------------------------------
	/// @brief	Returns the height of the cone
	//-----------------------------------------------------------------------------------
    Math::Real getHeight() const;

	//-----------------------------------------------------------------------------------
	/// @brief	Returns the axis of the cone
	//-----------------------------------------------------------------------------------
    inline tAxis getAxis() const
    {
        return m_axis;
    }

	//-----------------------------------------------------------------------------------
	/// @brief	Returns the Bullet's cone shape
	//-----------------------------------------------------------------------------------
    inline btConeShape* getConeShape() const
    {
        return (btConeShape*) m_pCollisionShape;
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


    //_____ Attributes __________
protected:
    tAxis m_axis;                   ///< Axis of the cone
};

}
}

#endif
