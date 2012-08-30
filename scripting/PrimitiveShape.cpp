#include <Athena-Physics/PrimitiveShape.h>
#include <Athena-Physics/Scripting.h>
#include <Athena-Scripting/Utils.h>
#include <Athena-Scripting/ScriptingManager.h>
#include <Athena-Math/Vector3.h>
#include <Athena-Math/Scripting.h>
#include <v8.h>

using namespace Athena::Physics;
using namespace Athena::Entities;
using namespace Athena::Scripting;
using namespace Athena::Math;
using namespace v8;


/**************************************** MACROS ***************************************/

#define GetPtr(HANDLE) GetObjectPtr<PrimitiveShape>(HANDLE)


/***************************** CONSTRUCTION / DESTRUCTION ******************************/

// Constructor
v8::Handle<Value> PrimitiveShape_New(const Arguments& args)
{
    // New C++ component
    if ((args.Length() == 2) && args[0]->IsString() && args[1]->IsObject())
    {
        ComponentsList* pList = 0;
        GetObjectPtr(args[1], &pList);

        if (pList)
            return SetObjectPtr(args.This(), new PrimitiveShape(*String::AsciiValue(args[0]->ToString()), pList),
                                &NoOpWeakCallback);
    }

    // Wrapper around an existing C++ component
    else if ((args.Length() == 1) && args[0]->IsExternal())
    {
        PrimitiveShape* pComponent = static_cast<PrimitiveShape*>(External::Unwrap(args[0]));
        return SetObjectPtr(args.This(), pComponent, &NoOpWeakCallback);
    }

    return ThrowException(String::New("Invalid parameters, valid syntax:\nPrimitiveShape(name, components_list)\nPrimitiveShape(<C++ shape>)"));
}


/************************************** PROPERTIES *************************************/

v8::Handle<Value> PrimitiveShape_GetSize(Local<String> property, const AccessorInfo &info)
{
    HandleScope handle_scope;

    PrimitiveShape* ptr = GetPtr(info.This());
    assert(ptr);

    return handle_scope.Close(toJavaScript(ptr->getSize()));
}

//-----------------------------------------------------------------------

v8::Handle<Value> PrimitiveShape_GetRadius(Local<String> property, const AccessorInfo &info)
{
    HandleScope handle_scope;

    PrimitiveShape* ptr = GetPtr(info.This());
    assert(ptr);

    return handle_scope.Close(Number::New(ptr->getRadius()));
}

//-----------------------------------------------------------------------

v8::Handle<Value> PrimitiveShape_GetHeight(Local<String> property, const AccessorInfo &info)
{
    HandleScope handle_scope;

    PrimitiveShape* ptr = GetPtr(info.This());
    assert(ptr);

    return handle_scope.Close(Number::New(ptr->getHeight()));
}

//-----------------------------------------------------------------------

v8::Handle<Value> PrimitiveShape_GetAxis(Local<String> property, const AccessorInfo &info)
{
    HandleScope handle_scope;

    PrimitiveShape* ptr = GetPtr(info.This());
    assert(ptr);

    return handle_scope.Close(Uint32::New((unsigned int) ptr->getAxis()));
}

//-----------------------------------------------------------------------

v8::Handle<Value> PrimitiveShape_GetShape(Local<String> property, const AccessorInfo &info)
{
    HandleScope handle_scope;

    PrimitiveShape* ptr = GetPtr(info.This());
    assert(ptr);

    return handle_scope.Close(Uint32::New((unsigned int) ptr->getShape()));
}


/**************************************** METHODS ***************************************/

v8::Handle<v8::Value> PrimitiveShape_CreateBox(const Arguments& args)
{
    HandleScope handle_scope;

    if ((args.Length() == 1) && isJSVector3(args[0]))
    {
        PrimitiveShape* ptr = GetPtr(args.This());
        assert(ptr);

        ptr->createBox(fromJSVector3Unsafe(args[0]));

        return v8::Handle<v8::Value>();
    }

    return ThrowException(String::New("Invalid parameters, valid syntax:\ncreateBox(size)"));
}

//-----------------------------------------------------------------------

v8::Handle<v8::Value> PrimitiveShape_CreateCapsule(const Arguments& args)
{
    HandleScope handle_scope;

    if (((args.Length() == 2) || (args.Length() == 3)) && args[0]->IsNumber() && args[1]->IsNumber())
    {
        PrimitiveShape* ptr = GetPtr(args.This());
        assert(ptr);

        PrimitiveShape::tAxis axis = PrimitiveShape::AXIS_Y;
        if ((args.Length() == 3) && args[2]->IsUint32())
        {
            unsigned int iAxis = args[2]->ToUint32()->Value();
            if (iAxis <= PrimitiveShape::AXIS_Z)
                axis = (PrimitiveShape::tAxis) iAxis;
        }

        ptr->createCapsule(args[0]->ToNumber()->Value(), args[1]->ToNumber()->Value(), axis);

        return v8::Handle<v8::Value>();
    }

    return ThrowException(String::New("Invalid parameters, valid syntax:\ncreateCapsule(radius, height [, axis])"));
}

//-----------------------------------------------------------------------

v8::Handle<v8::Value> PrimitiveShape_CreateCone(const Arguments& args)
{
    HandleScope handle_scope;

    if (((args.Length() == 2) || (args.Length() == 3)) && args[0]->IsNumber() && args[1]->IsNumber())
    {
        PrimitiveShape* ptr = GetPtr(args.This());
        assert(ptr);

        PrimitiveShape::tAxis axis = PrimitiveShape::AXIS_Y;
        if ((args.Length() == 3) && args[2]->IsUint32())
        {
            unsigned int iAxis = args[2]->ToUint32()->Value();
            if (iAxis <= PrimitiveShape::AXIS_Z)
                axis = (PrimitiveShape::tAxis) iAxis;
        }

        ptr->createCone(args[0]->ToNumber()->Value(), args[1]->ToNumber()->Value(), axis);

        return v8::Handle<v8::Value>();
    }

    return ThrowException(String::New("Invalid parameters, valid syntax:\ncreateCone(radius, height [, axis])"));
}

//-----------------------------------------------------------------------

v8::Handle<v8::Value> PrimitiveShape_CreateCylinder(const Arguments& args)
{
    HandleScope handle_scope;

    if (((args.Length() == 2) || (args.Length() == 3)) && args[0]->IsNumber() && args[1]->IsNumber())
    {
        PrimitiveShape* ptr = GetPtr(args.This());
        assert(ptr);

        PrimitiveShape::tAxis axis = PrimitiveShape::AXIS_Y;
        if ((args.Length() == 3) && args[2]->IsUint32())
        {
            unsigned int iAxis = args[2]->ToUint32()->Value();
            if (iAxis <= PrimitiveShape::AXIS_Z)
                axis = (PrimitiveShape::tAxis) iAxis;
        }

        ptr->createCylinder(args[0]->ToNumber()->Value(), args[1]->ToNumber()->Value(), axis);

        return v8::Handle<v8::Value>();
    }

    return ThrowException(String::New("Invalid parameters, valid syntax:\ncreateCylinder(radius, height [, axis])"));
}

//-----------------------------------------------------------------------

v8::Handle<v8::Value> PrimitiveShape_CreateSphere(const Arguments& args)
{
    HandleScope handle_scope;

    if ((args.Length() == 1) && args[0]->IsNumber())
    {
        PrimitiveShape* ptr = GetPtr(args.This());
        assert(ptr);

        ptr->createSphere(args[0]->ToNumber()->Value());

        return v8::Handle<v8::Value>();
    }

    return ThrowException(String::New("Invalid parameters, valid syntax:\ncreateSphere(radius)"));
}


/************************************ BINDING FUNCTION **********************************/

bool bind_PrimitiveShape(v8::Handle<v8::Object> parent)
{
    ScriptingManager* pManager = ScriptingManager::getSingletonPtr();

    v8::Handle<FunctionTemplate> component = pManager->getClassTemplate("Athena.Physics.PrimitiveShape");

    if (component.IsEmpty())
    {
        assert(!pManager->getClassTemplate("Athena.Physics.PhysicalComponent").IsEmpty());

        // Declaration of the class
        component = FunctionTemplate::New(PrimitiveShape_New);
        component->InstanceTemplate()->SetInternalFieldCount(1);
        component->Inherit(pManager->getClassTemplate("Athena.Physics.PhysicalComponent"));

        // Attributes
        AddAttribute(component, "size",   PrimitiveShape_GetSize, 0);
        AddAttribute(component, "radius", PrimitiveShape_GetRadius, 0);
        AddAttribute(component, "height", PrimitiveShape_GetHeight, 0);
        AddAttribute(component, "axis",   PrimitiveShape_GetAxis, 0);
        AddAttribute(component, "shape",  PrimitiveShape_GetShape, 0);

        // Methods
        AddMethod(component, "createBox",      PrimitiveShape_CreateBox);
        AddMethod(component, "createCapsule",  PrimitiveShape_CreateCapsule);
        AddMethod(component, "createCone",     PrimitiveShape_CreateCone);
        AddMethod(component, "createCylinder", PrimitiveShape_CreateCylinder);
        AddMethod(component, "createSphere",   PrimitiveShape_CreateSphere);

        pManager->declareClassTemplate("Athena.Physics.PrimitiveShape", component);

        parent->Set(String::New("PrimitiveShape_TYPE"), String::New(Athena::Physics::PrimitiveShape::TYPE.c_str()));
        parent->Set(String::New("SHAPE_BOX"),           Uint32::New(Athena::Physics::PrimitiveShape::SHAPE_BOX));
        parent->Set(String::New("SHAPE_CAPSULE"),       Uint32::New(Athena::Physics::PrimitiveShape::SHAPE_CAPSULE));
        parent->Set(String::New("SHAPE_CONE"),          Uint32::New(Athena::Physics::PrimitiveShape::SHAPE_CONE));
        parent->Set(String::New("SHAPE_CYLINDER"),      Uint32::New(Athena::Physics::PrimitiveShape::SHAPE_CYLINDER));
        parent->Set(String::New("SHAPE_SPHERE"),        Uint32::New(Athena::Physics::PrimitiveShape::SHAPE_SPHERE));
        parent->Set(String::New("AXIS_X"),              Uint32::New(Athena::Physics::PrimitiveShape::AXIS_X));
        parent->Set(String::New("AXIS_Y"),              Uint32::New(Athena::Physics::PrimitiveShape::AXIS_Y));
        parent->Set(String::New("AXIS_Z"),              Uint32::New(Athena::Physics::PrimitiveShape::AXIS_Z));
    }

    // Add the class to the parent
    return parent->Set(String::New("PrimitiveShape"), component->GetFunction());
}
