#pragma once

#include "PhysicsWorld.h"
#include "BulletCollision\NarrowPhaseCollision\btManifoldPoint.h"
#include "BulletCollision\CollisionDispatch\btCollisionObjectWrapper.h"
#include "../Physics/DebugDrawer/PhysicsDebugDrawer.h"
#include "LuaObject.h"
#include "../Manager/LuaManager.h"
#include "PhysicsComponent.h"
#include "../Event/RegistryEventPublisher.h"

class btBroadphaseInterface;
class btCollisionDispatcher;
class btConstraintSolver;
class btDefaultCollisionConfiguration;

class btDynamicsWorld;

class btRigidBody;

class PhysicsWorldFactory;
class BulletWord;
class PhysicsObject;

class BulletWorld : public PhysicsWorld
{
public:

	BulletWorld();
	~BulletWorld();

	virtual void Configure();
	virtual void Cleanup();

	virtual void AddObject(const PhysicsObject* i_object);
	virtual void RemoveObject(const PhysicsObject* i_object);
	virtual void RemoveAllObjects();
	
	virtual void Step(const real i_dt);
	virtual void PostStep();

	static bool CollisionCallback(btManifoldPoint& i_oCollisionPoint, const btCollisionObjectWrapper* i_pFirstCollisionObjWrapper, int i_iPartID0, int i_iIndex0, const btCollisionObjectWrapper* i_pSecCollisionObjWrapper, int i_iPartID1, int i_iIndex1);

	static void EnableDebugDrawer(bool i_bEnable)
	{
		if(s_BulletWorld && s_BulletWorld->m_pDebugDrawer)
		{
			s_BulletWorld->m_pDebugDrawer->setDebugMode((i_bEnable) ? 1 : 0);
		}
	}

private:

	btBroadphaseInterface*				m_overlappingPairCache;
	btCollisionDispatcher*				m_dispatcher;
	btConstraintSolver*					m_constraintSolver;
	btDefaultCollisionConfiguration*	m_collisionConfiguration;

	btDynamicsWorld*					m_dynamicsWorld;
	PhysicsDebugDrawer*					m_pDebugDrawer;

	//MGDMap<const btRigidBody*, PhysicsObject*>	m_MapCollisionObjects;
	//MGDMap<const btRigidBody*, CollisionData>		m_MapCollisionOccurred;
	MGDVector<PhysicsComponent::CollisionData>		m_MapCollisionOccurred;

	RegistryEventPublisher<PhysicsComponent::CollisionData> m_oEventCollisionPublisher;

	static BulletWorld* s_BulletWorld;
};

class BulletWorldFactory : public PhysicsWorldFactory
{
public:

	PhysicsWorld* CreateWorld() const
	{
		return new BulletWorld();
	}
};