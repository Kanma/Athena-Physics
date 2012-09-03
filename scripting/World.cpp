#include <Athena-Physics/World.h>
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

#define GetPtr(HANDLE) GetObjectPtr<World>(HANDLE)


/***************************** CONSTRUCTION / DESTRUCTION ******************************/

// Constructor
v8::Handle<Value> World_New(const Arguments& args)
{
    // New C++ component
    if ((args.Length() == 2) && args[0]->IsString() && args[1]->IsObject())
    {
        ComponentsList* pList = 0;
        GetObjectPtr(args[1], &pList);

        if (pList)
            return SetObjectPtr(args.This(), new World(*String::AsciiValue(args[0]->ToString()), pList),
                                &NoOpWeakCallback);
    }

    // Wrapper around an existing C++ component
    else if ((args.Length() == 1) && args[0]->IsExternal())
    {
        World* pComponent = static_cast<World*>(External::Unwrap(args[0]));
        return SetObjectPtr(args.This(), pComponent, &NoOpWeakCallback);
    }

    return ThrowException(String::New("Invalid parameters, valid syntax:\nWorld(name, components_list)\nWorld(<C++ world>)"));
}


/************************************** PROPERTIES *************************************/

v8::Handle<Value> World_GetWorldType(Local<String> property, const AccessorInfo &info)
{
    HandleScope handle_scope;

    World* ptr = GetPtr(info.This());
    assert(ptr);

    return handle_scope.Close(Uint32::New(ptr->getWorldType()));
}

//-----------------------------------------------------------------------

void World_SetWorldType(Local<String> property, Local<Value> value, const AccessorInfo& info)
{
    HandleScope handle_scope;

    World* ptr = GetPtr(info.This());
    assert(ptr);

    int type = value->ToInt32()->Value();
    if ((type == World::WORLD_RIGID_BODY) || (type == World::WORLD_SOFT_BODY))
        ptr->setWorldType((World::tType) type);
}

//-----------------------------------------------------------------------

v8::Handle<Value> World_GetGravity(Local<String> property, const AccessorInfo &info)
{
    HandleScope handle_scope;

    World* ptr = GetPtr(info.This());
    assert(ptr);

    return handle_scope.Close(toJavaScript(ptr->getGravity()));
}

//-----------------------------------------------------------------------

void World_SetGravity(Local<String> property, Local<Value> value, const AccessorInfo& info)
{
    HandleScope handle_scope;

    World* ptr = GetPtr(info.This());
    assert(ptr);

    ptr->setGravity(fromJSVector3(value));
}


/************************************ BINDING FUNCTION **********************************/

bool bind_World(v8::Handle<v8::Object> parent)
{
    ScriptingManager* pManager = ScriptingManager::getSingletonPtr();

    v8::Handle<FunctionTemplate> component = pManager->getClassTemplate("Athena.Physics.World");

    if (component.IsEmpty())
    {
        assert(!pManager->getClassTemplate("Athena.Physics.PhysicalComponent").IsEmpty());

        // Declaration of the class
        component = FunctionTemplate::New(World_New);
        component->InstanceTemplate()->SetInternalFieldCount(1);
        component->Inherit(pManager->getClassTemplate("Athena.Physics.PhysicalComponent"));

        // Attributes
        AddAttribute(component, "worldType", World_GetWorldType, World_SetWorldType);
        AddAttribute(component, "gravity",   World_GetGravity, World_SetGravity);

        pManager->declareClassTemplate("Athena.Physics.World", component);

        parent->Set(String::New("World_TYPE"),         String::New(Athena::Physics::World::TYPE.c_str()));
        parent->Set(String::New("World_DEFAULT_NAME"), String::New(Athena::Physics::World::DEFAULT_NAME.c_str()));
        parent->Set(String::New("World_RIGID_BODY"),   Uint32::New(Athena::Physics::World::WORLD_RIGID_BODY));
        parent->Set(String::New("World_SOFT_BODY"),    Uint32::New(Athena::Physics::World::WORLD_SOFT_BODY));
    }

    // Add the class to the parent
    return parent->Set(String::New("World"), component->GetFunction());
}
