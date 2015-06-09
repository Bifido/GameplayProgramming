#include "ScriptSystem.h"
#include "../Manager\LuaManager.h"
#include "..\OtherComponents\TransformationComponent.h"
#include "../Manager\SystemManager.h"
#include "..\Script\ScriptComponent.h"
#include "../LOG/LOGManager.h"

ID_IMPLEMENTATION(ScriptSystem);

ScriptSystem::ScriptSystem()
	:System()	
{
	ScriptComponent::RegisterScriptFuncitons();
}

ScriptSystem::~ScriptSystem()
{
	MGD_LOG::LOGManager::GetSingleton().WriteLog(MGD_LOG::MGD_INFO, SYSTEMS_CONTEXT, "ScriptSystem destroyed!");
}

void ScriptSystem::Init()
{
	RegisterCreationObject();
	RegisterScriptFunction();
	MGD_LOG::LOGManager::GetSingleton().WriteLog(MGD_LOG::MGD_INFO, SYSTEMS_CONTEXT, "ScriptSystem initialized!");
}

void ScriptSystem::RegisterCreationObject()
{
	LuaPlus::LuaObject pGlobalVars = LuaManager::GetSingleton().GetGlobalVars();
	//pGlobalVars.RegisterDirect("CreateTransformComponent", &TransformationComponent::CreateFromScript);		
}

void ScriptSystem::RegisterScriptFunction()
{
	LuaPlus::LuaObject pGlobalVars = LuaManager::GetSingleton().GetGlobalVars();
	pGlobalVars.RegisterDirect("GetComponent", &ScriptSystem::GetLuaObject);
	pGlobalVars.RegisterDirect("GetComponentByName", &ScriptSystem::GetLuaObjectByName);
	//pGlobalVars.RegisterDirect("Attach", &ScriptSystem::AttachScriptComponent);
}

void ScriptSystem::Update( real i_fFrametime, real i_fTimestep )
{
	const EntityComponentTable* pScriptComponentMap=  SystemManager::GetSingleton().GetComponentTable(ScriptComponent::ID);
	if(pScriptComponentMap)
	{
		for(EntityComponentTable::const_iterator ecIt= pScriptComponentMap->begin(); ecIt != pScriptComponentMap->end(); ++ecIt)
		{
			const MGDVector<Component*>& vComponents(ecIt->second);
			MGDVector<Component*>::const_iterator itComponent = vComponents.begin();
			for(; itComponent != vComponents.end(); ++itComponent)
			{
				ScriptComponent* pScriptComponent= static_cast<ScriptComponent*>((*itComponent));
				if(pScriptComponent)
				{
					if(pScriptComponent->GetState() == ScriptComponent::UNINITILIZE)
					{
						pScriptComponent->OnEnter();
					}
					else if(pScriptComponent->GetState() == ScriptComponent::RUNNING)
					{
						pScriptComponent->OnUpdate(i_fFrametime, i_fTimestep);
					}
					else if(pScriptComponent->IsEnded())
					{
						pScriptComponent->OnExit();
					}
				}
			}
		}
	}
}

LuaPlus::LuaObject ScriptSystem::GetLuaObject( const char* i_pEntityName, const char* i_pComponentId )
{
	Component* pComponent = SystemManager::GetSingleton().EditSingleComponent(ObjectId(i_pComponentId), ObjectId(i_pEntityName));
	MGD_ASSERT(pComponent);
	if(pComponent)
	{
		return pComponent->GetLuaObject();	
	}

	return LuaPlus::LuaObject();
}

LuaPlus::LuaObject ScriptSystem::GetLuaObjectByName( const char* i_pEntityName, const char* i_pComponentId, const char* i_pComponentName )
{
	Component* pComponent = pComponent = SystemManager::GetSingleton().EditComponent(ObjectId(i_pComponentId), ObjectId(i_pComponentName), ObjectId(i_pEntityName));
	MGD_ASSERT(pComponent);
	if(pComponent)
	{
		return pComponent->GetLuaObject();	
	}

	return LuaPlus::LuaObject();
}

void ScriptSystem::AttachScriptComponent(LuaPlus::LuaObject i_oScriptProcess)
{
//	LuaPlus::LuaObject temp = i_oScriptProcess.Lookup("__object");
//	if (!temp.IsNil())
//	{
//		ScriptComponent* pScriptComponent(static_cast<ScriptComponent*>(temp.GetLightUserData()));
//		(pScriptComponent);
//	}
}