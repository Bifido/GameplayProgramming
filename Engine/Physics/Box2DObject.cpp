#include "Box2DObject.h"
#include "../Physics/PhysicsUtilities.h"
#include "..\Physics\PhysicsComponent.h"
#include "Box2D\Collision\Shapes\b2PolygonShape.h"
#include "Box2D\Collision\Shapes\b2CircleShape.h"
#include "Box2D\Collision\Shapes\b2EdgeShape.h"

Box2DObject::Box2DObject(PhysicsObjectData& i_oObjectData, PhysicsComponent* i_pPhysicsComponent)
	: PhysicsObject(i_pPhysicsComponent)
	, m_pRigidBody(NULL)
{
	m_oFixtureInfo.density = i_oObjectData.m_fMass;
	m_oFixtureInfo.friction = i_oObjectData.m_fFriction;
	m_oFixtureInfo.restitution = i_oObjectData.m_fRestitution;
	m_oBodyInfo.angularDamping = i_oObjectData.m_fAngularDamping;
	m_oBodyInfo.linearDamping = i_oObjectData.m_fLinearDamping;

	if(i_oObjectData.m_oCollisionShape == PhysicsObjectData::BOX)
	{
		m_pShape = new b2PolygonShape();
		static_cast<b2PolygonShape*>(m_pShape)->SetAsBox(i_oObjectData.m_oDimCollisionShape.x, i_oObjectData.m_oDimCollisionShape.y);

		m_oFixtureInfo.shape = m_pShape;
	}
	else if(i_oObjectData.m_oCollisionShape == PhysicsObjectData::CIRCLE)
	{
		m_pShape = new b2CircleShape();
		static_cast<b2CircleShape*>(m_pShape)->m_radius = i_oObjectData.m_oDimCollisionShape.x;

		m_oFixtureInfo.shape = m_pShape;
	}	
	else if(i_oObjectData.m_oCollisionShape == PhysicsObjectData::EDGE)
	{
		m_pShape = new b2EdgeShape();
		static_cast<b2EdgeShape*>(m_pShape)->Set(b2Vec2(i_oObjectData.m_oStartPointShape.x, i_oObjectData.m_oStartPointShape.y), b2Vec2(i_oObjectData.m_oEndPointShape.x, i_oObjectData.m_oEndPointShape.y));
		
		m_oFixtureInfo.shape = m_pShape;
	}
	else
	{
		MGD_ASSERT(0 && "Collision shape not found!");
	}
}

Box2DObject::~Box2DObject()
{
	delete m_pShape;
}

void Box2DObject::UpdateTransform(const Vec3& i_oPos, const Quaternion& i_oRot)
{
	m_pRigidBody->SetTransform(b2Vec2(i_oPos.x,i_oPos.y), i_oRot.getRoll().valueDegrees() * 0.0174532925f);
}

void Box2DObject::CreateRigidBody(b2Body* i_pRigidBody)
{
	m_pRigidBody = i_pRigidBody;
	if(m_pRigidBody)
	{
		m_pRigidBody->CreateFixture(&m_oFixtureInfo);
		m_pRigidBody->SetUserData(m_pPhysicsComponent);

		static_cast<Box2DMotionState*>(m_pPhysicsComponent->GetObjectData().m_pMotionState)->getWorldTransform(*m_pRigidBody);
	}
}

const b2BodyDef& Box2DObject::GetRigidBodyInfo() const
{
	return m_oBodyInfo;
}
