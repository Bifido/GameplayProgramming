#include "BulletWorld.h"
#include "BulletObject.h"

#include "btBulletDynamicsCommon.h"
#include "BulletCollision\CollisionDispatch\btManifoldResult.h"

extern ContactAddedCallback		gContactAddedCallback;

BulletWorld* BulletWorld::s_BulletWorld = 0;

BulletWorld::BulletWorld()
	: m_overlappingPairCache(NULL)
	, m_dispatcher(NULL)
	, m_constraintSolver(NULL)
	, m_collisionConfiguration(NULL)
	, m_dynamicsWorld(NULL)
	, m_pDebugDrawer(NULL)
{
	gContactAddedCallback = CollisionCallback;
}

BulletWorld::~BulletWorld()
{
	
}

void BulletWorld::Configure()
{
	m_collisionConfiguration = new btDefaultCollisionConfiguration();
	m_dispatcher = new btCollisionDispatcher(m_collisionConfiguration);
	m_overlappingPairCache = new btDbvtBroadphase();
	m_constraintSolver = new btSequentialImpulseConstraintSolver();
	m_dynamicsWorld = new btDiscreteDynamicsWorld(m_dispatcher, m_overlappingPairCache, m_constraintSolver, m_collisionConfiguration);

	s_BulletWorld = this;

	if(m_dynamicsWorld)
	{
		m_dynamicsWorld->setGravity(btVector3(0,-9.8f,0));
	}

	//m_pDebugDrawer = new PhysicsDebugDrawer(m_dynamicsWorld);
	//m_dynamicsWorld->setDebugDrawer(m_pDebugDrawer);	
}

void BulletWorld::Cleanup()
{
	s_BulletWorld = 0;

	//delete dynamics world
	delete m_dynamicsWorld;

	//delete solver
	delete m_constraintSolver;

	//delete broadphase
	delete m_overlappingPairCache;

	//delete dispatcher
	delete m_dispatcher;

	delete m_collisionConfiguration;

	delete m_pDebugDrawer;
}

void BulletWorld::AddObject(const PhysicsObject* i_object)
{
	PhysicsObject* pPhysicsObject = const_cast<PhysicsObject*>(i_object);
	if(pPhysicsObject)
	{
		BulletObject* pBulletObject = static_cast<BulletObject*>(pPhysicsObject);
		if(pBulletObject)
		{
			btRigidBody* pBody = pBulletObject->GetRigidBody();
			if(pBody)
			{
				//m_MapCollisionObjects[pBody] = pPhysicsObject;
				m_dynamicsWorld->addRigidBody(pBody);
			}
		}
	}
}

void BulletWorld::RemoveObject(const PhysicsObject* i_object)
{
	PhysicsObject* pPhysicsObject = const_cast<PhysicsObject*>(i_object);
	if(pPhysicsObject)
	{
		BulletObject* pBulletObject = static_cast<BulletObject*>(pPhysicsObject);
		if(pBulletObject)
		{
			btRigidBody* pBody = pBulletObject->GetRigidBody();
			if(pBody)
			{
				/*MGDMap<const btRigidBody*, PhysicsObject*>::iterator it = m_MapCollisionObjects.find(const_cast<const btRigidBody*>(pBody));
				if(it != m_MapCollisionObjects.end())
				{
					m_MapCollisionObjects.erase(it);
				}*/
				m_dynamicsWorld->removeRigidBody(pBody);
			}
		}
	}
	
}

void BulletWorld::RemoveAllObjects()
{
	for (int i = m_dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
	{
		btCollisionObject* obj = m_dynamicsWorld->getCollisionObjectArray()[i];
		m_dynamicsWorld->removeCollisionObject( obj );
		delete obj;
	}

	//m_MapCollisionObjects.clear();
}

bool BulletWorld::CollisionCallback( btManifoldPoint& i_oCollisionPoint,const btCollisionObjectWrapper* i_pFirstCollisionObjWrapper, int i_iPartID0, int i_iIndex0, const btCollisionObjectWrapper* i_pSecCollisionObjWrapper, int i_iPartID1, int i_iIndex1 )
{
	//Iterate collisions
	const uint32 numManifolds= s_BulletWorld->m_dynamicsWorld->getDispatcher()->getNumManifolds();
	for(uint32 manifoldIndex= 0; manifoldIndex < numManifolds; ++manifoldIndex)
	{
		//Get collision info
		btPersistentManifold* manifold= s_BulletWorld->m_dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(manifoldIndex);
		if(manifold)
		{
			//Get Obj1 collided
			const btRigidBody* bodyA = static_cast<const btRigidBody*>(manifold->getBody0());

			//Get Obj2 collided
			const btRigidBody* bodyB = static_cast<const btRigidBody*>(manifold->getBody1());
			if(bodyA && bodyB)
			{
					//Get physic components for body1 and body2
					PhysicsComponent* pFirstPhysicsComponent = static_cast<PhysicsComponent*>(bodyA->getUserPointer());
					PhysicsComponent* pSecondPhysicsComponent = static_cast<PhysicsComponent*>(bodyB->getUserPointer());
					if(pFirstPhysicsComponent && pSecondPhysicsComponent)
					{
						//if obj1 has the notification of collisions enable I add this obj into MapCollisionOccurred
						if(pFirstPhysicsComponent->IsNotificationEnabled())
						{
							PhysicsComponent::CollisionData oCollisionInfo;
							oCollisionInfo.m_pFirstObject = pFirstPhysicsComponent;
							oCollisionInfo.m_pSecondObject = pSecondPhysicsComponent;
							oCollisionInfo.m_fImpulse = i_oCollisionPoint.getAppliedImpulse();

							char pEventName[128];
							sprintf_s(pEventName, "Collision/%s", pFirstPhysicsComponent->GetTextName().c_str());
							s_BulletWorld->m_oEventCollisionPublisher.SetPath(ObjectId(pEventName));
							s_BulletWorld->m_oEventCollisionPublisher.Raise(oCollisionInfo);
						}
						if(pSecondPhysicsComponent->IsNotificationEnabled())
						{
							PhysicsComponent::CollisionData oCollisionInfo;
							oCollisionInfo.m_pFirstObject = pSecondPhysicsComponent;
							oCollisionInfo.m_pSecondObject = pFirstPhysicsComponent;
							oCollisionInfo.m_fImpulse = i_oCollisionPoint.getAppliedImpulse();


							char pEventName[128];
							sprintf_s(pEventName, "Collision/%s", pSecondPhysicsComponent->GetTextName().c_str());
							s_BulletWorld->m_oEventCollisionPublisher.SetPath(ObjectId(pEventName));
							s_BulletWorld->m_oEventCollisionPublisher.Raise(oCollisionInfo);
						}
					}
			}
		}
	}

 return true;
}

void BulletWorld::Step(const real i_dt)
{
	m_dynamicsWorld->stepSimulation(1.f / 60.f);
	
	PostStep();
}

void BulletWorld::PostStep()
{
	if(m_pDebugDrawer)
	{
		m_pDebugDrawer->step();
	}

	//MGDVector<PhysicsComponent::CollisionData>::iterator it = m_MapCollisionOccurred.begin();
	//for(; it != m_MapCollisionOccurred.end(); ++it)
	//{
	//	if((*it).m_pFirstObject)
	//	{
	//		(*it).m_pFirstObject->NotifyCollision(*it);
	//	}
	//}

	//m_MapCollisionOccurred.clear();
}