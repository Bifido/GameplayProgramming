#include "SpriteComponent.h"
#include "../Common/Utilities.h"
#include "../Manager/LuaManager.h"
#include "../Resources/ResourceManager.h"
#include "../Common/TextureManager.h"

ID_IMPLEMENTATION(SpriteComponent);

const char* SpriteComponent::METATABLE_NAME = "BaseGfxComponentMetatable";

SpriteComponent::SpriteComponent(std::string i_szOwnerID, bool i_bIsCreatedFromTemplate)
	: Component(i_szOwnerID, i_bIsCreatedFromTemplate)
{
	CreateLuaObject();	
}

SpriteComponent::~SpriteComponent()
{
}

void SpriteComponent::Init()
{
	TextureManager::Inst()->LoadTexture( m_szPath.c_str(), ObjectId(m_szPath.c_str()).GetId());
	TextureManager::Inst()->GetInfo(m_szPath.c_str(), m_fWidth, m_fHeight);
}

bool SpriteComponent::SetupFromXml( const tinyxml2::XMLElement* pNode )
{
	if(pNode)
	{

		const tinyxml2::XMLElement* pImage = pNode->FirstChildElement("Image");
		if (pImage)
		{
			m_szPath = pImage->Attribute("filename");
		}
	}

	return Component::SetupFromXml(pNode);
}

/************************* LUA *******************/

void SpriteComponent::CreateLuaObject()
{
	m_oLuaObject.AssignNewTable(LuaManager::GetSingleton().GetLuaState());

	LuaPlus::LuaObject metaTable =	LuaManager::GetSingleton().GetGlobalVars().GetByName(METATABLE_NAME);

	m_oLuaObject.SetLightUserData("__object", const_cast<SpriteComponent*>(this));
	m_oLuaObject.SetMetaTable(metaTable);
}


void SpriteComponent::RegisterScriptFunction()
{
	LuaPlus::LuaObject metaTable = LuaManager::GetSingleton().GetGlobalVars().CreateTable(METATABLE_NAME);

	metaTable.SetObject("__index", metaTable);	


	metaTable.RegisterObjectDirect("AddTween", (SpriteComponent*)0, &SpriteComponent::AddTweenFromScript);
}

void SpriteComponent::AddTweenFromScript(const char* i_szType, const char* i_szRes, real i_fDuration, bool i_bLoop)
{
	if(i_szType && strlen(i_szType) > 0)
	{
		if(strcmp(i_szType, "animation") == 0)
		{
		}
	}
}