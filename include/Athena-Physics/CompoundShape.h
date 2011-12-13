/** @file	CompoundShape.h
	@author	Philip Abbet

	Declaration of the class 'Athena::Physics::CompoundShape'
*/

#ifndef _ATHENA_PHYSICS_COMPOUNDSHAPE_H_
#define _ATHENA_PHYSICS_COMPOUNDSHAPE_H_

#include <Athena-Physics/Prerequisites.h>
#include <Athena-Physics/CollisionShape.h>
#include <Athena-Physics/PrimitiveShape.h>
#include <Athena-Math/Vector3.h>
#include <Athena-Math/Quaternion.h>

namespace Athena {
namespace Physics {


//---------------------------------------------------------------------------------------
/// @brief	Primitive convex shape
///
/// The actual shape and its dimensions can be parametrized
//---------------------------------------------------------------------------------------
class ATHENA_SYMBOL CompoundShape: public CollisionShape
{
	//_____ Construction / Destruction __________
public:
    //-----------------------------------------------------------------------------------
    /// @brief	Constructor
    /// @param	strName		Name of the component
    //-----------------------------------------------------------------------------------
	CompoundShape(const std::string& strName, Entities::ComponentsList* pList);

    //-----------------------------------------------------------------------------------
    /// @brief	Create a new component (Component creation method)
    ///
    /// @param	strName	Name of the component
    /// @param	pList	List to which the component must be added
    /// @return			The new component
    //-----------------------------------------------------------------------------------
	static CompoundShape* create(const std::string& strName, Entities::ComponentsList* pList);

    //-----------------------------------------------------------------------------------
    /// @brief	Cast a component to a CompoundShape
    ///
    /// @param	pComponent	The component
    /// @return				The component, 0 if it isn't castable to a CompoundShape
    //-----------------------------------------------------------------------------------
	static CompoundShape* cast(Entities::Component* pComponent);

protected:
    //-----------------------------------------------------------------------------------
    /// @brief	Destructor
    //-----------------------------------------------------------------------------------
	virtual ~CompoundShape();


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
	/// @brief	Add a box child shape
	///
    /// @param	size	    Dimensions of the box
    /// @param	position	Position of the child shape
    /// @param	orientation	Orientation of the child shape
	//-----------------------------------------------------------------------------------
    void addBox(const Math::Vector3& size, const Math::Vector3& position = Math::Vector3::ZERO,
                const Math::Quaternion& orientation = Math::Quaternion::IDENTITY);

	//-----------------------------------------------------------------------------------
	/// @brief	Add a capsule child shape
	///
    /// @param	radius	    Radius of the half-spheres
    /// @param	height	    Distance between the center of each half-sphere of the capsule
    /// @param	axis	    Axis of the capsule
    /// @param	position	Position of the child shape
    /// @param	orientation	Orientation of the child shape
    ///
    /// @note   The total height is 'height + 2 * radius'
    ///
    /// @remark The CapsuleShape is a convex hull of two spheres. See MultiSphereShape for
    /// a more general collision shape that takes the convex hull of multiple spheres.
    //-----------------------------------------------------------------------------------
    void addCapsule(const Math::Real& radius, const Math::Real& height,
                    PrimitiveShape::tAxis axis = PrimitiveShape::AXIS_Y,
                    const Math::Vector3& position = Math::Vector3::ZERO,
                    const Math::Quaternion& orientation = Math::Quaternion::IDENTITY);

	//-----------------------------------------------------------------------------------
	/// @brief	Add a cone child shape
	///
    /// @param	radius	    Radius of the base of the cone
    /// @param	height	    Height of the cone
    /// @param	axis	    Axis of the cone
    /// @param	position	Position of the child shape
    /// @param	orientation	Orientation of the child shape
    //-----------------------------------------------------------------------------------
    void addCone(const Math::Real& radius, const Math::Real& height,
                 PrimitiveShape::tAxis axis = PrimitiveShape::AXIS_Y,
                 const Math::Vector3& position = Math::Vector3::ZERO,
                 const Math::Quaternion& orientation = Math::Quaternion::IDENTITY);

	//-----------------------------------------------------------------------------------
	/// @brief	Add a cylinder child shape
	///
    /// @param	radius	    Radius of the cylinder
    /// @param	height	    Height of the cylinder
    /// @param	axis	    Axis of the cylinder
    /// @param	position	Position of the child shape
    /// @param	orientation	Orientation of the child shape
    //-----------------------------------------------------------------------------------
    void addCylinder(const Math::Real& radius, const Math::Real& height,
                     PrimitiveShape::tAxis axis = PrimitiveShape::AXIS_Y,
                     const Math::Vector3& position = Math::Vector3::ZERO,
                     const Math::Quaternion& orientation = Math::Quaternion::IDENTITY);

	//-----------------------------------------------------------------------------------
	/// @brief	Add a sphere child shape
	///
    /// @param	radius	    Radius of the sphere
    /// @param	position	Position of the child shape
    /// @param	orientation	Orientation of the child shape
    //-----------------------------------------------------------------------------------
    void addSphere(const Math::Real& radius, const Math::Vector3& position = Math::Vector3::ZERO,
                   const Math::Quaternion& orientation = Math::Quaternion::IDENTITY);

	//-----------------------------------------------------------------------------------
	/// @brief	Returns the number of child shapes
	//-----------------------------------------------------------------------------------
    inline unsigned int getNbChildShapes() const
    {
        return dynamic_cast<btCompoundShape*>(m_pCollisionShape)->getNumChildShapes();
    }

	//-----------------------------------------------------------------------------------
	/// @brief	Remove one of the child shapes
	///
	/// @param  childIndex  Index of the child shape
	//-----------------------------------------------------------------------------------
    void removeChildShape(unsigned int childIndex) const;

	//-----------------------------------------------------------------------------------
	/// @brief	Returns the dimensions of one of the child shapes
	///
	/// @param  childIndex  Index of the child shape
	///
	/// Supported shapes:
	///   - Box
	//-----------------------------------------------------------------------------------
    Math::Vector3 getChildSize(unsigned int childIndex) const;

	//-----------------------------------------------------------------------------------
	/// @brief	Returns the radius of one of the child shapes
	///
	/// @param  childIndex  Index of the child shape
	///
	/// Supported shapes:
	///   - Capsule
	///   - Cone
	///   - Cylinder
	///   - Sphere
	//-----------------------------------------------------------------------------------
    Math::Real getChildRadius(unsigned int childIndex) const;

	//-----------------------------------------------------------------------------------
	/// @brief	Returns the height of one of the child shapes
	///
	/// @param  childIndex  Index of the child shape
	///
	/// Supported shapes:
	///   - Capsule
	///   - Cone
	///   - Cylinder
	//-----------------------------------------------------------------------------------
    Math::Real getChildHeight(unsigned int childIndex) const;

	//-----------------------------------------------------------------------------------
	/// @brief	Returns the axis of one of the child shapes
	///
	/// @param  childIndex  Index of the child shape
	///
	/// Supported shapes:
	///   - Capsule
	///   - Cone
	///   - Cylinder
	//-----------------------------------------------------------------------------------
    PrimitiveShape::tAxis getChildAxis(unsigned int childIndex) const;

	//-----------------------------------------------------------------------------------
	/// @brief	Returns the type of one of the child shapes
	///
	/// @param  childIndex  Index of the child shape
	//-----------------------------------------------------------------------------------
    PrimitiveShape::tShape getChildShape(unsigned int childIndex) const;

	//-----------------------------------------------------------------------------------
	/// @brief	Returns the transforms of one of the child shapes
	///
	/// @param  childIndex  Index of the child shape
	/// @retval position    Position of the child shape
	/// @retval orientation Orientation of the child shape
	//-----------------------------------------------------------------------------------
    void getChildTransforms(unsigned int childIndex, Math::Vector3 &position,
                            Math::Quaternion &orientation) const;


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
