/** @file   Scripting.cpp
    @author Philip Abbet

    Implementation of the scripting-related functions of the Athena-Physics module
*/

#include <Athena-Physics/Scripting.h>
#include <Athena-Scripting/ScriptingManager.h>
#include <Athena-Scripting/Utils.h>
#include <string>

using namespace Athena::Physics;
using namespace Athena::Scripting;


#define FROM_JS(CLASS_NAME)                                                                \
CLASS_NAME* fromJS##CLASS_NAME(v8::Handle<v8::Value> value)                                \
{                                                                                          \
    if (value->IsObject())                                                                 \
    {                                                                                      \
        CLASS_NAME* pObject = 0;                                                           \
        GetObjectPtr(value, &pObject);                                                     \
        return pObject;                                                                    \
    }                                                                                      \
                                                                                           \
    return 0;                                                                              \
}


#define CREATE_JS(CLASS_NAME)                                                              \
v8::Handle<v8::Object> createJS##CLASS_NAME()                                              \
{                                                                                          \
    v8::HandleScope handle_scope;                                                          \
                                                                                           \
    v8::Handle<v8::FunctionTemplate> func = ScriptingManager::getSingletonPtr()->getClassTemplate( \
                                                        "Athena.Physics." #CLASS_NAME);    \
                                                                                           \
    v8::Handle<v8::Object> jsObject = func->GetFunction()->NewInstance();                  \
                                                                                           \
    return handle_scope.Close(jsObject);                                                   \
}


#define TO_JS(CLASS_NAME)                                                                  \
v8::Handle<v8::Value> toJavaScript(CLASS_NAME* pObject)                                    \
{                                                                                          \
    v8::HandleScope handle_scope;                                                          \
                                                                                           \
    if (!pObject)                                                                          \
        return v8::Handle<v8::Value>();                                                    \
                                                                                           \
    v8::Handle<v8::FunctionTemplate> func = ScriptingManager::getSingletonPtr()->getClassTemplate( \
                                                        "Athena.Physics." #CLASS_NAME);    \
                                                                                           \
    v8::Handle<v8::Value> argv[1];                                                         \
    argv[0] = v8::External::New(pObject);                                                  \
                                                                                           \
    v8::Handle<v8::Object> jsObject = func->GetFunction()->NewInstance(1, argv);           \
                                                                                           \
    return handle_scope.Close(jsObject);                                                   \
}


#define IMPLEMENT_CONVERSIONS(CLASS_NAME)                                                  \
    FROM_JS(CLASS_NAME);                                                                   \
    CREATE_JS(CLASS_NAME);                                                                 \
    TO_JS(CLASS_NAME);



namespace Athena {
namespace Physics {

    IMPLEMENT_CONVERSIONS(Body)
    IMPLEMENT_CONVERSIONS(CollisionObject)
    IMPLEMENT_CONVERSIONS(CollisionShape)
    IMPLEMENT_CONVERSIONS(CompoundShape)
    IMPLEMENT_CONVERSIONS(GhostObject)
    IMPLEMENT_CONVERSIONS(PhysicalComponent)
    IMPLEMENT_CONVERSIONS(PrimitiveShape)
    IMPLEMENT_CONVERSIONS(StaticTriMeshShape)
    IMPLEMENT_CONVERSIONS(World)

}
}
