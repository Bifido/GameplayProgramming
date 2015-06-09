#include "Box2DWorld.h"
#include "box2d\Dynamics/b2World.h"
#include "Box2DObject.h"
#include "MotionState.h"
#include "../LOG/LOGManager.h"
#include "../Common/Common.h"

Box2DWorld* Box2DWorld::s_Box2DWorld = 0;

Box2DWorld::Box2DWorld()
	: m_World(NULL)
{
}

Box2DWorld::~Box2DWorld()
{
	
}

void Box2DWorld::Configure()
{
	tinyxml2::XMLDocument oDocument;
	tinyxml2::XMLError oErr = oDocument.LoadFile(CONFIG.c_str());
	if(oErr != tinyxml2::XML_NO_ERROR)
	{
		MGD_LOG::LOGManager::GetSingleton().WriteLog(MGD_LOG::MGD_ERROR, PHYSICS_CONTEXT, "Error loading file: %s", CONFIG.c_str());
		return;
	}
	const tinyxml2::XMLElement* pRoot = oDocument.RootElement();
	if(pRoot)
	{
		const tinyxml2::XMLElement* pPhysicsDebugDraw = pRoot->FirstChildElement("PhysicsDebugDraw");
		if(pPhysicsDebugDraw)
		{
			m_bDebugDraw = pPhysicsDebugDraw->BoolAttribute("enable");
		}

		const tinyxml2::XMLElement* pGravity = pRoot->FirstChildElement("Gravity");
		if( pGravity)
		{
			m_fGravity =  pGravity->FloatAttribute("value");
		}
	}


	m_World = new b2World(b2Vec2(0, -m_fGravity));

	if(m_World)
	{

// 		m_destructionListener.test = this;
// 		m_World->SetDestructionListener(&m_destructionListener);
		m_World->SetContactListener(&m_oContactListener);
		uint32 flags = 0;
		flags += b2Draw::e_shapeBit;
		flags += b2Draw::e_jointBit;
		flags += b2Draw::e_aabbBit;
		flags += b2Draw::e_centerOfMassBit;

		if(m_bDebugDraw)
		{
			m_debugDraw.SetFlags(flags);
			m_World->SetDebugDraw(&m_debugDraw);
		}
	}

	s_Box2DWorld = this;
}

void Box2DWorld::Cleanup()
{
	s_Box2DWorld = 0;

	//delete dynamics world
	delete m_World;
}

void Box2DWorld::AddObject(const PhysicsObject* i_object)
{
	PhysicsObject* pPhysicsObject = const_cast<PhysicsObject*>(i_object);
	if(pPhysicsObject)
	{
		Box2DObject* pBox2DObject = static_cast<Box2DObject*>(pPhysicsObject);
		if(pBox2DObject)
		{
			b2Body* pRigidBody = m_World->CreateBody(&pBox2DObject->GetRigidBodyInfo());
			MGD_ASSERT(pRigidBody);
			if(pRigidBody)
			{
				pBox2DObject->CreateRigidBody(pRigidBody);
			}
		}
	}
}

void Box2DWorld::RemoveObject(const PhysicsObject* i_object)
{	
	PhysicsObject* pPhysicsObject = const_cast<PhysicsObject*>(i_object);
	if(pPhysicsObject)
	{
		Box2DObject* pBox2DObject = static_cast<Box2DObject*>(pPhysicsObject);
		if(pBox2DObject)
		{
			b2Body* pBody = pBox2DObject->GetRigidBody();
			if(pBody)
			{
				/*MGDMap<const btRigidBody*, PhysicsObject*>::iterator it = m_MapCollisionObjects.find(const_cast<const btRigidBody*>(pBody));
				if(it != m_MapCollisionObjects.end())
				{
					m_MapCollisionObjects.erase(it);
				}*/
				m_World->DestroyBody(pBody);
			}
		}
	}
}

void Box2DWorld::RemoveAllObjects()
{
	for (int i = m_World->GetBodyCount() - 1; i >= 0; i--)
	{
		b2Body* obj = m_World->GetBodyList() + i;
		m_World->DestroyBody( obj );
	}

	//m_MapCollisionObjects.clear();
}

void Box2DWorld::Step(const real i_dt)
{
	m_World->Step(1.f / 60.f, 6, 2);

	if(m_bDebugDraw)
	{
		m_World->DrawDebugData();
	}
	
	PostStep();
}

void Box2DWorld::PostStep()
{
	UpdateBodies();
}

void Box2DWorld::UpdateBodies()
{
	for(b2Body* pBody = m_World->GetBodyList(); pBody; pBody = pBody->GetNext())
	{
		PhysicsComponent* p = static_cast<PhysicsComponent*>(pBody->GetUserData());
		if(p)
		{
			static_cast<Box2DMotionState*>(p->GetObjectData().m_pMotionState)->setWorldTransform(Vec2(pBody->GetPosition().x,pBody->GetPosition().y), pBody->GetAngle());
		}
	}
}