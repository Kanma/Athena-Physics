/** @file   CollisionObject.h
    @author Philip Abbet

    Declaration of the class 'Athena::Physics::CollisionObject'
*/

#ifndef _ATHENA_PHYSICS_COLLISIONOBJECT_H_
#define _ATHENA_PHYSICS_COLLISIONOBJECT_H_

#include <Athena-Physics/Prerequisites.h>
#include <Athena-Physics/PhysicalComponent.h>
#include <Athena-Physics/Conversions.h>

namespace Athena {
namespace Physics {


//---------------------------------------------------------------------------------------
/// @brief  Base class for all the physical components that can collide with others
//---------------------------------------------------------------------------------------
class ATHENA_PHYSICS_SYMBOL CollisionObject: public PhysicalComponent
{
    //_____ Construction / Destruction __________
public:
    //-----------------------------------------------------------------------------------
    /// @brief  Constructor
    /// @param  strName     Name of the component
    //-----------------------------------------------------------------------------------
    CollisionObject(const std::string& strName, Entities::ComponentsList* pList);

    //-----------------------------------------------------------------------------------
    /// @brief  Create a new component (Component creation method)
    ///
    /// @param  strName Name of the component
    /// @param  pList   List to which the component must be added
    /// @return         The new component
    //-----------------------------------------------------------------------------------
    static CollisionObject* create(const std::string& strName, Entities::ComponentsList* pList);

    //-----------------------------------------------------------------------------------
    /// @brief  Cast a component to a CollisionObject
    ///
    /// @param  pComponent  The component
    /// @return             The component, 0 if it isn't castable to a CollisionObject
    //-----------------------------------------------------------------------------------
    static CollisionObject* cast(Entities::Component* pComponent);

protected:
    //-----------------------------------------------------------------------------------
    /// @brief  Destructor
    //-----------------------------------------------------------------------------------
    virtual ~CollisionObject();


    //_____ Implementation of Component __________
public:
    //-----------------------------------------------------------------------------------
    /// @brief  Returns the type of the component
    /// @return The type
    //-----------------------------------------------------------------------------------
    virtual const std::string getType() const { return TYPE; }


    //_____ Methods __________
public:
    //-----------------------------------------------------------------------------------
    /// @brief  Set the 'collision group' of the ghost object
    //-----------------------------------------------------------------------------------
    inline void setCollisionGroup(tCollisionGroup group)
    {
        m_collisionGroup = group;
    }

    //-----------------------------------------------------------------------------------
    /// @brief  Returns the 'collision group' of the ghost object
    //-----------------------------------------------------------------------------------
    inline tCollisionGroup getCollisionGroup() const
    {
        return m_collisionGroup;
    }


    //_____ Management of the properties __________
public:
    //-----------------------------------------------------------------------------------
    /// @brief  Returns a list containing the properties of the component
    ///
    /// Used in the serialization mecanism of the components
    /// @remark Must be overriden by each component type. Each implementation must first call
    ///         its base class one, and add a new category (named after the component's type)
    ///         AT THE BEGINNING of the obtained list, containing the properties related to
    ///         this type.
    /// @return The list of properties
    //-----------------------------------------------------------------------------------
    virtual Utils::PropertiesList* getProperties() const;

    //-----------------------------------------------------------------------------------
    /// @brief  Set the value of a property of the component
    ///
    /// Used in the deserialization mecanism of the parts
    /// @param  strCategory     The category of the property
    /// @param  strName         The name of the property
    /// @param  pValue          The value of the property
    /// @return                 'true' if the property was used, 'false' if a required object
    ///                         is missing
    /// @remark Must be overriden by each component type. Each implementation must test if the
    ///         property's category is the one of the component's type, and if so process the
    ///         property's value. Otherwise, it must call its base class implementation.
    //-----------------------------------------------------------------------------------
    virtual bool setProperty(const std::string& strCategory, const std::string& strName,
                             Utils::Variant* pValue);

    //-----------------------------------------------------------------------------------
    /// @brief  Set the value of a property of the component
    ///
    /// Used in the deserialization mecanism of the parts
    /// @param  strName     The name of the property
    /// @param  pValue      The value of the property
    /// @return             'true' if the property was used, 'false' if a required object
    ///                     is missing
    //-----------------------------------------------------------------------------------
    bool setProperty(const std::string& strName, Utils::Variant* pValue);


    //_____ Constants __________
public:
    static const std::string TYPE;  ///< Name of the type of component


    //_____ Attributes __________
protected:
    tCollisionGroup m_collisionGroup;   ///< The collision group
};

}
}

#endif
