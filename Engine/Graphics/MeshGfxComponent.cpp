#include "MeshGfxComponent.h"
#include "../Common/Utilities.h"
#include "../Manager/LuaManager.h"


ID_IMPLEMENTATION(MeshGfxComponent);

const std::string MeshGfxComponent::NO_MESH("NO_MESH");
const char* MeshGfxComponent::METATABLE_NAME = "MeshGfxComponentMetatable";

MeshGfxComponent::MeshGfxComponent(std::string i_szOwnerID, bool i_bIsCreatedFromTemplate)
	: IGfxComponent(i_szOwnerID, i_bIsCreatedFromTemplate)
	, m_szMeshName(NO_MESH)
{
	CreateLuaObject();
}


MeshGfxComponent::~MeshGfxComponent()
{
}

void MeshGfxComponent::Init()
{

}

bool MeshGfxComponent::SetupFromXml( const tinyxml2::XMLElement* pNode )
{
	if(pNode)
	{
		const tinyxml2::XMLElement* pMeshName = pNode->FirstChildElement("MeshName");
		if (pMeshName)
		{			
			m_szMeshName = std::string(pMeshName->Attribute("value"));
		}
	}

	return IGfxComponent::SetupFromXml(pNode);
}

void MeshGfxComponent::SetMeshName( const std::string& i_szMeshName )
{
	m_szMeshName = i_szMeshName;
}

/************************* LUA *******************/

void MeshGfxComponent::CreateLuaObject()
{
	m_oLuaObject.AssignNewTable(LuaManager::GetSingleton().GetLuaState());

	LuaPlus::LuaObject metaTable =	LuaManager::GetSingleton().GetGlobalVars().GetByName(METATABLE_NAME);

	m_oLuaObject.SetLightUserData("__object", const_cast<MeshGfxComponent*>(this));
	m_oLuaObject.SetMetaTable(metaTable);
}


void MeshGfxComponent::RegisterScriptFunction()
{
	LuaPlus::LuaObject metaTable = LuaManager::GetSingleton().GetGlobalVars().CreateTable(METATABLE_NAME);

	metaTable.SetObject("__index", metaTable);	
}