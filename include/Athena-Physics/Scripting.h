/** @file   Scripting.h
    @author Philip Abbet

    Scripting-related declarations for the Athena-Physics module
*/

#ifndef _ATHENA_PHYSICS_SCRIPTING_H_
#define _ATHENA_PHYSICS_SCRIPTING_H_

#include <v8.h>
#include <Athena-Physics/Prerequisites.h>


namespace Athena {
namespace Physics {

    //------------------------------------------------------------------------------------
    /// @brief  Retrieve the C++ Body wrapped by a JavaScript one
    //------------------------------------------------------------------------------------
    ATHENA_PHYSICS_SYMBOL Body* fromJSBody(v8::Handle<v8::Value> value);

    //------------------------------------------------------------------------------------
    /// @brief  Create a new JavaScript Body (wrapping a new C++ one)
    //------------------------------------------------------------------------------------
    v8::Handle<v8::Object> ATHENA_PHYSICS_SYMBOL createJSBody();

    //------------------------------------------------------------------------------------
    /// @brief  Wrap a new JavaScript Body around a C++ one
    //------------------------------------------------------------------------------------
    v8::Handle<v8::Value> ATHENA_PHYSICS_SYMBOL toJavaScript(Body* pBody);


    //------------------------------------------------------------------------------------
    /// @brief  Retrieve the C++ CollisionObject wrapped by a JavaScript one
    //------------------------------------------------------------------------------------
    ATHENA_PHYSICS_SYMBOL CollisionObject* fromJSCollisionObject(v8::Handle<v8::Value> value);

    //------------------------------------------------------------------------------------
    /// @brief  Create a new JavaScript CollisionObject (wrapping a new C++ one)
    //------------------------------------------------------------------------------------
    v8::Handle<v8::Object> ATHENA_PHYSICS_SYMBOL createJSCollisionObject();

    //------------------------------------------------------------------------------------
    /// @brief  Wrap a new JavaScript CollisionObject around a C++ one
    //------------------------------------------------------------------------------------
    v8::Handle<v8::Value> ATHENA_PHYSICS_SYMBOL toJavaScript(CollisionObject* pCollisionObject);


    //------------------------------------------------------------------------------------
    /// @brief  Retrieve the C++ CollisionShape wrapped by a JavaScript one
    //------------------------------------------------------------------------------------
    ATHENA_PHYSICS_SYMBOL CollisionShape* fromJSCollisionShape(v8::Handle<v8::Value> value);

    //------------------------------------------------------------------------------------
    /// @brief  Create a new JavaScript CollisionShape (wrapping a new C++ one)
    //------------------------------------------------------------------------------------
    v8::Handle<v8::Object> ATHENA_PHYSICS_SYMBOL createJSCollisionShape();

    //------------------------------------------------------------------------------------
    /// @brief  Wrap a new JavaScript CollisionShape around a C++ one
    //------------------------------------------------------------------------------------
    v8::Handle<v8::Value> ATHENA_PHYSICS_SYMBOL toJavaScript(CollisionShape* pCollisionShape);


    //------------------------------------------------------------------------------------
    /// @brief  Retrieve the C++ CompoundShape wrapped by a JavaScript one
    //------------------------------------------------------------------------------------
    ATHENA_PHYSICS_SYMBOL CompoundShape* fromJSCompoundShape(v8::Handle<v8::Value> value);

    //------------------------------------------------------------------------------------
    /// @brief  Create a new JavaScript CompoundShape (wrapping a new C++ one)
    //------------------------------------------------------------------------------------
    v8::Handle<v8::Object> ATHENA_PHYSICS_SYMBOL createJSCompoundShape();

    //------------------------------------------------------------------------------------
    /// @brief  Wrap a new JavaScript CompoundShape around a C++ one
    //------------------------------------------------------------------------------------
    v8::Handle<v8::Value> ATHENA_PHYSICS_SYMBOL toJavaScript(CompoundShape* pCompoundShape);


    //------------------------------------------------------------------------------------
    /// @brief  Retrieve the C++ GhostObject wrapped by a JavaScript one
    //------------------------------------------------------------------------------------
    ATHENA_PHYSICS_SYMBOL GhostObject* fromJSGhostObject(v8::Handle<v8::Value> value);

    //------------------------------------------------------------------------------------
    /// @brief  Create a new JavaScript GhostObject (wrapping a new C++ one)
    //------------------------------------------------------------------------------------
    v8::Handle<v8::Object> ATHENA_PHYSICS_SYMBOL createJSGhostObject();

    //------------------------------------------------------------------------------------
    /// @brief  Wrap a new JavaScript GhostObject around a C++ one
    //------------------------------------------------------------------------------------
    v8::Handle<v8::Value> ATHENA_PHYSICS_SYMBOL toJavaScript(GhostObject* pGhostObject);


    //------------------------------------------------------------------------------------
    /// @brief  Retrieve the C++ PhysicalComponent wrapped by a JavaScript one
    //------------------------------------------------------------------------------------
    ATHENA_PHYSICS_SYMBOL PhysicalComponent* fromJSPhysicalComponent(v8::Handle<v8::Value> value);

    //------------------------------------------------------------------------------------
    /// @brief  Create a new JavaScript PhysicalComponent (wrapping a new C++ one)
    //------------------------------------------------------------------------------------
    v8::Handle<v8::Object> ATHENA_PHYSICS_SYMBOL createJSPhysicalComponent();

    //------------------------------------------------------------------------------------
    /// @brief  Wrap a new JavaScript PhysicalComponent around a C++ one
    //------------------------------------------------------------------------------------
    v8::Handle<v8::Value> ATHENA_PHYSICS_SYMBOL toJavaScript(PhysicalComponent* pPhysicalComponent);


    //------------------------------------------------------------------------------------
    /// @brief  Retrieve the C++ PrimitiveShape wrapped by a JavaScript one
    //------------------------------------------------------------------------------------
    ATHENA_PHYSICS_SYMBOL PrimitiveShape* fromJSPrimitiveShape(v8::Handle<v8::Value> value);

    //------------------------------------------------------------------------------------
    /// @brief  Create a new JavaScript PrimitiveShape (wrapping a new C++ one)
    //------------------------------------------------------------------------------------
    v8::Handle<v8::Object> ATHENA_PHYSICS_SYMBOL createJSPrimitiveShape();

    //------------------------------------------------------------------------------------
    /// @brief  Wrap a new JavaScript PrimitiveShape around a C++ one
    //------------------------------------------------------------------------------------
    v8::Handle<v8::Value> ATHENA_PHYSICS_SYMBOL toJavaScript(PrimitiveShape* pPrimitiveShape);


    //------------------------------------------------------------------------------------
    /// @brief  Retrieve the C++ StaticTriMeshShape wrapped by a JavaScript one
    //------------------------------------------------------------------------------------
    ATHENA_PHYSICS_SYMBOL StaticTriMeshShape* fromJSStaticTriMeshShape(v8::Handle<v8::Value> value);

    //------------------------------------------------------------------------------------
    /// @brief  Create a new JavaScript StaticTriMeshShape (wrapping a new C++ one)
    //------------------------------------------------------------------------------------
    v8::Handle<v8::Object> ATHENA_PHYSICS_SYMBOL createJSStaticTriMeshShape();

    //------------------------------------------------------------------------------------
    /// @brief  Wrap a new JavaScript StaticTriMeshShape around a C++ one
    //------------------------------------------------------------------------------------
    v8::Handle<v8::Value> ATHENA_PHYSICS_SYMBOL toJavaScript(StaticTriMeshShape* pStaticTriMeshShape);


    //------------------------------------------------------------------------------------
    /// @brief  Retrieve the C++ World wrapped by a JavaScript one
    //------------------------------------------------------------------------------------
    ATHENA_PHYSICS_SYMBOL World* fromJSWorld(v8::Handle<v8::Value> value);

    //------------------------------------------------------------------------------------
    /// @brief  Create a new JavaScript World (wrapping a new C++ one)
    //------------------------------------------------------------------------------------
    v8::Handle<v8::Object> ATHENA_PHYSICS_SYMBOL createJSWorld();

    //------------------------------------------------------------------------------------
    /// @brief  Wrap a new JavaScript World around a C++ one
    //------------------------------------------------------------------------------------
    v8::Handle<v8::Value> ATHENA_PHYSICS_SYMBOL toJavaScript(World* pWorld);

}
}

#endif
