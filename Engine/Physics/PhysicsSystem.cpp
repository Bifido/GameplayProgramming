#include "PhysicsSystem.h"
#include "../Common/Utilities.h"
#include "../Physics/PhysicsWorld.h"
#include "../Physics/PhysicsComponent.h"
#include "../Manager/SystemManager.h"
#include "../LOG/LOGManager.h"
#include "../Core/Actor.h"
#include "../Script/ScriptComponent.h"

ID_IMPLEMENTATION(PhysicsSystem)

PhysicsSystem::PhysicsSystem(PhysicsWorldFactory* i_pFactory)
: System()
{
	m_oRegisterEvent.Subscribe(this,&PhysicsSystem::CreateObjects,ObjectId("Registered/PhysicsComponent"));
	m_oUnregisterEvent.Subscribe(this,&PhysicsSystem::DeleteObjects,ObjectId("Test/Unregistered/PhysicsComponent"));

	m_pPhysicWorld = i_pFactory->CreateWorld();
}


PhysicsSystem::~PhysicsSystem(void)
{
	m_pPhysicWorld->Cleanup();

	delete(m_pPhysicWorld);

	MGD_LOG::LOGManager::GetSingleton().WriteLog(MGD_LOG::MGD_INFO, SYSTEMS_CONTEXT, "PhysicsSystem destroyed!");
}

void PhysicsSystem::Init()
{
	m_pPhysicWorld->Configure();
	
	MGD_LOG::LOGManager::GetSingleton().WriteLog(MGD_LOG::MGD_INFO, SYSTEMS_CONTEXT, "PhysicsSystem initialized!");
}

void PhysicsSystem::CreateObjects(const char* i_szName)
{
	const EntityComponentTable* pPhysicsComponentMap=  SystemManager::GetSingleton().GetComponentTable(PhysicsComponent::ID);
	if(pPhysicsComponentMap == NULL)
	{	
		MGD_LOG::LOGManager::GetSingleton().WriteLog(MGD_LOG::MGD_INFO, SYSTEMS_CONTEXT, "No PhysicComponents in scene");		
	}
	else
	{
		for(EntityComponentTable::const_iterator ecIt= pPhysicsComponentMap->begin(); ecIt != pPhysicsComponentMap->end(); ++ecIt)
		{
			const MGDVector<Component*>& vComponents(ecIt->second);
			MGDVector<Component*>::const_iterator itComponent = vComponents.begin();
			for(; itComponent != vComponents.end(); ++itComponent)
			{
				if((*itComponent)->GetTextName().compare(i_szName) == 0)
				{
					PhysicsComponent* pPhysicsComponent= static_cast<PhysicsComponent*>((*itComponent));
					if(pPhysicsComponent && !pPhysicsComponent->IsInitialize())
					{
						if(m_pPhysicWorld)
						{
							m_pPhysicWorld->AddObject(pPhysicsComponent->GetPhysicsObject());
						}

						pPhysicsComponent->SetInit();
					}
				}
			}
		}
	}
}

void PhysicsSystem::DeleteObjects(const char* i_szName)
{
	const EntityComponentTable* pPhysicsComponentMap=  SystemManager::GetSingleton().GetComponentTable(PhysicsComponent::ID);
	if(pPhysicsComponentMap)
	{
		for(EntityComponentTable::const_iterator ecIt = pPhysicsComponentMap->begin(); ecIt != pPhysicsComponentMap->end(); ++ecIt)
		{
			const MGDVector<Component*>& vComponents(ecIt->second);
			MGDVector<Component*>::const_iterator itComponent = vComponents.begin();
			for(; itComponent != vComponents.end(); ++itComponent)
			{
				if((*itComponent)->GetTextName().compare(i_szName) == 0)
				{
					PhysicsComponent* pPhysicsComponent= static_cast<PhysicsComponent*>(*itComponent);
					if(pPhysicsComponent)
					{
						m_pPhysicWorld->RemoveObject(pPhysicsComponent->GetPhysicsObject());
					}	
				}
			}		
		}
	}
}

void PhysicsSystem::NotifyCollisions()
{
	const EntityComponentTable* pPhysicsComponentMap=  SystemManager::GetSingleton().GetComponentTable(PhysicsComponent::ID);
	if(pPhysicsComponentMap == NULL)
	{	
		MGD_LOG::LOGManager::GetSingleton().WriteLog(MGD_LOG::MGD_INFO, SYSTEMS_CONTEXT, "No PhysicComponents in scene");		
	}
	else
	{
		for(EntityComponentTable::const_iterator ecIt= pPhysicsComponentMap->begin(); ecIt != pPhysicsComponentMap->end(); ++ecIt)
		{
			const MGDVector<Component*>& vComponents(ecIt->second);
			MGDVector<Component*>::const_iterator itComponent = vComponents.begin();
			for(; itComponent != vComponents.end(); ++itComponent)
			{
				PhysicsComponent* pPhysicsComponent= static_cast<PhysicsComponent*>(*itComponent);
				if(pPhysicsComponent && pPhysicsComponent->IsInCollision() && pPhysicsComponent->IsInitialize())
				{
					const EntityComponentTable* pScriptComponentMap=  SystemManager::GetSingleton().GetComponentTable(ScriptComponent::ID);
					if(pScriptComponentMap != NULL)
					{
						for(EntityComponentTable::const_iterator ItScriptCompEntity = pScriptComponentMap->begin(); ItScriptCompEntity != pScriptComponentMap->end(); ++ItScriptCompEntity)
						{
							const MGDVector<Component*>& vScriptComponents(ItScriptCompEntity->second);
							MGDVector<Component*>::const_iterator itScriptComponent = vScriptComponents.begin();
							for(; itScriptComponent != vScriptComponents.end(); ++itScriptComponent)
							{
								ScriptComponent* pScriptComponent = static_cast<ScriptComponent*>(*itScriptComponent);
								if(pScriptComponent)
								{
									pScriptComponent->OnCollision(pPhysicsComponent->GetCollisionData());
									pPhysicsComponent->ResetCollision();
								}
							}
						}
					}
				}
			}
		}
	}
}

void PhysicsSystem::Update( real /*i_fFrametime*/, real i_fTimestep )
{
	m_pPhysicWorld->Step(i_fTimestep);
	m_pPhysicWorld->PostStep();

	NotifyCollisions();
}
