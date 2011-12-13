/** @file	PrimitiveShape.h
	@author	Philip Abbet

	Declaration of the class 'Athena::Physics::PrimitiveShape'
*/

#ifndef _ATHENA_PHYSICS_PRIMITIVESHAPE_H_
#define _ATHENA_PHYSICS_PRIMITIVESHAPE_H_

#include <Athena-Physics/Prerequisites.h>
#include <Athena-Physics/CollisionShape.h>

namespace Athena {
namespace Physics {


//---------------------------------------------------------------------------------------
/// @brief	Primitive convex shape
///
/// The actual shape and its dimensions can be parametrized
//---------------------------------------------------------------------------------------
class ATHENA_SYMBOL PrimitiveShape: public CollisionShape
{
    //_____ Internal types __________
public:
    //-----------------------------------------------------------------------------------
    /// @brief	The available shapes
    //-----------------------------------------------------------------------------------
    enum tShape
    {
        SHAPE_BOX,
        SHAPE_CAPSULE,
        SHAPE_CONE,
        SHAPE_CYLINDER,
        SHAPE_SPHERE,
    };

    //-----------------------------------------------------------------------------------
    /// @brief	Some shapes need an axis
    //-----------------------------------------------------------------------------------
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
	PrimitiveShape(const std::string& strName, Entities::ComponentsList* pList);

    //-----------------------------------------------------------------------------------
    /// @brief	Create a new component (Component creation method)
    ///
    /// @param	strName	Name of the component
    /// @param	pList	List to which the component must be added
    /// @return			The new component
    //-----------------------------------------------------------------------------------
	static PrimitiveShape* create(const std::string& strName, Entities::ComponentsList* pList);

    //-----------------------------------------------------------------------------------
    /// @brief	Cast a component to a PrimitiveShape
    ///
    /// @param	pComponent	The component
    /// @return				The component, 0 if it isn't castable to a PrimitiveShape
    //-----------------------------------------------------------------------------------
	static PrimitiveShape* cast(Entities::Component* pComponent);

protected:
    //-----------------------------------------------------------------------------------
    /// @brief	Destructor
    //-----------------------------------------------------------------------------------
	virtual ~PrimitiveShape();


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
	/// @brief	Create a box shape
	///
    /// @param	size	Dimensions of the box
	//-----------------------------------------------------------------------------------
    void createBox(const Math::Vector3& size);

	//-----------------------------------------------------------------------------------
	/// @brief	Create a capsule shape
	///
    /// @param	radius	Radius of the half-spheres
    /// @param	height	Distance between the center of each half-sphere of the capsule
    /// @param	axis	Axis of the capsule
    ///
    /// @note   The total height is 'height + 2 * radius'
    ///
    /// @remark The CapsuleShape is a convex hull of two spheres. See MultiSphereShape for
    /// a more general collision shape that takes the convex hull of multiple spheres.
    //-----------------------------------------------------------------------------------
    void createCapsule(const Math::Real& radius, const Math::Real& height,
                       tAxis axis = AXIS_Y);

	//-----------------------------------------------------------------------------------
	/// @brief	Create a cone shape
	///
    /// @param	radius	Radius of the base of the cone
    /// @param	height	Height of the cone
    /// @param	axis	Axis of the cone
    //-----------------------------------------------------------------------------------
    void createCone(const Math::Real& radius, const Math::Real& height,
                    tAxis axis = AXIS_Y);

	//-----------------------------------------------------------------------------------
	/// @brief	Create a cylinder shape
	///
    /// @param	radius	Radius of the cylinder
    /// @param	height	Height of the cylinder
    /// @param	axis	Axis of the cylinder
    //-----------------------------------------------------------------------------------
    void createCylinder(const Math::Real& radius, const Math::Real& height,
                        tAxis axis = AXIS_Y);

	//-----------------------------------------------------------------------------------
	/// @brief	Create a sphere shape
	///
    /// @param	radius	Radius of the sphere
    //-----------------------------------------------------------------------------------
    void createSphere(const Math::Real& radius);

	//-----------------------------------------------------------------------------------
	/// @brief	Returns the dimensions of the shape
	///
	/// Supported shapes:
	///   - Box
	//-----------------------------------------------------------------------------------
    Math::Vector3 getSize() const;

	//-----------------------------------------------------------------------------------
	/// @brief	Returns the radius of the shape
	///
	/// Supported shapes:
	///   - Capsule
	///   - Cone
	///   - Cylinder
	///   - Sphere
	//-----------------------------------------------------------------------------------
    Math::Real getRadius() const;

	//-----------------------------------------------------------------------------------
	/// @brief	Returns the height of the shape
	///
	/// Supported shapes:
	///   - Capsule
	///   - Cone
	///   - Cylinder
	//-----------------------------------------------------------------------------------
    Math::Real getHeight() const;

	//-----------------------------------------------------------------------------------
	/// @brief	Returns the axis of the shape
	///
	/// Supported shapes:
	///   - Capsule
	///   - Cone
	///   - Cylinder
	//-----------------------------------------------------------------------------------
    inline tAxis getAxis() const
    {
        return m_axis;
    }

	//-----------------------------------------------------------------------------------
	/// @brief	Returns the type of the shape
	//-----------------------------------------------------------------------------------
    inline tShape getShape() const
    {
        return m_shape;
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
    tShape  m_shape;                ///< Type of the shape
    tAxis   m_axis;                 ///< Axis of the shape
};

}
}

#endif
