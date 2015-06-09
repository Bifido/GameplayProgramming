#include "LightComponent.h"
#include "../Common/Utilities.h"
#include "../Manager/LuaManager.h"

ID_IMPLEMENTATION(LightComponent);

const std::string LightComponent::LIGHT_COMPONENT_METATABLE("LightComponentMetatable");

LightComponent::LightComponent(std::string i_szOwnerID, bool i_bIsCreatedFromTemplate)
	: Component(i_szOwnerID, i_bIsCreatedFromTemplate)
	, m_oDiffuseColor()
	, m_oSpecularColor()
	, m_oType(Ogre::Light::LightTypes::LT_DIRECTIONAL)
	, m_oShadowTecnique(Ogre::SHADOWTYPE_NONE)
	, m_bIsEnable(false)
	, m_fDegreeX(0.0f)
	, m_fDegreeY(0.0f)
{
	CreateLuaObject();
}


LightComponent::~LightComponent()
{
}

void LightComponent::Init()
{

}

bool LightComponent::SetupFromXml( const tinyxml2::XMLElement* pNode )
{
	if(pNode)
	{
		const tinyxml2::XMLElement* pDiffuseColor = pNode->FirstChildElement("DiffuseColor");
		if (pDiffuseColor)
		{
			real r = pDiffuseColor->FloatAttribute("r");
			real g = pDiffuseColor->FloatAttribute("g");
			real b = pDiffuseColor->FloatAttribute("b");
			//real a = pDiffuseColor->FloatAttribute("a");

			SetDiffuseColor(Ogre::ColourValue(r,g,b));
		}
		const tinyxml2::XMLElement* pSpecularColor = pNode->FirstChildElement("SpecularColor");
		if (pSpecularColor)
		{
			real r = pSpecularColor->FloatAttribute("r");
			real g = pSpecularColor->FloatAttribute("g");
			real b = pSpecularColor->FloatAttribute("b");
			//real a = pSpecularColor->FloatAttribute("a");

			SetSpecularColor(Ogre::ColourValue(r,g,b));
		}
		const tinyxml2::XMLElement* pType = pNode->FirstChildElement("Type");
		if (pType)
		{
			const char* pszType = pType->Attribute("value");
			if(strcmp(pszType, "Directional") == 0)
			{
				SetType(Ogre::Light::LT_DIRECTIONAL);
			}
			else if(strcmp(pszType, "Point") == 0)
			{
				SetType(Ogre::Light::LT_POINT);
			}
			else if(strcmp(pszType, "Spotlight") == 0)
			{
				SetType(Ogre::Light::LT_SPOTLIGHT);

				real fDegreeX = 0.0f;
				real fDegreeY = 0.0f;

				fDegreeX = pType->FloatAttribute("degreeX");
				fDegreeY = pType->FloatAttribute("degreeY");

				SetRange(fDegreeX, fDegreeY);
			}
			else
			{
				MGD_ASSERT(0 && "Not valid light type!");
			}
			
		}
		const tinyxml2::XMLElement* pEnable = pNode->FirstChildElement("Enable");
		if (pEnable)
		{
			SetEnable(pEnable->BoolAttribute("value"));
		}
	}

	return Component::SetupFromXml(pNode);
}

void LightComponent::RegisterScriptFunction()
{
	LuaPlus::LuaObject metaTable = LuaManager::GetSingleton().GetGlobalVars().CreateTable(LIGHT_COMPONENT_METATABLE.c_str());
	metaTable.SetObject("__index", metaTable); // it’s also its own metatable

	metaTable.RegisterObjectDirect("SetEnable", (LightComponent*)0, &LightComponent::SetEnable);
	metaTable.RegisterObjectDirect("IsEnable", (LightComponent*)0, &LightComponent::IsEnable);
	metaTable.RegisterObjectDirect("SetDiffuseColor", (LightComponent*)0, &LightComponent::SetDiffuseColorFromScript);
	metaTable.RegisterObjectDirect("SetSpecularColor", (LightComponent*)0, &LightComponent::SetSpecularColorFromScript);
}

void LightComponent::CreateLuaObject()
{
	m_oLuaObject.AssignNewTable(LuaManager::GetSingleton().GetLuaState());

	LuaPlus::LuaObject metaTable =	LuaManager::GetSingleton().GetGlobalVars().GetByName(LIGHT_COMPONENT_METATABLE.c_str());

	m_oLuaObject.SetLightUserData("__object", const_cast<LightComponent*>(this));
	m_oLuaObject.SetMetaTable(metaTable);
}
