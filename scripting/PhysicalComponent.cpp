#include <Athena-Physics/PhysicalComponent.h>
#include <Athena-Physics/Scripting.h>
#include <Athena-Scripting/Utils.h>
#include <Athena-Scripting/ScriptingManager.h>
#include <v8.h>

using namespace Athena::Physics;
using namespace Athena::Entities;
using namespace Athena::Scripting;
using namespace v8;


/**************************************** MACROS ***************************************/

#define GetPtr(HANDLE) GetObjectPtr<PhysicalComponent>(HANDLE)


/***************************** CONSTRUCTION / DESTRUCTION ******************************/

// Constructor
v8::Handle<Value> PhysicalComponent_New(const Arguments& args)
{
    // New C++ component
    if ((args.Length() == 2) && args[0]->IsString() && args[1]->IsObject())
    {
        ComponentsList* pList = 0;
        GetObjectPtr(args[1], &pList);

        if (pList)
            return SetObjectPtr(args.This(), new PhysicalComponent(*String::AsciiValue(args[0]->ToString()), pList),
                                &NoOpWeakCallback);
    }

    // Wrapper around an existing C++ component
    else if ((args.Length() == 1) && args[0]->IsExternal())
    {
        PhysicalComponent* pComponent = static_cast<PhysicalComponent*>(External::Unwrap(args[0]));
        return SetObjectPtr(args.This(), pComponent, &NoOpWeakCallback);
    }

    return ThrowException(String::New("Invalid parameters, valid syntax:\nPhysicalComponent(name, components_list)\nPhysicalComponent(<C++ component>)"));
}


/************************************** PROPERTIES *************************************/

v8::Handle<Value> PhysicalComponent_GetWorld(Local<String> property, const AccessorInfo &info)
{
    HandleScope handle_scope;

    PhysicalComponent* ptr = GetPtr(info.This());
    assert(ptr);

    return handle_scope.Close(toJavaScript(ptr->getWorld()));
}


/************************************ BINDING FUNCTION **********************************/

bool bind_PhysicalComponent(v8::Handle<v8::Object> parent)
{
    ScriptingManager* pManager = ScriptingManager::getSingletonPtr();

    v8::Handle<FunctionTemplate> component = pManager->getClassTemplate("Athena.Physics.PhysicalComponent");

    if (component.IsEmpty())
    {
        assert(!pManager->getClassTemplate("Athena.Entities.Component").IsEmpty());

        // Declaration of the class
        component = FunctionTemplate::New(PhysicalComponent_New);
        component->InstanceTemplate()->SetInternalFieldCount(1);
        component->Inherit(pManager->getClassTemplate("Athena.Entities.Component"));

        // Attributes
        AddAttribute(component, "world", PhysicalComponent_GetWorld, 0);

        pManager->declareClassTemplate("Athena.Physics.PhysicalComponent", component);

        parent->Set(String::New("PhysicalComponent_TYPE"), String::New(Athena::Physics::PhysicalComponent::TYPE.c_str()));
    }

    // Add the class to the parent
    return parent->Set(String::New("PhysicalComponent"), component->GetFunction());
}
