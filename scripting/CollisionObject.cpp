#include <Athena-Physics/CollisionObject.h>
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

#define GetPtr(HANDLE) GetObjectPtr<CollisionObject>(HANDLE)


/***************************** CONSTRUCTION / DESTRUCTION ******************************/

// Constructor
v8::Handle<Value> CollisionObject_New(const Arguments& args)
{
    // New C++ component
    if ((args.Length() == 2) && args[0]->IsString() && args[1]->IsObject())
    {
        ComponentsList* pList = 0;
        GetObjectPtr(args[1], &pList);

        if (pList)
            return SetObjectPtr(args.This(), new CollisionObject(*String::AsciiValue(args[0]->ToString()), pList),
                                &NoOpWeakCallback);
    }

    // Wrapper around an existing C++ component
    else if ((args.Length() == 1) && args[0]->IsExternal())
    {
        CollisionObject* pComponent = static_cast<CollisionObject*>(External::Unwrap(args[0]));
        return SetObjectPtr(args.This(), pComponent, &NoOpWeakCallback);
    }

    return ThrowException(String::New("Invalid parameters, valid syntax:\nCollisionObject(name, components_list)\nCollisionObject(<C++ collision object>)"));
}


/************************************** PROPERTIES *************************************/

v8::Handle<Value> CollisionObject_GetCollisionGroup(Local<String> property, const AccessorInfo &info)
{
    HandleScope handle_scope;

    CollisionObject* ptr = GetPtr(info.This());
    assert(ptr);

    return handle_scope.Close(Uint32::New(ptr->getCollisionGroup()));
}

//-----------------------------------------------------------------------

void CollisionObject_SetCollisionGroup(Local<String> property, Local<Value> value, const AccessorInfo& info)
{
    HandleScope handle_scope;

    CollisionObject* ptr = GetPtr(info.This());
    assert(ptr);

    unsigned int group = value->ToUint32()->Value();
    if (group <= 31)
        ptr->setCollisionGroup((tCollisionGroup) group);
}


/************************************ BINDING FUNCTION **********************************/

bool bind_CollisionObject(v8::Handle<v8::Object> parent)
{
    ScriptingManager* pManager = ScriptingManager::getSingletonPtr();

    v8::Handle<FunctionTemplate> component = pManager->getClassTemplate("Athena.Physics.CollisionObject");

    if (component.IsEmpty())
    {
        assert(!pManager->getClassTemplate("Athena.Physics.PhysicalComponent").IsEmpty());

        // Declaration of the class
        component = FunctionTemplate::New(CollisionObject_New);
        component->InstanceTemplate()->SetInternalFieldCount(1);
        component->Inherit(pManager->getClassTemplate("Athena.Physics.PhysicalComponent"));

        // Attributes
        AddAttribute(component, "collisionGroup", CollisionObject_GetCollisionGroup, CollisionObject_SetCollisionGroup);

        pManager->declareClassTemplate("Athena.Physics.CollisionObject", component);

        parent->Set(String::New("CollisionObject_TYPE"), String::New(Athena::Physics::CollisionObject::TYPE.c_str()));
    }

    // Add the class to the parent
    return parent->Set(String::New("CollisionObject"), component->GetFunction());
}
