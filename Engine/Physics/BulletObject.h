#pragma once

#include "PhysicsObject.h"

class btRigidBody;
class btCollisionShape;

class BulletObject : public PhysicsObject
{
public:

	BulletObject(PhysicsObjectData& i_oObjectData, PhysicsComponent* i_pPhysicsComponent);
	~BulletObject();

	const btRigidBody* GetRigidBody() const;	
	btRigidBody* GetRigidBody();

	virtual void UpdateTransform(const Vec3& i_oPos, const Quaternion& i_oRot);
	//virtual void NotifyCollision();

private:	
	btCollisionShape* GetCollisionShape( const PhysicsObjectData::CollisionShape& i_oCollisionShape, const Vec3& i_oDimCollisionShape);

	btRigidBody* m_pRigidBody;
	btCollisionShape* m_pCollisionShape;
};

inline const btRigidBody* BulletObject::GetRigidBody() const
{
	return m_pRigidBody;
}

inline btRigidBody* BulletObject::GetRigidBody()
{
	return m_pRigidBody;
}