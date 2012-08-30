#include <Athena-Physics/CompoundShape.h>
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

#define GetPtr(HANDLE) GetObjectPtr<CompoundShape>(HANDLE)


/***************************** CONSTRUCTION / DESTRUCTION ******************************/

// Constructor
v8::Handle<Value> CompoundShape_New(const Arguments& args)
{
    // New C++ component
    if ((args.Length() == 2) && args[0]->IsString() && args[1]->IsObject())
    {
        ComponentsList* pList = 0;
        GetObjectPtr(args[1], &pList);

        if (pList)
            return SetObjectPtr(args.This(), new CompoundShape(*String::AsciiValue(args[0]->ToString()), pList),
                                &NoOpWeakCallback);
    }

    // Wrapper around an existing C++ component
    else if ((args.Length() == 1) && args[0]->IsExternal())
    {
        CompoundShape* pComponent = static_cast<CompoundShape*>(External::Unwrap(args[0]));
        return SetObjectPtr(args.This(), pComponent, &NoOpWeakCallback);
    }

    return ThrowException(String::New("Invalid parameters, valid syntax:\nCompoundShape(name, components_list)\nCompoundShape(<C++ shape>)"));
}


/************************************** PROPERTIES *************************************/

v8::Handle<Value> CompoundShape_GetNbChildShapes(Local<String> property, const AccessorInfo &info)
{
    HandleScope handle_scope;

    CompoundShape* ptr = GetPtr(info.This());
    assert(ptr);

    return handle_scope.Close(Uint32::New(ptr->getNbChildShapes()));
}


/**************************************** METHODS ***************************************/

v8::Handle<v8::Value> CompoundShape_AddBox(const Arguments& args)
{
    HandleScope handle_scope;

    if ((args.Length() >= 1) && (args.Length() <= 3) && isJSVector3(args[0]))
    {
        CompoundShape* ptr = GetPtr(args.This());
        assert(ptr);

        Vector3 position = Vector3::ZERO;
        Quaternion orientation = Quaternion::IDENTITY;

        if ((args.Length() >= 2) && isJSVector3(args[1]))
        {
            position = fromJSVector3Unsafe(args[1]);

            if ((args.Length() == 3) && isJSQuaternion(args[2]))
                orientation = fromJSQuaternionUnsafe(args[2]);
        }

        ptr->addBox(fromJSVector3Unsafe(args[0]), position, orientation);

        return v8::Handle<v8::Value>();
    }

    return ThrowException(String::New("Invalid parameters, valid syntax:\naddBox(size [, position, orientation])"));
}

//-----------------------------------------------------------------------

v8::Handle<v8::Value> CompoundShape_AddCapsule(const Arguments& args)
{
    HandleScope handle_scope;

    if (((args.Length() >= 2) || (args.Length() <= 5)) && args[0]->IsNumber() && args[1]->IsNumber())
    {
        CompoundShape* ptr = GetPtr(args.This());
        assert(ptr);

        PrimitiveShape::tAxis axis  = PrimitiveShape::AXIS_Y;
        Vector3 position            = Vector3::ZERO;
        Quaternion orientation      = Quaternion::IDENTITY;

        if ((args.Length() >= 3) && args[2]->IsUint32())
        {
            unsigned int iAxis = args[2]->ToUint32()->Value();
            if (iAxis <= PrimitiveShape::AXIS_Z)
                axis = (PrimitiveShape::tAxis) iAxis;

            if ((args.Length() >= 4) && isJSVector3(args[3]))
            {
                position = fromJSVector3Unsafe(args[3]);

                if ((args.Length() == 5) && isJSQuaternion(args[4]))
                    orientation = fromJSQuaternionUnsafe(args[4]);
            }
        }

        ptr->addCapsule(args[0]->ToNumber()->Value(), args[1]->ToNumber()->Value(),
                        axis, position, orientation);

        return v8::Handle<v8::Value>();
    }

    return ThrowException(String::New("Invalid parameters, valid syntax:\naddCapsule(radius, height [, axis, position, orientation])"));
}

//-----------------------------------------------------------------------

v8::Handle<v8::Value> CompoundShape_AddCone(const Arguments& args)
{
    HandleScope handle_scope;

    if (((args.Length() >= 2) || (args.Length() <= 5)) && args[0]->IsNumber() && args[1]->IsNumber())
    {
        CompoundShape* ptr = GetPtr(args.This());
        assert(ptr);

        PrimitiveShape::tAxis axis  = PrimitiveShape::AXIS_Y;
        Vector3 position            = Vector3::ZERO;
        Quaternion orientation      = Quaternion::IDENTITY;

        if ((args.Length() >= 3) && args[2]->IsUint32())
        {
            unsigned int iAxis = args[2]->ToUint32()->Value();
            if (iAxis <= PrimitiveShape::AXIS_Z)
                axis = (PrimitiveShape::tAxis) iAxis;

            if ((args.Length() >= 4) && isJSVector3(args[3]))
            {
                position = fromJSVector3Unsafe(args[3]);

                if ((args.Length() == 5) && isJSQuaternion(args[4]))
                    orientation = fromJSQuaternionUnsafe(args[4]);
            }
        }

        ptr->addCone(args[0]->ToNumber()->Value(), args[1]->ToNumber()->Value(),
                     axis, position, orientation);

        return v8::Handle<v8::Value>();
    }

    return ThrowException(String::New("Invalid parameters, valid syntax:\naddCone(radius, height [, axis, position, orientation])"));
}

//-----------------------------------------------------------------------

v8::Handle<v8::Value> CompoundShape_AddCylinder(const Arguments& args)
{
    HandleScope handle_scope;

    if (((args.Length() >= 2) || (args.Length() <= 5)) && args[0]->IsNumber() && args[1]->IsNumber())
    {
        CompoundShape* ptr = GetPtr(args.This());
        assert(ptr);

        PrimitiveShape::tAxis axis  = PrimitiveShape::AXIS_Y;
        Vector3 position            = Vector3::ZERO;
        Quaternion orientation      = Quaternion::IDENTITY;

        if ((args.Length() >= 3) && args[2]->IsUint32())
        {
            unsigned int iAxis = args[2]->ToUint32()->Value();
            if (iAxis <= PrimitiveShape::AXIS_Z)
                axis = (PrimitiveShape::tAxis) iAxis;

            if ((args.Length() >= 4) && isJSVector3(args[3]))
            {
                position = fromJSVector3Unsafe(args[3]);

                if ((args.Length() == 5) && isJSQuaternion(args[4]))
                    orientation = fromJSQuaternionUnsafe(args[4]);
            }
        }

        ptr->addCylinder(args[0]->ToNumber()->Value(), args[1]->ToNumber()->Value(),
                         axis, position, orientation);

        return v8::Handle<v8::Value>();
    }

    return ThrowException(String::New("Invalid parameters, valid syntax:\naddCylinder(radius, height [, axis, position, orientation])"));
}

//-----------------------------------------------------------------------

v8::Handle<v8::Value> CompoundShape_AddSphere(const Arguments& args)
{
    HandleScope handle_scope;

    if ((args.Length() >= 1) && (args.Length() <= 3) && args[0]->IsNumber())
    {
        CompoundShape* ptr = GetPtr(args.This());
        assert(ptr);

        Vector3 position = Vector3::ZERO;
        Quaternion orientation = Quaternion::IDENTITY;

        if ((args.Length() >= 2) && isJSVector3(args[1]))
        {
            position = fromJSVector3Unsafe(args[1]);

            if ((args.Length() == 3) && isJSQuaternion(args[2]))
                orientation = fromJSQuaternionUnsafe(args[2]);
        }

        ptr->addSphere(args[0]->ToNumber()->Value(), position, orientation);

        return v8::Handle<v8::Value>();
    }

    return ThrowException(String::New("Invalid parameters, valid syntax:\naddSphere(radius [, position, orientation])"));
}

//-----------------------------------------------------------------------

v8::Handle<v8::Value> CompoundShape_RemoveChildShape(const Arguments& args)
{
    HandleScope handle_scope;

    if ((args.Length() == 1) && args[0]->IsUint32())
    {
        CompoundShape* ptr = GetPtr(args.This());
        assert(ptr);

        unsigned int index = args[0]->ToUint32()->Value();
        if (index < ptr->getNbChildShapes())
        {
            ptr->removeChildShape(index);
            return v8::Handle<v8::Value>();
        }
    }

    return ThrowException(String::New("Invalid parameters, valid syntax:\nremoveChildShape(childIndex)"));
}

//-----------------------------------------------------------------------

v8::Handle<v8::Value> CompoundShape_GetChildSize(const Arguments& args)
{
    HandleScope handle_scope;

    if ((args.Length() == 1) && args[0]->IsUint32())
    {
        CompoundShape* ptr = GetPtr(args.This());
        assert(ptr);

        unsigned int index = args[0]->ToUint32()->Value();
        if (index < ptr->getNbChildShapes())
            return handle_scope.Close(toJavaScript(ptr->getChildSize(index)));
    }

    return ThrowException(String::New("Invalid parameters, valid syntax:\ngetChildSize(childIndex)"));
}

//-----------------------------------------------------------------------

v8::Handle<v8::Value> CompoundShape_GetChildRadius(const Arguments& args)
{
    HandleScope handle_scope;

    if ((args.Length() == 1) && args[0]->IsUint32())
    {
        CompoundShape* ptr = GetPtr(args.This());
        assert(ptr);

        unsigned int index = args[0]->ToUint32()->Value();
        if (index < ptr->getNbChildShapes())
            return handle_scope.Close(Number::New(ptr->getChildRadius(index)));
    }

    return ThrowException(String::New("Invalid parameters, valid syntax:\ngetChildRadius(childIndex)"));
}

//-----------------------------------------------------------------------

v8::Handle<v8::Value> CompoundShape_GetChildHeight(const Arguments& args)
{
    HandleScope handle_scope;

    if ((args.Length() == 1) && args[0]->IsUint32())
    {
        CompoundShape* ptr = GetPtr(args.This());
        assert(ptr);

        unsigned int index = args[0]->ToUint32()->Value();
        if (index < ptr->getNbChildShapes())
            return handle_scope.Close(Number::New(ptr->getChildHeight(index)));
    }

    return ThrowException(String::New("Invalid parameters, valid syntax:\ngetChildHeight(childIndex)"));
}

//-----------------------------------------------------------------------

v8::Handle<v8::Value> CompoundShape_GetChildAxis(const Arguments& args)
{
    HandleScope handle_scope;

    if ((args.Length() == 1) && args[0]->IsUint32())
    {
        CompoundShape* ptr = GetPtr(args.This());
        assert(ptr);

        unsigned int index = args[0]->ToUint32()->Value();
        if (index < ptr->getNbChildShapes())
            return handle_scope.Close(Uint32::New((unsigned int) ptr->getChildAxis(index)));
    }

    return ThrowException(String::New("Invalid parameters, valid syntax:\ngetChildAxis(childIndex)"));
}

//-----------------------------------------------------------------------

v8::Handle<v8::Value> CompoundShape_GetChildShape(const Arguments& args)
{
    HandleScope handle_scope;

    if ((args.Length() == 1) && args[0]->IsUint32())
    {
        CompoundShape* ptr = GetPtr(args.This());
        assert(ptr);

        unsigned int index = args[0]->ToUint32()->Value();
        if (index < ptr->getNbChildShapes())
            return handle_scope.Close(Uint32::New((unsigned int) ptr->getChildShape(index)));
    }

    return ThrowException(String::New("Invalid parameters, valid syntax:\ngetChildShape(childIndex)"));
}

//-----------------------------------------------------------------------

v8::Handle<v8::Value> CompoundShape_GetChildTransforms(const Arguments& args)
{
    HandleScope handle_scope;

    if ((args.Length() == 1) && args[0]->IsUint32())
    {
        CompoundShape* ptr = GetPtr(args.This());
        assert(ptr);

        unsigned int index = args[0]->ToUint32()->Value();
        if (index < ptr->getNbChildShapes())
        {
            Vector3 position;
            Quaternion orientation;

            ptr->getChildTransforms(index, position, orientation);

            Handle<Object> jsResponse = Object::New();
            jsResponse->Set(String::New("position"), toJavaScript(position));
            jsResponse->Set(String::New("orientation"), toJavaScript(orientation));

            return handle_scope.Close(jsResponse);
        }
    }

    return ThrowException(String::New("Invalid parameters, valid syntax:\ngetChildTransforms(childIndex)"));
}


/************************************ BINDING FUNCTION **********************************/

bool bind_CompoundShape(v8::Handle<v8::Object> parent)
{
    ScriptingManager* pManager = ScriptingManager::getSingletonPtr();

    v8::Handle<FunctionTemplate> component = pManager->getClassTemplate("Athena.Physics.CompoundShape");

    if (component.IsEmpty())
    {
        assert(!pManager->getClassTemplate("Athena.Physics.PhysicalComponent").IsEmpty());

        // Declaration of the class
        component = FunctionTemplate::New(CompoundShape_New);
        component->InstanceTemplate()->SetInternalFieldCount(1);
        component->Inherit(pManager->getClassTemplate("Athena.Physics.PhysicalComponent"));

        // Attributes
        AddAttribute(component, "nbChildShapes",    CompoundShape_GetNbChildShapes, 0);

        // Methods
        AddMethod(component, "addBox",              CompoundShape_AddBox);
        AddMethod(component, "addCapsule",          CompoundShape_AddCapsule);
        AddMethod(component, "addCone",             CompoundShape_AddCone);
        AddMethod(component, "addCylinder",         CompoundShape_AddCylinder);
        AddMethod(component, "addSphere",           CompoundShape_AddSphere);
        AddMethod(component, "removeChildShape",    CompoundShape_RemoveChildShape);
        AddMethod(component, "getChildSize",        CompoundShape_GetChildSize);
        AddMethod(component, "getChildRadius",      CompoundShape_GetChildRadius);
        AddMethod(component, "getChildHeight",      CompoundShape_GetChildHeight);
        AddMethod(component, "getChildAxis",        CompoundShape_GetChildAxis);
        AddMethod(component, "getChildShape",       CompoundShape_GetChildShape);
        AddMethod(component, "getChildTransforms",  CompoundShape_GetChildTransforms);

        pManager->declareClassTemplate("Athena.Physics.CompoundShape", component);

        parent->Set(String::New("CompoundShape_TYPE"), String::New(Athena::Physics::CompoundShape::TYPE.c_str()));
    }

    // Add the class to the parent
    return parent->Set(String::New("CompoundShape"), component->GetFunction());
}
