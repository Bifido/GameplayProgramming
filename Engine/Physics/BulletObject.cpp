#include "BulletObject.h"
#include "BulletCollision\CollisionShapes\btBoxShape.h"
#include "LinearMath\btDefaultMotionState.h"
#include "BulletDynamics\Dynamics\btRigidBody.h"
#include "../Physics/PhysicsUtilities.h"
#include "BulletCollision\CollisionShapes\btCollisionShape.h"
#include "BulletCollision\CollisionShapes\btSphereShape.h"
#include "BulletCollision\CollisionShapes\btCylinderShape.h"
#include "..\Physics\PhysicsComponent.h"

BulletObject::BulletObject(PhysicsObjectData& i_oObjectData, PhysicsComponent* i_pPhysicsComponent)
	: PhysicsObject(i_pPhysicsComponent)
	, m_pRigidBody(NULL)
	, m_pCollisionShape(NULL)
{
	btVector3 localInertia(0, 0, 0);
	m_pCollisionShape = GetCollisionShape(i_oObjectData.m_oCollisionShape, i_oObjectData.m_oDimCollisionShape);
	if(m_pCollisionShape)
	{
		m_pCollisionShape->calculateLocalInertia(i_oObjectData.m_fMass, localInertia);
	}
	
	btRigidBody::btRigidBodyConstructionInfo rbInfo(i_oObjectData.m_fMass, static_cast<BulletMotionState*>(i_oObjectData.m_pMotionState), m_pCollisionShape, localInertia);
	rbInfo.m_friction = i_oObjectData.m_fFriction;
	rbInfo.m_restitution = i_oObjectData.m_fRestitution;
	rbInfo.m_angularDamping = i_oObjectData.m_fAngularDamping;
	rbInfo.m_linearDamping = i_oObjectData.m_fLinearDamping;
	rbInfo.m_rollingFriction = i_oObjectData.m_fRollingFriction;

	m_pRigidBody = new btRigidBody(rbInfo);
	m_pRigidBody->setUserPointer(m_pPhysicsComponent);

}

BulletObject::~BulletObject()
{
	delete m_pRigidBody;
	delete m_pCollisionShape;
}

void BulletObject::UpdateTransform(const Vec3& i_oPos, const Quaternion& i_oRot)
{
	if(m_pRigidBody && m_pRigidBody->getMotionState())
	{
		btVector3 obtPos;
		btQuaternion obtRot;

		Physics::Vec3TobtVector3(i_oPos, obtPos);
		Physics::QuaternionTobtQuaternion(i_oRot,obtRot);

		btTransform t;
		t.setOrigin(obtPos);
		t.setRotation(obtRot);

		m_pRigidBody->clearForces();
		m_pRigidBody->activate(true);
		m_pRigidBody->setCenterOfMassTransform(t);
	}
}

btCollisionShape* BulletObject::GetCollisionShape( const PhysicsObjectData::CollisionShape& i_oCollisionShape, const Vec3& i_oDimCollisionShape)
{
	switch(i_oCollisionShape)
	{
	case PhysicsObjectData::BOX:
		return new btBoxShape(btVector3(i_oDimCollisionShape.x,i_oDimCollisionShape.y,i_oDimCollisionShape.z));
		break;
	case PhysicsObjectData::SPHERE:
		return new btSphereShape(i_oDimCollisionShape.x);
		break;
	case PhysicsObjectData::CYLINDER:
		return new btCylinderShape(btVector3(i_oDimCollisionShape.x,i_oDimCollisionShape.y,i_oDimCollisionShape.z));
		break;
	default:
		return NULL;
	}
}

//void BulletObject::NotifyCollision()
//{
//	MGD_ASSERT(m_pPhysicsComponent);
//	if(m_pPhysicsComponent)
//	{
//		m_pPhysicsComponent->NotifyCollision();
//	}
//}
