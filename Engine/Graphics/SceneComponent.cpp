#include "SceneComponent.h"
#include "../Common/Utilities.h"
#include "../Manager/LuaManager.h"

ID_IMPLEMENTATION(SceneComponent);

const char* SceneComponent::METATABLE_NAME = "SceneComponentMetatable";

SceneComponent::SceneComponent(std::string i_szOwnerID)
	: Component(i_szOwnerID)
	, m_szActor(i_szOwnerID)
{
	CreateLuaObject();
}

SceneComponent::~SceneComponent()
{
	MGDVector<SpriteComponent*>::iterator it = m_vSpriteComponents.begin();

	for(; it != m_vSpriteComponents.end(); ++it)
	{
		delete(*it);
	}

	m_vSpriteComponents.clear();
}

void SceneComponent::Init()
{
	oxygine::getRoot()->addChild(&m_oScene);
}

bool SceneComponent::SetupFromXml( const tinyxml2::XMLElement* pNode )
{
	if(pNode)
	{
		for (const tinyxml2::XMLElement* pSpriteActor = pNode->FirstChildElement("SpriteComponent"); pSpriteActor; pSpriteActor = pSpriteActor->NextSiblingElement("SpriteComponent"))
		{
			if(pSpriteActor)
			{
				SpriteComponent* pSprite = new SpriteComponent(m_szActor/*, this*/);
				if(pSprite)
				{
					pSprite->SetupFromXml(pSpriteActor);
				}				

				m_vSpriteComponents.push_back(pSprite);
			}

		}
	}

	return Component::SetupFromXml(pNode);
}

/************************* LUA *******************/

void SceneComponent::CreateLuaObject()
{
	m_oLuaObject.AssignNewTable(LuaManager::GetSingleton().GetLuaState());

	LuaPlus::LuaObject metaTable =	LuaManager::GetSingleton().GetGlobalVars().GetByName(METATABLE_NAME);

	m_oLuaObject.SetLightUserData("__object", const_cast<SceneComponent*>(this));
	m_oLuaObject.SetMetaTable(metaTable);
}


void SceneComponent::RegisterScriptFunction()
{
	LuaPlus::LuaObject metaTable = LuaManager::GetSingleton().GetGlobalVars().CreateTable(METATABLE_NAME);

	metaTable.SetObject("__index", metaTable);	
}