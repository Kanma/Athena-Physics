#include <Athena-Physics/GhostObject.h>
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

#define GetPtr(HANDLE) GetObjectPtr<GhostObject>(HANDLE)


/***************************** CONSTRUCTION / DESTRUCTION ******************************/

// Constructor
v8::Handle<Value> GhostObject_New(const Arguments& args)
{
    // New C++ component
    if ((args.Length() == 2) && args[0]->IsString() && args[1]->IsObject())
    {
        ComponentsList* pList = 0;
        GetObjectPtr(args[1], &pList);

        if (pList)
            return SetObjectPtr(args.This(), new GhostObject(*String::AsciiValue(args[0]->ToString()), pList),
                                &NoOpWeakCallback);
    }

    // Wrapper around an existing C++ component
    else if ((args.Length() == 1) && args[0]->IsExternal())
    {
        GhostObject* pComponent = static_cast<GhostObject*>(External::Unwrap(args[0]));
        return SetObjectPtr(args.This(), pComponent, &NoOpWeakCallback);
    }

    return ThrowException(String::New("Invalid parameters, valid syntax:\nGhostObject(name, components_list)\nGhostObject(<C++ ghost object>)"));
}


/************************************** PROPERTIES *************************************/

v8::Handle<Value> GhostObject_GetCollisionShape(Local<String> property, const AccessorInfo &info)
{
    HandleScope handle_scope;

    GhostObject* ptr = GetPtr(info.This());
    assert(ptr);

    return handle_scope.Close(toJavaScript(ptr->getCollisionShape()));
}

//-----------------------------------------------------------------------

void GhostObject_SetCollisionShape(Local<String> property, Local<Value> value, const AccessorInfo& info)
{
    HandleScope handle_scope;

    GhostObject* ptr = GetPtr(info.This());
    assert(ptr);

    ptr->setCollisionShape(fromJSCollisionShape(value));
}

//-----------------------------------------------------------------------

v8::Handle<Value> GhostObject_GetNbOverlappingObjects(Local<String> property, const AccessorInfo &info)
{
    HandleScope handle_scope;

    GhostObject* ptr = GetPtr(info.This());
    assert(ptr);

    return handle_scope.Close(Uint32::New(ptr->getNbOverlappingObjects()));
}


/**************************************** METHODS ***************************************/

v8::Handle<v8::Value> GhostObject_GetOverlappingObject(const Arguments& args)
{
    HandleScope handle_scope;

    if ((args.Length() == 1) && args[0]->IsUint32())
    {
        GhostObject* ptr = GetPtr(args.This());
        assert(ptr);

        return toJavaScript(ptr->getOverlappingObject(args[0]->ToUint32()->Value()));

    }

    return ThrowException(String::New("Invalid parameters, valid syntax:\ngetOverlappingObject(index)"));
}


/************************************ BINDING FUNCTION **********************************/

bool bind_GhostObject(v8::Handle<v8::Object> parent)
{
    ScriptingManager* pManager = ScriptingManager::getSingletonPtr();

    v8::Handle<FunctionTemplate> component = pManager->getClassTemplate("Athena.Physics.GhostObject");

    if (component.IsEmpty())
    {
        assert(!pManager->getClassTemplate("Athena.Physics.CollisionObject").IsEmpty());

        // Declaration of the class
        component = FunctionTemplate::New(GhostObject_New);
        component->InstanceTemplate()->SetInternalFieldCount(1);
        component->Inherit(pManager->getClassTemplate("Athena.Physics.CollisionObject"));

        // Attributes
        AddAttribute(component, "collisionShape",       GhostObject_GetCollisionShape, GhostObject_SetCollisionShape);
        AddAttribute(component, "nbOverlappingObjects", GhostObject_GetNbOverlappingObjects, 0);

        // Methods
        AddMethod(component, "getOverlappingObject", GhostObject_GetOverlappingObject);

        pManager->declareClassTemplate("Athena.Physics.GhostObject", component);

        parent->Set(String::New("GhostObject_TYPE"), String::New(Athena::Physics::GhostObject::TYPE.c_str()));
    }

    // Add the class to the parent
    return parent->Set(String::New("GhostObject"), component->GetFunction());
}
