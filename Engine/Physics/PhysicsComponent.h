/*
*
*
*	Lua Functions:
*
*	void ApplyForce(Force) -> Force = LuaObject = { x= val, y = val, z = val}
*	void SetVelocity(Vel) -> Vel = LuaObject = { x= val, y = val, z = val}
*	LuaObject GetVelocity() -> LuaObject = { x= val, y = val, z = val}
*	real GetMass()
*	real GetFriction()
*	real GetRestitution()
*	real GetLinearDamping()
*	real GetAngularDamping()
*	real GetRollingFriction()
*
*/

#pragma once
#include "../Core/Component.h"
#include "PhysicsObject.h"
#include "../Event\RegistryEventHandler.h"
#include "BulletCollision\CollisionDispatch\btCollisionObject.h"
#include "LuaPlus.h"
#include "..\Script\LuaUtilities.h"
#include "..\Core\Actor.h"

class PhysicsComponent : public Component
{
public:
	ID_DECLARATION;

	PhysicsComponent(std::string i_szOwnerID, bool i_bIsCreatedFromTemplate = false);
	virtual ~PhysicsComponent();

	enum ePhysicsEngineType
	{
		eBox2D,
		eBullet
	};

	enum eType
	{
		DYNAMIC,
		STATIC,
		KINEMATIC
	};

	virtual void Init();
	virtual bool SetupFromXml(const tinyxml2::XMLElement* pNode);
	virtual void CreateFromTemplate(Component* i_pComponent, const ObjectId& i_oOwner)
	{
		if (i_pComponent)
		{
			PhysicsComponent* pPhysicsComponent = static_cast<PhysicsComponent*>(i_pComponent);
			if (pPhysicsComponent)
			{			
				m_oObjectData = pPhysicsComponent->GetObjectData();
				m_CollisionFlags = pPhysicsComponent->GetCollisionFlag();
				m_oCollisionInfo = pPhysicsComponent->GetCollisionData();
				m_bNotifyCollision = pPhysicsComponent->IsNotificationEnabled();
				m_oType = pPhysicsComponent->GetObjectType();
				Component::CreateFromTemplate(i_oOwner);
			}			
		}
	}
	
	class CollisionData
	{	
	public:
		ID_DECLARATION;
		CollisionData()
		{
			LuaPlus::LuaObject metaTable1 = LuaManager::GetSingleton().GetGlobalVars().CreateTable("CollisionInfoMetatable");
			metaTable1.SetObject("__index", metaTable1);	

			m_oLuaObject.AssignNewTable(LuaManager::GetSingleton().GetLuaState());
			LuaPlus::LuaObject metaTable =	LuaManager::GetSingleton().GetGlobalVars().GetByName("CollisionInfoMetatable");
			m_oLuaObject.SetLightUserData("__object", const_cast<CollisionData*>(this));
			m_oLuaObject.SetMetaTable(metaTable);
		}

		LuaPlus::LuaObject GetLuaCollisionObj()
		{
			MGD_ASSERT(!m_oLuaObject.IsNil());
			return m_oLuaObject;
		}
		void SetupLuaObject()
		{
			//Set Physics component first obj
			m_oLuaObject.SetString("FirstObj", m_pFirstObject->GetOwnerID().GetDebugName().c_str());

			//Set Physics component second obj
			m_oLuaObject.SetString("SecondObj", m_pSecondObject->GetOwnerID().GetDebugName().c_str());

			//Set velocity
			m_oLuaObject.Set("Impulse", static_cast<uint32>(m_fImpulse));			
		};

		LuaPlus::LuaObject	m_oLuaObject;
		PhysicsComponent* m_pFirstObject;
		PhysicsComponent* m_pSecondObject;
		//Actor*	m_pFirstActor;
		//Actor*	m_pSecondActor;
		real    m_fImpulse;

	};

	void NotifyCollision(const PhysicsComponent::CollisionData& i_oCollisionData);
	void ResetCollision();
	bool IsInCollision();
	bool IsNotificationEnabled() const;

	PhysicsObject* GetPhysicsObject() const
	{
		MGD_ASSERT(m_pObject);
		return m_pObject;
	}
	
	eType GetObjectType() const
	{
		return m_oType;
	}

	void SetMass( real i_fMass);
	real GetMass() const;

	void SetFriction( real i_fFriction);
	real GetFriction() const;

	void SetRestitution( real i_fRestitution);
	real GetRestitution() const;

	void SetLinearDamping(real i_fLinearDamping);
	real GetLinearDamping() const;

	void SetAngularDamping(real i_fAngularDamping);
	real GetAngularDamping() const;

	void SetRollingFriction(real i_fRollingFriction);
	real GetRollingFriction() const;

	void SetCollisionShape( PhysicsObjectData::CollisionShape i_oCollisionShape, const Vec3& i_oDimCollisionShape, const Vec3& i_oScaleCollisionShape = Vec3::UNIT_SCALE);
	
	void SetCollisionFlag(int32 i_iCollisionFlag);
	int32 GetCollisionFlag() const;
	
	void SetCollisionData(CollisionData i_oCollisionInfo);
	const CollisionData& GetCollisionData() const;

	void ApplyForce( const Vec3& i_oForce);

	void SetVelocity( const Vec3& i_oVelocity);
	LuaPlus::LuaObject GetVelocity() const;

	void SyncronizePosition();	

	const PhysicsObjectData& GetObjectData() const;

	/******* LUA *******/
	static void RegisterScriptFunction();
	/*******************/

private:
	PhysicsObject*					m_pObject;
	PhysicsObjectData				m_oObjectData;
	CollisionData					m_oCollisionInfo;
	ePhysicsEngineType				m_eEngineType;

	bool				m_bIsInCollision;
	bool				m_bNotifyCollision;

	eType				m_oType;
	int32				m_CollisionFlags;

	RegistryEventHandler<PhysicsComponent> m_oEvent;
	RegistryEventHandler<PhysicsComponent, CollisionData> m_oCollisionEvent;

	/******* LUA *******/
	void CreateLuaObject();
	void ApplyForceFromScript(LuaPlus::LuaObject i_oVec3LuaObject);
	void SetVelocityFromScript(LuaPlus::LuaObject i_oVec3LuaObject);
	void SetNotificationEnabled(LuaPlus::LuaObject i_bEnable);
	/*******************/

	static const std::string METATABLE_NAME;
};


inline void PhysicsComponent::SetMass( real i_fMass)
{
	m_oObjectData.m_fMass = i_fMass;
}

inline void PhysicsComponent::SetRestitution( real i_fRestitution)
{
	m_oObjectData.m_fRestitution = i_fRestitution;
}

inline void PhysicsComponent::SetFriction( real i_fFriction)
{
	m_oObjectData.m_fFriction = i_fFriction;
}

inline void PhysicsComponent::SetLinearDamping( real i_fLinearDamping)
{
	m_oObjectData.m_fLinearDamping = i_fLinearDamping;
}

inline void PhysicsComponent::SetAngularDamping( real i_fAngularDamping)
{
	m_oObjectData.m_fAngularDamping = i_fAngularDamping;
}

inline void PhysicsComponent::SetRollingFriction( real i_fRollingFriction)
{
	m_oObjectData.m_fRollingFriction = i_fRollingFriction;
}

inline void PhysicsComponent::SetCollisionShape( PhysicsObjectData::CollisionShape i_oCollisionShape, const Vec3& i_oDimCollisionShape, const Vec3& i_oScaleCollisionShape)
{
	m_oObjectData.m_oCollisionShape = i_oCollisionShape;
	m_oObjectData.m_oDimCollisionShape = i_oDimCollisionShape * i_oScaleCollisionShape;
}

inline bool PhysicsComponent::IsInCollision()
{
	return m_bIsInCollision;
}

inline void PhysicsComponent::SetCollisionData(PhysicsComponent::CollisionData i_oCollisionInfo)
{
	m_oCollisionInfo = i_oCollisionInfo;
}

inline const PhysicsComponent::CollisionData& PhysicsComponent::GetCollisionData() const
{
	return m_oCollisionInfo;
}

inline bool PhysicsComponent::IsNotificationEnabled() const
{
	return m_bNotifyCollision;
}

inline real PhysicsComponent::GetMass() const
{
	return m_oObjectData.m_fMass;
}

inline real PhysicsComponent::GetFriction() const
{
	return m_oObjectData.m_fFriction;
}

inline real PhysicsComponent::GetRestitution() const
{
	return m_oObjectData.m_fRestitution;
}

inline real PhysicsComponent::GetLinearDamping() const
{
	return m_oObjectData.m_fLinearDamping;
}

inline real PhysicsComponent::GetAngularDamping() const
{
	return m_oObjectData.m_fAngularDamping;
}

inline real PhysicsComponent::GetRollingFriction() const
{
	return m_oObjectData.m_fRollingFriction;
}

inline int32 PhysicsComponent::GetCollisionFlag() const
{
	return m_CollisionFlags;
}

inline const PhysicsObjectData& PhysicsComponent::GetObjectData() const
{
	return m_oObjectData;
}