#pragma once

#include "../Common/Utilities.h"
#include "MotionState.h"

struct PhysicsObjectData
{
	PhysicsObjectData()
		: m_fMass(0.0f)
		, m_fFriction(0.0f)
		, m_fRestitution(0.0f)
		, m_fLinearDamping(0.0f)
		, m_fAngularDamping(0.0f)
		, m_fRollingFriction(0.0f)
		, m_pMotionState(NULL)
		, m_oCollisionShape(NONE)
		, m_oDimCollisionShape()
	{

	}

	enum CollisionShape
	{
		SPHERE = 0,
		BOX,
		CYLINDER,
		CAPSULE,
		CONE,
		CIRCLE,
		EDGE,
		NONE
	};

	real m_fMass;
	real m_fFriction;
	real m_fRestitution;
	real m_fLinearDamping;
	real m_fAngularDamping;
	real m_fRollingFriction;
	MotionState* m_pMotionState;
	CollisionShape	m_oCollisionShape;
	Vec3			m_oDimCollisionShape;

	Vec3			m_oStartPointShape;
	Vec3			m_oEndPointShape;
};

class PhysicsComponent;

class PhysicsObject
{
public:

	PhysicsObject(PhysicsComponent*  i_pPhysicsComponent)
		: m_pPhysicsComponent(i_pPhysicsComponent) { }

	virtual ~PhysicsObject() { }

	virtual void UpdateTransform(const Vec3& i_oPos, const Quaternion& i_oRot) = 0;

	//virtual void NotifyCollision() = 0;

	PhysicsComponent* GetPhysicsComponent() const
	{
		return m_pPhysicsComponent;
	}

protected:
	PhysicsComponent*	m_pPhysicsComponent;
};