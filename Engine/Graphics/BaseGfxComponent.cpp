#include "BaseGfxComponent.h"
#include "../Common/Utilities.h"
#include "../Manager/LuaManager.h"

ID_IMPLEMENTATION(BaseGfxComponent);

const char* BaseGfxComponent::METATABLE_NAME = "BaseGfxComponentMetatable";

BaseGfxComponent::BaseGfxComponent(std::string i_szOwnerID, bool i_bIsCreatedFromTemplate)
	: IGfxComponent(i_szOwnerID, i_bIsCreatedFromTemplate)
{
	CreateLuaObject();
}

BaseGfxComponent::BaseGfxComponent( std::string i_szOwnerID, PrefabType i_oPrefabType )
	: IGfxComponent(i_szOwnerID)
	, m_oPrefabType(i_oPrefabType)
{
	CreateLuaObject();
}


BaseGfxComponent::~BaseGfxComponent()
{
}

void BaseGfxComponent::Init()
{
}

bool BaseGfxComponent::SetupFromXml( const tinyxml2::XMLElement* pNode )
{
	if(pNode)
	{
		const tinyxml2::XMLElement* pPrefabType = pNode->FirstChildElement("PrefabType");
		if (pPrefabType)
		{
			const char* szPrefabType = pPrefabType->Attribute("value");			
			if (strcmp(szPrefabType, "CUBE") == 0)
			{
				m_oPrefabType = CUBE;
			}
			else if (strcmp(szPrefabType, "SPHERE") == 0)
			{
				m_oPrefabType = SPHERE;
			}
			else if (strcmp(szPrefabType, "PLANE") == 0)
			{
				m_oPrefabType = PLANE;
			}
			else
			{
				MGD_ASSERT(0 && "This prefab type doesn't exist!");
			}
		}
	}

	return IGfxComponent::SetupFromXml(pNode);
}

/************************* LUA *******************/

void BaseGfxComponent::CreateLuaObject()
{
	m_oLuaObject.AssignNewTable(LuaManager::GetSingleton().GetLuaState());

	LuaPlus::LuaObject metaTable =	LuaManager::GetSingleton().GetGlobalVars().GetByName(METATABLE_NAME);

	m_oLuaObject.SetLightUserData("__object", const_cast<BaseGfxComponent*>(this));
	m_oLuaObject.SetMetaTable(metaTable);
}


void BaseGfxComponent::RegisterScriptFunction()
{
	LuaPlus::LuaObject metaTable = LuaManager::GetSingleton().GetGlobalVars().CreateTable(METATABLE_NAME);

	metaTable.SetObject("__index", metaTable);	
}