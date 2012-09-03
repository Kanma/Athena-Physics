/** @file   Conversions.h
    @author Philip Abbet

    Declaration of some conversion functions between Athena and Bullet types
*/

#ifndef _ATHENA_PHYSICS_CONVERSIONS_H_
#define _ATHENA_PHYSICS_CONVERSIONS_H_

#include <Athena-Physics/Prerequisites.h>
#include <Athena-Math/Quaternion.h>
#include <Athena-Math/Vector3.h>
#include <LinearMath/btQuaternion.h>
#include <LinearMath/btVector3.h>


namespace Athena {
namespace Physics {

btVector3 toBullet(const Math::Vector3& v);
btQuaternion toBullet(const Math::Quaternion& q);

Math::Vector3 fromBullet(const btVector3& v);
Math::Quaternion fromBullet(const btQuaternion& q);

}
}

#endif
