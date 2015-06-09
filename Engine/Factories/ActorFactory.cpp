#include "ActorFactory.h"
#include "../Manager\LuaManager.h"
#include "tinyxml2.h"
#include "..\OtherComponents\TransformationComponent.h"
#include "..\Physics\PhysicsComponent.h"
#include "..\Graphics\BaseGfxComponent.h"
#include "..\Script\ScriptComponent.h"
#include "..\Graphics\LightComponent.h"
#include "..\Graphics\CameraComponent.h"
#include "../LOG/LOGManager.h"
#include "..\GUI\GUIViewComponent.h"
#include "..\Graphics\MeshGfxComponent.h"
#include "..\Graphics\SpriteComponent.h"
#include "..\Animation\AnimatorComponent.h"

void ActorFactory::CreateInstance()
{
	if(GetSingletonPtr() == NULL)
		new ActorFactory();
}

ActorFactory::ActorFactory()
{
	RegisterGlobalLuaFunction();
	RegisterComponents();
}

ActorFactory::~ActorFactory()
{
}

void ActorFactory::RegisterGlobalLuaFunction()
{
	LuaManager::GetSingleton().GetGlobalVars().RegisterDirect("LoadActorFile", &ActorFactory::LoadFile);
	LuaManager::GetSingleton().GetGlobalVars().RegisterDirect("UnloadActorFile", &ActorFactory::UnloadFile);
	LuaManager::GetSingleton().GetGlobalVars().RegisterDirect("RemoveActor", &ActorFactory::RemoveActor);

	LuaManager::GetSingleton().GetGlobalVars().RegisterDirect("CreateActorFromTemplate", &ActorFactory::CreateActorFromTemplate);
}


void ActorFactory::RegisterComponents()
{
	m_Factory.Register<TransformationComponent>(TransformationComponent::ID);
	m_Factory.Register<PhysicsComponent>(PhysicsComponent::ID);
	m_Factory.Register<BaseGfxComponent>(BaseGfxComponent::ID);	
	m_Factory.Register<MeshGfxComponent>(MeshGfxComponent::ID);	
	m_Factory.Register<ScriptComponent>(ScriptComponent::ID);
	m_Factory.Register<CameraComponent>(CameraComponent::ID);
	m_Factory.Register<LightComponent>(LightComponent::ID);
	m_Factory.Register<GUIViewComponent>(GUIViewComponent::ID);
	m_Factory.Register<SpriteComponent>(SpriteComponent::ID);
	m_Factory.Register<AnimatorComponent>(AnimatorComponent::ID);

}

void ActorFactory::InitComponents(MGDVector<Component*>& i_vNewComponents)
{
	MGDVector<Component*>::iterator it = i_vNewComponents.begin();
	for(; it != i_vNewComponents.end(); ++it)
	{
		(*it)->Init();

		char pEventName[128];
		sprintf_s(pEventName, "Registered/%s", (*it)->GetID().GetDebugName().c_str());
		ActorFactory::GetSingleton().m_oEventPublisher.SetPath(ObjectId(pEventName));
		ActorFactory::GetSingleton().m_oEventPublisher.Raise((*it)->GetTextName().c_str());
	}
}

void ActorFactory::LoadFile(const char* i_szFilename)
{
	if(!i_szFilename)
	{
		MGD_LOG::LOGManager::GetSingleton().WriteLog(MGD_LOG::MGD_ERROR, ACTOR_FACTORY_CONTEXT, "File name incorrect: %s", i_szFilename);
		return;
	}

	tinyxml2::XMLDocument oDocument;
	tinyxml2::XMLError oErr = oDocument.LoadFile(i_szFilename);
	if(oErr != tinyxml2::XML_NO_ERROR)
	{
		MGD_LOG::LOGManager::GetSingleton().WriteLog(MGD_LOG::MGD_ERROR, ACTOR_FACTORY_CONTEXT, "Error loading file: %s", i_szFilename);
		return;
	}
	const tinyxml2::XMLElement* pRoot = oDocument.RootElement();
	if(pRoot)
	{
		MGDVector<Component*> oNewComponents;
		
		//CREATE ACTOR TEMPLATES
		for (const tinyxml2::XMLElement* pTemplateActor = pRoot->FirstChildElement("TemplateActor"); pTemplateActor; pTemplateActor = pTemplateActor->NextSiblingElement("TemplateActor"))
		{
			CreateTemplateActor(pTemplateActor);
		}

		//CREATE ACTOR
		for (const tinyxml2::XMLElement* pActor = pRoot->FirstChildElement("Actor"); pActor; pActor = pActor->NextSiblingElement("Actor"))
		{
			CreateActor(pActor, oNewComponents);			
		}

		InitComponents(oNewComponents);
	}
}

void ActorFactory::CreateTemplateActor(const tinyxml2::XMLElement* i_pTemplateActor)
{
	if (i_pTemplateActor)
	{
		char szTemplateActorName[32]; 
		sprintf_s(szTemplateActorName,i_pTemplateActor->Attribute("Name"));

		for (const tinyxml2::XMLElement* pNode = i_pTemplateActor->FirstChildElement(); pNode; pNode = pNode->NextSiblingElement())
		{
			Component* pComponent = CreateComponent(pNode, szTemplateActorName);
			MGD_ASSERT(pComponent);
			if (pComponent)
			{
				SystemManager::GetSingleton().AddTemplate(ObjectId(szTemplateActorName), pComponent);
			}
		}
	}
}

void ActorFactory::CreateActor(const tinyxml2::XMLElement* i_pActor, MGDVector<Component*>& o_vNewComponents)
{
	if(i_pActor)
	{
		const char* pActorName = i_pActor->Attribute("Name");

		for (const tinyxml2::XMLElement* pNode = i_pActor->FirstChildElement(); pNode; pNode = pNode->NextSiblingElement())
		{
			Component* pComponent = CreateComponent(pNode, pActorName);
#if defined(_DEBUG)
			++NumComponents;
#endif
			MGD_ASSERT(pComponent);
			if (pComponent)
			{
				SystemManager::GetSingleton().AddComponent(pComponent, ObjectId(pActorName));
				o_vNewComponents.push_back(pComponent);
			}
		}
	}
}

Component* ActorFactory::CreateComponent(const tinyxml2::XMLElement* i_pNode, const char* i_pActorName)
{
	if(i_pNode)
	{
		const char* pComponentID = i_pNode->Value();
		Component* pComponent = GetSingleton().m_Factory.Create(ObjectId(pComponentID), i_pActorName);
		if (pComponent)
		{
			if (pComponent->SetupFromXml(i_pNode))
			{
				return pComponent;
			}
		}

		delete(pComponent);
		MGD_LOG::LOGManager::GetSingleton().WriteLog(MGD_LOG::MGD_ERROR, ACTOR_FACTORY_CONTEXT, "Component failed to initialize: %s", pComponentID);
	}
	return NULL;
}

void ActorFactory::CreateActorFromTemplate(const char* i_pTemplateActorName, const char* i_pActorName)
{
	if (i_pTemplateActorName && strlen(i_pTemplateActorName) > 0 && i_pActorName && strlen(i_pActorName) > 0)
	{
		MGDVector<Component*> pTemplateComponents;
		SystemManager::GetSingleton().GetComponentsFromTemplate(ObjectId(i_pTemplateActorName), pTemplateComponents);

		MGDVector<Component*> pNewComponents;
		for (uint32 uiIndex = 0; uiIndex < pTemplateComponents.size(); ++uiIndex)
		{
			Component* pComponent = GetSingleton().m_Factory.Create(pTemplateComponents[uiIndex]->GetID(), i_pActorName, true);
			if (pComponent)
			{
				pComponent->CreateFromTemplate(pTemplateComponents[uiIndex], ObjectId(i_pActorName));
				SystemManager::GetSingleton().AddComponent(pComponent, ObjectId(i_pActorName));

				pNewComponents.push_back(pComponent);
			}
		}

		InitComponents(pNewComponents);
	}
}

void ActorFactory::UnloadFile(const char* i_szFilename)
{
	if(!i_szFilename)
	{
		MGD_LOG::LOGManager::GetSingleton().WriteLog(MGD_LOG::MGD_ERROR, ACTOR_FACTORY_CONTEXT, "File name incorrect: %s", i_szFilename);
		return;
	}

	tinyxml2::XMLDocument oDocument;
	tinyxml2::XMLError oErr = oDocument.LoadFile(i_szFilename);
	if(oErr != tinyxml2::XML_NO_ERROR)
	{
		MGD_LOG::LOGManager::GetSingleton().WriteLog(MGD_LOG::MGD_ERROR, ACTOR_FACTORY_CONTEXT, "File name incorrect: %s", i_szFilename);
		return;
	}
	const tinyxml2::XMLElement* pRoot = oDocument.RootElement();
	if(pRoot)
	{
		for (const tinyxml2::XMLElement* pActor = pRoot->FirstChildElement("Actor"); pActor; pActor = pActor->NextSiblingElement("Actor"))
		{
			const char* pActorName = pActor->Attribute("Name");
			MGD_ASSERT(pActorName);
			if(pActorName)
			{
				SystemManager::GetSingleton().RemoveComponentsByOwnerId(ObjectId(pActorName));
			}
		}
		for (const tinyxml2::XMLElement* pTemplateActor = pRoot->FirstChildElement("TemplateActor"); pTemplateActor; pTemplateActor = pTemplateActor->NextSiblingElement("TemplateActor"))
		{
			const char* pTemplateActorName = pTemplateActor->Attribute("Name");
			MGD_ASSERT(pTemplateActorName);
			if(pTemplateActorName)
			{
				SystemManager::GetSingleton().RemoveTemplate(ObjectId(pTemplateActorName));
			}
		}

		SystemManager::GetSingleton().RemoveTemplateComponents();
	}
}

void ActorFactory::RemoveActor(const char* i_szActor)
{
	SystemManager::GetSingleton().RemoveComponentsByOwnerId(ObjectId(i_szActor));
}