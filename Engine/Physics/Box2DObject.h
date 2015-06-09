#pragma once

#include "PhysicsObject.h"
#include "Box2D\Dynamics\b2Body.h"
#include "Box2D\Dynamics\b2Fixture.h"

class Box2DObject : public PhysicsObject
{
public:

	Box2DObject(PhysicsObjectData& i_oObjectData, PhysicsComponent* i_pPhysicsComponent);
	~Box2DObject();

	const b2Body* GetRigidBody() const;	
	b2Body* GetRigidBody();
	void CreateRigidBody(b2Body* i_pRigidBody);

	const b2BodyDef& GetRigidBodyInfo() const;

	virtual void UpdateTransform(const Vec3& i_oPos, const Quaternion& i_oRot);
	//virtual void NotifyCollision();

private:	
	b2Body* m_pRigidBody;
	b2BodyDef m_oBodyInfo;
	b2FixtureDef m_oFixtureInfo;
	b2Shape*		m_pShape;
};

inline const b2Body* Box2DObject::GetRigidBody() const
{
	return m_pRigidBody;
}

inline b2Body* Box2DObject::GetRigidBody()
{
	return m_pRigidBody;
}