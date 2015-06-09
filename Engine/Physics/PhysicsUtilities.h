#pragma once

#include "LinearMath/btMatrix3x3.h"
#include "../Common/Utilities.h"
#include "box2d/common/b2math.h"

namespace Physics
{
	static void btQuaternionToQuaternion(const btQuaternion& i_btQuaternion, Quaternion& o_Quaternion)
	{
		o_Quaternion.w = i_btQuaternion.getW();
		o_Quaternion.x = i_btQuaternion.getX();
		o_Quaternion.y = i_btQuaternion.getY();
		o_Quaternion.z = i_btQuaternion.getZ();
	}

	static void btMatrix3x3ToMatrix33(const btMatrix3x3& i_btMatrix33, Matrix33& o_Matrix33)
	{
		o_Matrix33 = Matrix33::IDENTITY;

		o_Matrix33[0][0] = i_btMatrix33[0][0];
		o_Matrix33[0][1] = i_btMatrix33[0][1];
		o_Matrix33[0][2] = i_btMatrix33[0][2];
		o_Matrix33[1][0] = i_btMatrix33[1][0];
		o_Matrix33[1][1] = i_btMatrix33[1][1];
		o_Matrix33[1][2] = i_btMatrix33[1][2];
		o_Matrix33[2][0] = i_btMatrix33[2][0];
		o_Matrix33[2][1] = i_btMatrix33[2][1];
		o_Matrix33[2][2] = i_btMatrix33[2][2];
	}

	static void btVector3ToVec3(const btVector3& i_btVector3, Vec3& o_Vec3)
	{
		o_Vec3.x = static_cast<real>(i_btVector3.getX());
		o_Vec3.y = static_cast<real>(i_btVector3.getY());
		o_Vec3.z = static_cast<real>(i_btVector3.getZ());
	}

	static void b2Vec2ToVec2(const b2Vec2& i_b2Vec2, Vec2& o_Vec2)
	{
		o_Vec2.x = i_b2Vec2.x;
		o_Vec2.y = i_b2Vec2.y;
	}

	static void QuaternionTobtQuaternion(const Quaternion& i_Quaternion, btQuaternion& o_btQuaternion)
	{
		o_btQuaternion.setW(i_Quaternion.w);
		o_btQuaternion.setX(i_Quaternion.x);
		o_btQuaternion.setY(i_Quaternion.y);
		o_btQuaternion.setZ(i_Quaternion.z);
	}

	static void Matrix33TobtMatrix3x3(const Matrix33& i_Matrix33, btMatrix3x3& o_btMatrix33)
	{
		o_btMatrix33.setIdentity();

		o_btMatrix33[0][0] = i_Matrix33[0][0];
		o_btMatrix33[0][1] = i_Matrix33[0][1];
		o_btMatrix33[0][2] = i_Matrix33[0][2];
		o_btMatrix33[1][0] = i_Matrix33[1][0];
		o_btMatrix33[1][1] = i_Matrix33[1][1];
		o_btMatrix33[1][2] = i_Matrix33[1][2];
		o_btMatrix33[2][0] = i_Matrix33[2][0];
		o_btMatrix33[2][1] = i_Matrix33[2][1];
		o_btMatrix33[2][2] = i_Matrix33[2][2];
	}

	static void Vec3TobtVector3(const Vec3& i_Vec3, btVector3& o_btVector3)
	{
		o_btVector3.setX(i_Vec3.x);
		o_btVector3.setY(i_Vec3.y);
		o_btVector3.setZ(i_Vec3.z);
	}

	static void Vec2Tob2Vec2(const Vec2& i_Vec2, b2Vec2& o_b2Vec2)
	{
		o_b2Vec2.x = i_Vec2.x;
		o_b2Vec2.y = i_Vec2.y;
	}
}