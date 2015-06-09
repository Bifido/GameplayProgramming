#include "CameraComponent.h"
#include "../Common/Utilities.h"
#include "../LOG/LogManager.h"
#include "../Manager/LuaManager.h"

ID_IMPLEMENTATION(CameraComponent);

const std::string CameraComponent::CAMERA_COMPONENT_METATABLE("CameraComponentMetatable");

CameraComponent::CameraComponent(std::string i_szOwnerID, bool i_bIsCreatedFromTemplate)
	: Component(i_szOwnerID, i_bIsCreatedFromTemplate)
	, m_bIsFixedAxis(false)
	, m_uiNearClipDistance(0)
	, m_bIsEnable(false)
	, m_fAspectRatio(16.0f/9.0f)
	, m_bIsOrthographic(false)
{
	CreateLuaObject();
}


CameraComponent::~CameraComponent()
{
}

void CameraComponent::Init()
{

}


void CameraComponent::RegisterScriptFunction()
{
	LuaPlus::LuaObject metaTable = LuaManager::GetSingleton().GetGlobalVars().CreateTable(CAMERA_COMPONENT_METATABLE.c_str());
	metaTable.SetObject("__index", metaTable); // it’s also its own metatable

	metaTable.RegisterObjectDirect("IsEnable", (CameraComponent*)0, &CameraComponent::IsEnable);
	metaTable.RegisterObjectDirect("SetEnable", (CameraComponent*)0, &CameraComponent::SetEnable);
	metaTable.RegisterObjectDirect("SetFixedAxis", (CameraComponent*)0, &CameraComponent::SetFixedAxisFromScript);
}


bool CameraComponent::SetupFromXml( const tinyxml2::XMLElement* pNode )
{
	if(pNode)
	{		
		const tinyxml2::XMLElement* pLookAt = pNode->FirstChildElement("FixedAxis");
		if (pLookAt)
		{
			SetFixedAxis(pLookAt->BoolAttribute("value"));
		}

		const tinyxml2::XMLElement* pNearClipDistance = pNode->FirstChildElement("NearClipDistance");
		if (pNearClipDistance)
		{
			SetNearClipDistance(pNearClipDistance->FloatAttribute("value"));
		}

		const tinyxml2::XMLElement* pEnable = pNode->FirstChildElement("Enable");
		if (pEnable)
		{
			SetEnable(pEnable->BoolAttribute("value"));
		}

		const tinyxml2::XMLElement* pIsOrthographic = pNode->FirstChildElement("IsOrthographic");
		if (pIsOrthographic)
		{
			SetOrthographic(pIsOrthographic->BoolAttribute("value"));
		}
	}

	return Component::SetupFromXml(pNode);
}

void CameraComponent::SetFixedAxisFromScript( LuaPlus::LuaObject i_oObject )
{
	if(i_oObject.IsBoolean())
	{
		SetFixedAxis(i_oObject.GetBoolean());
	}
	else
	{
		MGD_LOG::LOGManager::GetSingleton().WriteLog(MGD_LOG::MGD_WARNING, COMPONENTS_CONTEXT, "Param error: it's not a boolean");
	}
}

void CameraComponent::CreateLuaObject()
{
	m_oLuaObject.AssignNewTable(LuaManager::GetSingleton().GetLuaState());

	LuaPlus::LuaObject metaTable =	LuaManager::GetSingleton().GetGlobalVars().GetByName(CAMERA_COMPONENT_METATABLE.c_str());

	m_oLuaObject.SetLightUserData("__object", const_cast<CameraComponent*>(this));
	m_oLuaObject.SetMetaTable(metaTable);
}
