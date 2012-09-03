#include <Athena-Physics/CollisionShape.h>
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

#define GetPtr(HANDLE) GetObjectPtr<CollisionShape>(HANDLE)


/***************************** CONSTRUCTION / DESTRUCTION ******************************/

// Constructor
v8::Handle<Value> CollisionShape_New(const Arguments& args)
{
    // New C++ component
    if ((args.Length() == 2) && args[0]->IsString() && args[1]->IsObject())
    {
        ComponentsList* pList = 0;
        GetObjectPtr(args[1], &pList);

        if (pList)
            return SetObjectPtr(args.This(), new CollisionShape(*String::AsciiValue(args[0]->ToString()), pList),
                                &NoOpWeakCallback);
    }

    // Wrapper around an existing C++ component
    else if ((args.Length() == 1) && args[0]->IsExternal())
    {
        CollisionShape* pComponent = static_cast<CollisionShape*>(External::Unwrap(args[0]));
        return SetObjectPtr(args.This(), pComponent, &NoOpWeakCallback);
    }

    return ThrowException(String::New("Invalid parameters, valid syntax:\nCollisionShape(name, components_list)\nCollisionShape(<C++ shape>)"));
}


/************************************ BINDING FUNCTION **********************************/

bool bind_CollisionShape(v8::Handle<v8::Object> parent)
{
    ScriptingManager* pManager = ScriptingManager::getSingletonPtr();

    v8::Handle<FunctionTemplate> component = pManager->getClassTemplate("Athena.Physics.CollisionShape");

    if (component.IsEmpty())
    {
        assert(!pManager->getClassTemplate("Athena.Physics.PhysicalComponent").IsEmpty());

        // Declaration of the class
        component = FunctionTemplate::New(CollisionShape_New);
        component->InstanceTemplate()->SetInternalFieldCount(1);
        component->Inherit(pManager->getClassTemplate("Athena.Physics.PhysicalComponent"));

        pManager->declareClassTemplate("Athena.Physics.CollisionShape", component);

        parent->Set(String::New("CollisionShape_TYPE"), String::New(Athena::Physics::CollisionShape::TYPE.c_str()));
    }

    // Add the class to the parent
    return parent->Set(String::New("CollisionShape"), component->GetFunction());
}
