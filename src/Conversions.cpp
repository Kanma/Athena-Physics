/**	@file	Conversions.cpp
	@author	Philip Abbet

	Implementation of the conversion functions between Athena and Bullet types
*/

#include <Athena-Physics/Conversions.h>


using namespace Athena;
using namespace std;


namespace Athena {
namespace Physics {

//-----------------------------------------------------------------------

btVector3 toBullet(const Math::Vector3& v)
{
    return btVector3(v.x, v.y, v.z);
}

//-----------------------------------------------------------------------

btQuaternion toBullet(const Math::Quaternion& q)
{
    return btQuaternion(q.x, q.y, q.z, q.w);
}

//-----------------------------------------------------------------------

Math::Vector3 fromBullet(const btVector3& v)
{
    return Math::Vector3(v.m_floats[0], v.m_floats[1], v.m_floats[2]);
}

//-----------------------------------------------------------------------

Math::Quaternion fromBullet(const btQuaternion& q)
{
    return Math::Quaternion(q.w(), q.x(), q.y(), q.z());
}

}
}
