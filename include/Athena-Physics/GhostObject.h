/** @file   GhostObject.h
    @author Philip Abbet

    Declaration of the class 'Athena::Physics::GhostObject'
*/

#ifndef _ATHENA_PHYSICS_GHOSTOBJECT_H_
#define _ATHENA_PHYSICS_GHOSTOBJECT_H_

#include <Athena-Physics/Prerequisites.h>
#include <Athena-Physics/CollisionObject.h>
#include <Athena-Physics/Conversions.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>

namespace Athena {
namespace Physics {


//---------------------------------------------------------------------------------------
/// @brief  A ghost object can be used to retrieve a list of all the collisions that
///         happened with its shape during the last simulation step
///
/// The ghost object doesn't have any influence on the simulation
//---------------------------------------------------------------------------------------
class ATHENA_PHYSICS_SYMBOL GhostObject: public CollisionObject
{
    //_____ Construction / Destruction __________
public:
    //-----------------------------------------------------------------------------------
    /// @brief  Constructor
    /// @param  strName     Name of the component
    //-----------------------------------------------------------------------------------
    GhostObject(const std::string& strName, Entities::ComponentsList* pList);

    //-----------------------------------------------------------------------------------
    /// @brief  Create a new component (Component creation method)
    ///
    /// @param  strName Name of the component
    /// @param  pList   List to which the component must be added
    /// @return         The new component
    //-----------------------------------------------------------------------------------
    static GhostObject* create(const std::string& strName, Entities::ComponentsList* pList);

    //-----------------------------------------------------------------------------------
    /// @brief  Cast a component to a GhostObject
    ///
    /// @param  pComponent  The component
    /// @return             The component, 0 if it isn't castable to a GhostObject
    //-----------------------------------------------------------------------------------
    static GhostObject* cast(Entities::Component* pComponent);

protected:
    //-----------------------------------------------------------------------------------
    /// @brief  Destructor
    //-----------------------------------------------------------------------------------
    virtual ~GhostObject();


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
    /// @brief  Set the shape
    //-----------------------------------------------------------------------------------
    void setCollisionShape(CollisionShape* pShape);

    //-----------------------------------------------------------------------------------
    /// @brief  Retrieve the shape
    //-----------------------------------------------------------------------------------
    inline CollisionShape* getCollisionShape() const
    {
        return m_pShape;
    }

    //-----------------------------------------------------------------------------------
    /// @brief  Returns the number of objects that are overlapping with the shape of
    ///         the ghost object
    //-----------------------------------------------------------------------------------
    inline unsigned int getNbOverlappingObjects() const
    {
        return (unsigned) m_pGhostObject->getNumOverlappingObjects();
    }

    //-----------------------------------------------------------------------------------
    /// @brief  Returns one of the objects that are overlapping with the shape of the
    ///         ghost object
    //-----------------------------------------------------------------------------------
    PhysicalComponent* getOverlappingObject(unsigned int index);

    //-----------------------------------------------------------------------------------
    /// @brief  Returns the Bullet's ghost object
    //-----------------------------------------------------------------------------------
    inline btGhostObject* getGhostObject() const
    {
        return m_pGhostObject;
    }

    //-----------------------------------------------------------------------------------
    /// @brief  Called when the transforms affecting this component have changed
    ///
    /// Can be called when the component isn't affected by any transforms anymore
    /// (getTransforms() returns 0).
    //-----------------------------------------------------------------------------------
    virtual void onTransformsChanged();


    //_____ Links management __________
protected:
    //-----------------------------------------------------------------------------------
    /// @brief  Called when a component this one is linked to must be unlinked
    ///
    /// @remark If you override it in your component, don't forget to call the base class
    ///         implementation!
    //-----------------------------------------------------------------------------------
    virtual void mustUnlinkComponent(Component* pComponent);


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
    btGhostObject*  m_pGhostObject;     ///< The ghost object
    CollisionShape* m_pShape;           ///< The shape
};

}
}

#endif
