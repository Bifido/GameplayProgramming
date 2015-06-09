#include "PhysicsComponent.h"
#include "../Common/Utilities.h"
#include "../Physics/BulletObject.h"
#include "../Physics/MotionState.h"
#include "LinearMath/btMotionState.h"
#include "../OtherComponents/TransformationComponent.h"
#include "../Manager/SystemManager.h"
#include "../Manager/LuaManager.h"
#include "BulletDynamics/Dynamics/btRigidBody.h"
#include "../LOG/LOGManager.h"
#include "../Physics/PhysicsUtilities.h"
#include "../Script/LuaUtilities.h"
#include "../Common/Common.h"
#include "Box2DObject.h"

ID_IMPLEMENTATION(PhysicsComponent);
ID_IMPLEMENTATION(PhysicsComponent::CollisionData)


const std::string PhysicsComponent::METATABLE_NAME = "PhysicsComponentMetatable";

PhysicsComponent::PhysicsComponent(std::string i_szOwnerID, bool i_bIsCreatedFromTemplate)
	: Component(i_szOwnerID, i_bIsCreatedFromTemplate)
	, m_pObject(NULL)
	, m_bIsInCollision(false)
	, m_bNotifyCollision(false)
	, m_CollisionFlags(0)
	, m_eEngineType(eBullet)
{
	CreateLuaObject();

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
		const tinyxml2::XMLElement* pPhysicsEngine = pRoot->FirstChildElement("PhysicsEngine");
		if(pPhysicsEngine)
		{
			const char* szType = pPhysicsEngine->Attribute("type");
			if(strcmp(szType,"Box2D") == 0)
			{
				m_eEngineType = eBox2D;
			}
			else if(strcmp(szType,"Bullet") == 0)
			{
				m_eEngineType = eBullet;
			}
			else
			{
				MGD_ASSERT(0 && "INVALID ENGINE!");
			}
		}
	}
}


PhysicsComponent::~PhysicsComponent()
{
	char pEventName[128];
	sprintf_s(pEventName, "Test/Unregistered/%s", GetID().GetDebugName().c_str());
	RegistryEventPublisher<const char*> m_oEventPublisher;
	m_oEventPublisher.SetPath(ObjectId(pEventName));
	m_oEventPublisher.Raise(GetTextName().c_str());

	delete(m_pObject);
	delete(m_oObjectData.m_pMotionState);
}

void PhysicsComponent::Init()
{
	std::string szEventName;
	szEventName.append(GetOwnerID().GetDebugName().c_str());
	szEventName.append("/POSITION_CHANGED");
	m_oEvent.Subscribe(this, &PhysicsComponent::SyncronizePosition, ObjectId(szEventName.c_str()));

	std::string szCollisionEventName;
	szCollisionEventName.append("Collision/");
	szCollisionEventName.append(GetTextName().c_str());
	m_oCollisionEvent.Subscribe(this, &PhysicsComponent::NotifyCollision, ObjectId(szCollisionEventName.c_str()));
	
	if(m_eEngineType == eBullet)
	{
		m_oObjectData.m_pMotionState = new BulletMotionState(GetName(), GetOwnerID());
		m_pObject = new BulletObject(m_oObjectData, this);
		m_CollisionFlags |= btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK;
		SetCollisionFlag(m_CollisionFlags);
	}
	else if(m_eEngineType == eBox2D)
	{
		m_oObjectData.m_pMotionState = new Box2DMotionState(GetName(), GetOwnerID());
		m_pObject = new Box2DObject(m_oObjectData, this);
		if(m_oType == DYNAMIC)
		{
			const_cast<b2BodyDef&>(static_cast<Box2DObject*>(m_pObject)->GetRigidBodyInfo()).type = b2_dynamicBody;
		}
		else if(m_oType == STATIC)
		{
			const_cast<b2BodyDef&>(static_cast<Box2DObject*>(const_cast<PhysicsObject*>(m_pObject))->GetRigidBodyInfo()).type = b2_staticBody;
		}
		else if(m_oType == KINEMATIC)
		{
			const_cast<b2BodyDef&>(static_cast<Box2DObject*>(const_cast<PhysicsObject*>(m_pObject))->GetRigidBodyInfo()).type = b2_kinematicBody;
		}
	}
}

void PhysicsComponent::NotifyCollision(const PhysicsComponent::CollisionData& i_oCollisionData)
{
	if(m_bNotifyCollision)
	{
		m_oCollisionInfo = i_oCollisionData;
		m_oCollisionInfo.SetupLuaObject();
		m_bIsInCollision = true;
	}
}

void PhysicsComponent::ResetCollision()
{
	m_bIsInCollision = false;
}

void PhysicsComponent::SetCollisionFlag(int32 i_iCollisionFlag)
{
	BulletObject* pBulletObject = static_cast<BulletObject*>(m_pObject);
	if(pBulletObject)
	{
		btRigidBody* pRigidBody = pBulletObject->GetRigidBody();
		if(pRigidBody)
		{
			pRigidBody->setCollisionFlags(pRigidBody->getCollisionFlags() | i_iCollisionFlag);
			if(pRigidBody->isKinematicObject())
			{
				pRigidBody->setActivationState(DISABLE_DEACTIVATION);
			}
		}
	}
}

void PhysicsComponent::SyncronizePosition()	
{
	TransformationComponent* pTransformationComponent= NULL;
	Component* component= SystemManager::GetSingleton().EditSingleComponent(TransformationComponent::ID, GetOwnerID());
	if(component != NULL)
	{
		pTransformationComponent = static_cast<TransformationComponent*>(component);

		if(pTransformationComponent)
		{
			Vec3 oPos = pTransformationComponent->GetPosition();
			Quaternion oRotation = pTransformationComponent->GetRotation();
			
			m_pObject->UpdateTransform(oPos, oRotation);
		}
	}
}

bool PhysicsComponent::SetupFromXml( const tinyxml2::XMLElement* pNode )
{
	if(pNode)
	{
		real val = 0.0f;
		const tinyxml2::XMLElement* pMass = pNode->FirstChildElement("mass");
		if (pMass)
		{
			val = pMass->FloatAttribute("value");
			SetMass(val);
		}
		const tinyxml2::XMLElement* pFriction = pNode->FirstChildElement("friction");
		if (pFriction)
		{
			val = pFriction->FloatAttribute("value");
			SetFriction(val);
		}
		const tinyxml2::XMLElement* pRestitution = pNode->FirstChildElement("restitution");
		if (pRestitution)
		{
			val = pRestitution->FloatAttribute("value");
			SetRestitution(val);
		}
		const tinyxml2::XMLElement* pLinearDamping = pNode->FirstChildElement("linear_damping");
		if (pLinearDamping)
		{
			val = pLinearDamping->FloatAttribute("value");
			SetLinearDamping(val);
		}
		const tinyxml2::XMLElement* pAngularDamping = pNode->FirstChildElement("angular_damping");
		if (pAngularDamping)
		{
			val = pAngularDamping->FloatAttribute("value");
			SetAngularDamping(val);
		}
		const tinyxml2::XMLElement* pRollingFriction = pNode->FirstChildElement("rolling_friction");
		if (pRollingFriction)
		{
			val = pRollingFriction->FloatAttribute("value");
			SetRollingFriction(val);
		}
		const tinyxml2::XMLElement* pCollision = pNode->FirstChildElement("Collision");
		if(pCollision)
		{
			const tinyxml2::XMLElement* pCollisionShape = pCollision->FirstChildElement("shape");
			if (pCollisionShape)
			{
				real x, y, z;
				PhysicsObjectData::CollisionShape oShape = PhysicsObjectData::NONE;

				const char* shape = pCollisionShape->Attribute("shape");
				x = pCollisionShape->FloatAttribute("x");
				y = pCollisionShape->FloatAttribute("y");
				z = pCollisionShape->FloatAttribute("z");
				real scaleX = pCollisionShape->FloatAttribute("scaleX");
				real scaleY = pCollisionShape->FloatAttribute("scaleY");
				real scaleZ = pCollisionShape->FloatAttribute("scaleZ");

				const tinyxml2::XMLElement* pStartPointShape = pCollision->FirstChildElement("StartPoint");
				if (pStartPointShape)
				{
					m_oObjectData.m_oStartPointShape.x = pStartPointShape->FloatAttribute("x");
					m_oObjectData.m_oStartPointShape.y = pStartPointShape->FloatAttribute("y");
					m_oObjectData.m_oStartPointShape.z = pStartPointShape->FloatAttribute("z");
				}
				const tinyxml2::XMLElement* pEndPointShape = pCollision->FirstChildElement("EndPoint");
				if (pEndPointShape)
				{
					m_oObjectData.m_oEndPointShape.x = pEndPointShape->FloatAttribute("x");
					m_oObjectData.m_oEndPointShape.y = pEndPointShape->FloatAttribute("y");
					m_oObjectData.m_oEndPointShape.z = pEndPointShape->FloatAttribute("z");
				}

				if(strcmp(shape,"BOX") == 0)
				{
					oShape = PhysicsObjectData::BOX;
				}
				else if(strcmp(shape,"SPHERE") == 0)
				{
					oShape = PhysicsObjectData::SPHERE;
				}
				else if(strcmp(shape,"CYLINDER") == 0)
				{
					oShape = PhysicsObjectData::CYLINDER;
				}
				else if(strcmp(shape,"CONE") == 0)
				{
					oShape = PhysicsObjectData::CONE;
				}
				else if(strcmp(shape,"CAPSULE") == 0)
				{
					oShape = PhysicsObjectData::CAPSULE;
				}
				else if(strcmp(shape,"CIRCLE") == 0)
				{
					oShape = PhysicsObjectData::CIRCLE;
				}
				else if(strcmp(shape,"EDGE") == 0)
				{
					oShape = PhysicsObjectData::EDGE;
				}

				if(abs(scaleX) <= 0.00005 || abs(scaleY) <= 0.00005 || abs(scaleZ) <= 0.00005)
				{
					SetCollisionShape(oShape, Vec3(x,y,z));
				}
				else
				{
					SetCollisionShape(oShape, Vec3(x,y,z), Vec3(scaleX,scaleY,scaleZ));
				}
			}

			const tinyxml2::XMLElement* pOnCollisionScript = pCollision->FirstChildElement("NotifyCollision");
			if (pOnCollisionScript)
			{
				m_bNotifyCollision = pOnCollisionScript->BoolAttribute("enable");
			}

			const tinyxml2::XMLElement* pType = pCollision->FirstChildElement("CollisionFlag");
			if (pType)
			{
				for (const tinyxml2::XMLElement* pFlag = pType->FirstChildElement("Flag"); pFlag; pFlag = pFlag->NextSiblingElement("Flag"))
				{
					const char* szCollisionFlag = pFlag->Attribute("value");
					if(szCollisionFlag)
					{
						if(strcmp(szCollisionFlag,"DYNAMIC") == 0)
						{
							m_oType = DYNAMIC;
						}
						else if(strcmp(szCollisionFlag,"STATIC") == 0)
						{
							m_oType = STATIC;
							m_CollisionFlags |= btCollisionObject::CF_STATIC_OBJECT;
						}
						else if(strcmp(szCollisionFlag,"KINEMATIC") == 0)
						{
							m_oType = KINEMATIC;
							m_CollisionFlags |= btCollisionObject::CF_KINEMATIC_OBJECT;
						}
						else
						{
							MGD_LOG::LOGManager::GetSingleton().WriteLog(MGD_LOG::MGD_WARNING, COMPONENTS_CONTEXT, "Collision type '%s' is invalid!", szCollisionFlag);
						}
					}
				}
			}
		}
	}

	return Component::SetupFromXml(pNode);
}

void PhysicsComponent::ApplyForce( const Vec3& i_oForce)
{
	if(m_pObject)
	{
		if(m_eEngineType == eBullet)
		{
			BulletObject* pBulletObject = static_cast<BulletObject*>(m_pObject);
			if(pBulletObject)
			{
				btVector3 oForce;
				Physics::Vec3TobtVector3(i_oForce, oForce);
				pBulletObject->GetRigidBody()->activate(true);
				pBulletObject->GetRigidBody()->applyForce(oForce, btVector3(0.0f,0.0f,0.0f));
			}
		}
		else if(m_eEngineType == eBox2D)
		{
			Box2DObject* pBox2DObject = static_cast<Box2DObject*>(m_pObject);
			if(pBox2DObject)
			{
				b2Vec2 oForce;
				Physics::Vec2Tob2Vec2(Vec2(i_oForce.x, i_oForce.y), oForce);
				pBox2DObject->GetRigidBody()->ApplyLinearImpulse(oForce, b2Vec2(0,0), true);
			}
		}
	}
}

void PhysicsComponent::SetVelocity( const Vec3& i_oVelocity)
{
	if(m_pObject)
	{
		if(m_eEngineType == eBullet)
		{
			BulletObject* pBulletObject = static_cast<BulletObject*>(m_pObject);
			if(pBulletObject)
			{
				btVector3 oVel;
				Physics::Vec3TobtVector3(i_oVelocity, oVel);
				pBulletObject->GetRigidBody()->activate(true);
				pBulletObject->GetRigidBody()->setLinearVelocity(oVel);
			}
		}
		else if(m_eEngineType == eBox2D)
		{
			Box2DObject* pBox2DObject = static_cast<Box2DObject*>(m_pObject);
			if(pBox2DObject)
			{
				b2Vec2 oVel;
				Physics::Vec2Tob2Vec2(Vec2(i_oVelocity.x,i_oVelocity.y), oVel);
				pBox2DObject->GetRigidBody()->SetLinearVelocity(oVel);
			}
		}
	}
}

/************************* LUA *******************/

void PhysicsComponent::CreateLuaObject()
{
	m_oLuaObject.AssignNewTable(LuaManager::GetSingleton().GetLuaState());

	LuaPlus::LuaObject metaTable =	LuaManager::GetSingleton().GetGlobalVars().GetByName(METATABLE_NAME.c_str());

	m_oLuaObject.SetLightUserData("__object", const_cast<PhysicsComponent*>(this));
	m_oLuaObject.SetMetaTable(metaTable);
}

void PhysicsComponent::ApplyForceFromScript(LuaPlus::LuaObject i_oVec3LuaObject)
{
	Vec3 oForce;
	LuaUtilities::ConvertLuaObjectToVec3(i_oVec3LuaObject, oForce);

	ApplyForce(oForce);
}

void PhysicsComponent::SetVelocityFromScript(LuaPlus::LuaObject i_oVec3LuaObject)
{
	Vec3 oVelocity;
	LuaUtilities::ConvertLuaObjectToVec3(i_oVec3LuaObject, oVelocity);

	SetVelocity(oVelocity);
}

void PhysicsComponent::SetNotificationEnabled(LuaPlus::LuaObject i_bEnable)
{
	m_bNotifyCollision = i_bEnable.GetBoolean();
	if(!m_bNotifyCollision)
	{
		b2Filter filter;
		filter.categoryBits = 0x0002;
		filter.maskBits = 0x0002;

		b2Fixture* pFixture = static_cast<Box2DObject*>(const_cast<PhysicsObject*>(m_pObject))->GetRigidBody()->GetFixtureList();
		while(pFixture)
		{
			pFixture->SetFilterData(filter);
			pFixture = pFixture->GetNext();
		}
	}
	else
	{
		b2Filter filter;
		filter.categoryBits = 0x0001;
		filter.maskBits = 0xFFFF;

		b2Fixture* pFixture = static_cast<Box2DObject*>(const_cast<PhysicsObject*>(m_pObject))->GetRigidBody()->GetFixtureList();
		while(pFixture)
		{
			pFixture->SetFilterData(filter);
			pFixture = pFixture->GetNext();
		}
	}
}

void PhysicsComponent::RegisterScriptFunction()
{
	LuaPlus::LuaObject metaTable = LuaManager::GetSingleton().GetGlobalVars().CreateTable(METATABLE_NAME.c_str());
	metaTable.SetObject("__index", metaTable);	

	metaTable.RegisterObjectDirect("ApplyForce", (PhysicsComponent*)0, &PhysicsComponent::ApplyForceFromScript);
	metaTable.RegisterObjectDirect("SetVelocity", (PhysicsComponent*)0, &PhysicsComponent::SetVelocityFromScript);
	metaTable.RegisterObjectDirect("GetVelocity", (PhysicsComponent*)0, &PhysicsComponent::GetVelocity);
	metaTable.RegisterObjectDirect("GetMass", (PhysicsComponent*)0, &PhysicsComponent::GetMass);
	metaTable.RegisterObjectDirect("GetFriction", (PhysicsComponent*)0, &PhysicsComponent::GetFriction);
	metaTable.RegisterObjectDirect("GetRestitution", (PhysicsComponent*)0, &PhysicsComponent::GetRestitution);
	metaTable.RegisterObjectDirect("GetLinearDamping", (PhysicsComponent*)0, &PhysicsComponent::GetLinearDamping);
	metaTable.RegisterObjectDirect("GetAngularDamping", (PhysicsComponent*)0, &PhysicsComponent::GetAngularDamping);
	metaTable.RegisterObjectDirect("GetRollingFriction", (PhysicsComponent*)0, &PhysicsComponent::GetRollingFriction);
	metaTable.RegisterObjectDirect("NotifyCollision", (PhysicsComponent*)0, &PhysicsComponent::SetNotificationEnabled);
}

LuaPlus::LuaObject PhysicsComponent::GetVelocity() const
{
	LuaPlus::LuaObject oObejct;
	if(m_pObject)
	{
		if(m_eEngineType == eBullet)
		{
			BulletObject* pBulletObject = static_cast<BulletObject*>(m_pObject);
			if(pBulletObject)
			{
				Vec3 oVelocity;
				Physics::btVector3ToVec3(pBulletObject->GetRigidBody()->getLinearVelocity(),oVelocity);			
				LuaUtilities::ConvertVec3ToLuaObject(oVelocity,oObejct);
			}
		}
		else if(m_eEngineType == eBox2D)
		{
			Box2DObject* pBox2DObject = static_cast<Box2DObject*>(m_pObject);
			if(pBox2DObject)
			{
				Vec2 oVelocity;
				Physics::b2Vec2ToVec2(pBox2DObject->GetRigidBody()->GetLinearVelocity(), oVelocity);			
				LuaUtilities::ConvertVec2ToLuaObject(oVelocity,oObejct);
			}
		}
	}
	return oObejct;
}

/********************************************/