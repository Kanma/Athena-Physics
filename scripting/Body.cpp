#include <Athena-Physics/Body.h>
#include <Athena-Physics/Scripting.h>
#include <Athena-Scripting/Utils.h>
#include <Athena-Scripting/ScriptingManager.h>
#include <Athena-Math/Color.h>
#include <Athena-Math/Scripting.h>
#include <v8.h>

using namespace Athena::Physics;
using namespace Athena::Entities;
using namespace Athena::Scripting;
using namespace Athena::Math;
using namespace v8;


/**************************************** MACROS ***************************************/

#define GetPtr(HANDLE) GetObjectPtr<Body>(HANDLE)


/***************************** CONSTRUCTION / DESTRUCTION ******************************/

// Constructor
v8::Handle<Value> Body_New(const Arguments& args)
{
    // New C++ component
    if ((args.Length() == 2) && args[0]->IsString() && args[1]->IsObject())
    {
        ComponentsList* pList = 0;
        GetObjectPtr(args[1], &pList);

        if (pList)
            return SetObjectPtr(args.This(), new Body(*String::AsciiValue(args[0]->ToString()), pList),
                                &NoOpWeakCallback);
    }

    // Wrapper around an existing C++ component
    else if ((args.Length() == 1) && args[0]->IsExternal())
    {
        Body* pComponent = static_cast<Body*>(External::Unwrap(args[0]));
        return SetObjectPtr(args.This(), pComponent, &NoOpWeakCallback);
    }

    return ThrowException(String::New("Invalid parameters, valid syntax:\nBody(name, components_list)\nBody(<C++ body>)"));
}


/************************************** PROPERTIES *************************************/

v8::Handle<Value> Body_IsKinematic(Local<String> property, const AccessorInfo &info)
{
    HandleScope handle_scope;

    Body* ptr = GetPtr(info.This());
    assert(ptr);

    return handle_scope.Close(Boolean::New(ptr->isKinematic()));
}

//-----------------------------------------------------------------------

void Body_SetKinematic(Local<String> property, Local<Value> value, const AccessorInfo& info)
{
    HandleScope handle_scope;

    Body* ptr = GetPtr(info.This());
    assert(ptr);

    if (value->IsBoolean())
        ptr->setKinematic(value->ToBoolean()->Value());
}

//-----------------------------------------------------------------------

v8::Handle<Value> Body_IsStatic(Local<String> property, const AccessorInfo &info)
{
    HandleScope handle_scope;

    Body* ptr = GetPtr(info.This());
    assert(ptr);

    return handle_scope.Close(Boolean::New(ptr->isStatic()));
}

//-----------------------------------------------------------------------

v8::Handle<Value> Body_IsDynamic(Local<String> property, const AccessorInfo &info)
{
    HandleScope handle_scope;

    Body* ptr = GetPtr(info.This());
    assert(ptr);

    return handle_scope.Close(Boolean::New(ptr->isDynamic()));
}

//-----------------------------------------------------------------------

v8::Handle<Value> Body_GetMass(Local<String> property, const AccessorInfo &info)
{
    HandleScope handle_scope;

    Body* ptr = GetPtr(info.This());
    assert(ptr);

    return handle_scope.Close(Number::New(ptr->getMass()));
}

//-----------------------------------------------------------------------

void Body_SetMass(Local<String> property, Local<Value> value, const AccessorInfo& info)
{
    HandleScope handle_scope;

    Body* ptr = GetPtr(info.This());
    assert(ptr);

    if (value->IsNumber())
        ptr->setMass(value->ToNumber()->Value());
}

//-----------------------------------------------------------------------

v8::Handle<Value> Body_GetCollisionShape(Local<String> property, const AccessorInfo &info)
{
    HandleScope handle_scope;

    Body* ptr = GetPtr(info.This());
    assert(ptr);

    return handle_scope.Close(toJavaScript(ptr->getCollisionShape()));
}

//-----------------------------------------------------------------------

void Body_SetCollisionShape(Local<String> property, Local<Value> value, const AccessorInfo& info)
{
    HandleScope handle_scope;

    Body* ptr = GetPtr(info.This());
    assert(ptr);

    ptr->setCollisionShape(fromJSCollisionShape(value));
}

//-----------------------------------------------------------------------

v8::Handle<Value> Body_GetLinearVelocity(Local<String> property, const AccessorInfo &info)
{
    HandleScope handle_scope;

    Body* ptr = GetPtr(info.This());
    assert(ptr);

    return handle_scope.Close(toJavaScript(ptr->getLinearVelocity()));
}

//-----------------------------------------------------------------------

void Body_SetLinearVelocity(Local<String> property, Local<Value> value, const AccessorInfo& info)
{
    HandleScope handle_scope;

    Body* ptr = GetPtr(info.This());
    assert(ptr);

    if (!isJSVector3(value))
    {
        ThrowException(String::New("Invalid value, expected: Vector3"));
        return;
    }

    ptr->setLinearVelocity(fromJSVector3Unsafe(value));
}

//-----------------------------------------------------------------------

void Body_SetWorldLinearVelocity(Local<String> property, Local<Value> value, const AccessorInfo& info)
{
    HandleScope handle_scope;

    Body* ptr = GetPtr(info.This());
    assert(ptr);

    if (!isJSVector3(value))
    {
        ThrowException(String::New("Invalid value, expected: Vector3"));
        return;
    }

    ptr->setWorldLinearVelocity(fromJSVector3Unsafe(value));
}

//-----------------------------------------------------------------------

v8::Handle<Value> Body_GetAngularVelocity(Local<String> property, const AccessorInfo &info)
{
    HandleScope handle_scope;

    Body* ptr = GetPtr(info.This());
    assert(ptr);

    return handle_scope.Close(toJavaScript(ptr->getAngularVelocity()));
}

//-----------------------------------------------------------------------

void Body_SetAngularVelocity(Local<String> property, Local<Value> value, const AccessorInfo& info)
{
    HandleScope handle_scope;

    Body* ptr = GetPtr(info.This());
    assert(ptr);

    if (!isJSVector3(value))
    {
        ThrowException(String::New("Invalid value, expected: Vector3"));
        return;
    }

    ptr->setAngularVelocity(fromJSVector3Unsafe(value));
}

//-----------------------------------------------------------------------

v8::Handle<Value> Body_GetAngularFactor(Local<String> property, const AccessorInfo &info)
{
    HandleScope handle_scope;

    Body* ptr = GetPtr(info.This());
    assert(ptr);

    return handle_scope.Close(toJavaScript(ptr->getAngularFactor()));
}

//-----------------------------------------------------------------------

void Body_SetAngularFactor(Local<String> property, Local<Value> value, const AccessorInfo& info)
{
    HandleScope handle_scope;

    Body* ptr = GetPtr(info.This());
    assert(ptr);

    if (isJSVector3(value))
        ptr->setAngularFactor(fromJSVector3Unsafe(value));
    else if (value->IsNumber())
        ptr->setAngularFactor(value->ToNumber()->Value());
    else
        ThrowException(String::New("Invalid value, expected: Vector3 or number"));
}


/**************************************** METHODS ***************************************/

v8::Handle<v8::Value> Body_EnableDeactivation(const Arguments& args)
{
    HandleScope handle_scope;

    if ((args.Length() == 1) && args[0]->IsBoolean())
    {
        Body* ptr = GetPtr(args.This());
        assert(ptr);

        ptr->enableDeactivation(args[0]->ToBoolean()->Value());

        return v8::Handle<v8::Value>();
    }

    return ThrowException(String::New("Invalid parameters, valid syntax:\nenableDeactivation(boolean)"));
}

//-----------------------------------------------------------------------

v8::Handle<v8::Value> Body_EnableSimulation(const Arguments& args)
{
    HandleScope handle_scope;

    if ((args.Length() == 1) && args[0]->IsBoolean())
    {
        Body* ptr = GetPtr(args.This());
        assert(ptr);

        ptr->enableSimulation(args[0]->ToBoolean()->Value());

        return v8::Handle<v8::Value>();
    }

    return ThrowException(String::New("Invalid parameters, valid syntax:\nenableSimulation(boolean)"));
}

//-----------------------------------------------------------------------

v8::Handle<v8::Value> Body_EnableRotation(const Arguments& args)
{
    HandleScope handle_scope;

    if ((args.Length() == 1) && args[0]->IsBoolean())
    {
        Body* ptr = GetPtr(args.This());
        assert(ptr);

        ptr->enableRotation(args[0]->ToBoolean()->Value());

        return v8::Handle<v8::Value>();
    }

    return ThrowException(String::New("Invalid parameters, valid syntax:\nenableRotation(boolean)"));
}


/************************************ BINDING FUNCTION **********************************/

bool bind_Body(v8::Handle<v8::Object> parent)
{
    ScriptingManager* pManager = ScriptingManager::getSingletonPtr();

    v8::Handle<FunctionTemplate> component = pManager->getClassTemplate("Athena.Physics.Body");

    if (component.IsEmpty())
    {
        assert(!pManager->getClassTemplate("Athena.Physics.CollisionObject").IsEmpty());

        // Declaration of the class
        component = FunctionTemplate::New(Body_New);
        component->InstanceTemplate()->SetInternalFieldCount(1);
        component->Inherit(pManager->getClassTemplate("Athena.Physics.CollisionObject"));

        // Attributes
        AddAttribute(component, "kinematic",           Body_IsKinematic, Body_SetKinematic);
        AddAttribute(component, "static",              Body_IsStatic, 0);
        AddAttribute(component, "dynamic",             Body_IsDynamic, 0);
        AddAttribute(component, "mass",                Body_GetMass, Body_SetMass);
        AddAttribute(component, "collisionShape",      Body_GetCollisionShape, Body_SetCollisionShape);
        AddAttribute(component, "linearVelocity",      Body_GetLinearVelocity, Body_SetLinearVelocity);
        AddAttribute(component, "worldLinearVelocity", Body_GetLinearVelocity, Body_SetWorldLinearVelocity);
        AddAttribute(component, "angularVelocity",     Body_GetAngularVelocity, Body_SetAngularVelocity);
        AddAttribute(component, "angularFactor",       Body_GetAngularFactor, Body_SetAngularFactor);

        // Methods
        AddMethod(component, "enableDeactivation", Body_EnableDeactivation);
        AddMethod(component, "enableSimulation",   Body_EnableSimulation);
        AddMethod(component, "enableRotation",     Body_EnableRotation);

        pManager->declareClassTemplate("Athena.Physics.Body", component);

        parent->Set(String::New("Body_TYPE"), String::New(Athena::Physics::Body::TYPE.c_str()));
    }

    // Add the class to the parent
    return parent->Set(String::New("Body"), component->GetFunction());
}
