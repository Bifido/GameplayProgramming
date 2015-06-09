#pragma once

#include "PhysicsWorld.h"
#include "LuaObject.h"
#include "../Manager/LuaManager.h"
#include "PhysicsComponent.h"
#include "../Event/RegistryEventPublisher.h"
#include "DebugDrawer/PhysicsDebugDrawer.h"
#include "Box2D/Dynamics/b2WorldCallbacks.h"
#include "Box2D/Dynamics/Contacts/b2Contact.h"

class b2World;

class PhysicsWorldFactory;
class Box2DWorld;
class PhysicsObject;

class ContactListener : public b2ContactListener
{
private:
	void BeginContact(b2Contact* contact)
	{
		PhysicsComponent* pFirstPhysicsComponent = static_cast<PhysicsComponent*>(contact->GetFixtureA()->GetBody()->GetUserData());
		PhysicsComponent* pSecondPhysicsComponent = static_cast<PhysicsComponent*>(contact->GetFixtureB()->GetBody()->GetUserData());
		if(pFirstPhysicsComponent && pSecondPhysicsComponent)
		{
			//if obj1 has the notification of collisions enable I add this obj into MapCollisionOccurred
			if(pFirstPhysicsComponent->IsNotificationEnabled())
			{
				PhysicsComponent::CollisionData oCollisionInfo;
				oCollisionInfo.m_pFirstObject = pFirstPhysicsComponent;
				oCollisionInfo.m_pSecondObject = pSecondPhysicsComponent;

				char pEventName[128];
				sprintf_s(pEventName, "Collision/%s", pFirstPhysicsComponent->GetTextName().c_str());
				m_oEventCollisionPublisher.SetPath(ObjectId(pEventName));
				m_oEventCollisionPublisher.Raise(oCollisionInfo);
			}
			if(pSecondPhysicsComponent->IsNotificationEnabled())
			{
				PhysicsComponent::CollisionData oCollisionInfo;
				oCollisionInfo.m_pFirstObject = pSecondPhysicsComponent;
				oCollisionInfo.m_pSecondObject = pFirstPhysicsComponent;


				char pEventName[128];
				sprintf_s(pEventName, "Collision/%s", pSecondPhysicsComponent->GetTextName().c_str());
				m_oEventCollisionPublisher.SetPath(ObjectId(pEventName));
				m_oEventCollisionPublisher.Raise(oCollisionInfo);
			}
		}


	}

	void EndContact(b2Contact* contact) 
	{
	}

	RegistryEventPublisher<PhysicsComponent::CollisionData> m_oEventCollisionPublisher;

};

class Box2DWorld : public PhysicsWorld
{
public:

	Box2DWorld();
	~Box2DWorld();

	virtual void Configure();
	virtual void Cleanup();

	virtual void AddObject(const PhysicsObject* i_object);
	virtual void RemoveObject(const PhysicsObject* i_object);
	virtual void RemoveAllObjects();
	
	virtual void Step(const real i_dt);
	virtual void PostStep();
	void UpdateBodies();
private:

	ContactListener						m_oContactListener;
	b2World*							m_World;
	DebugDraw							m_debugDraw;
	//DestructionListener					m_destructionListener;

	MGDVector<PhysicsComponent::CollisionData>		m_MapCollisionOccurred;

	static Box2DWorld* s_Box2DWorld;
};

class Box2DWorldFactory : public PhysicsWorldFactory
{
public:

	PhysicsWorld* CreateWorld() const
	{
		return new Box2DWorld();
	}
};