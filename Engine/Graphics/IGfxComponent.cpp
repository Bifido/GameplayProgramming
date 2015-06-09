#include "IGfxComponent.h"
#include "../Common/Utilities.h"

ID_IMPLEMENTATION(IGfxComponent);

const std::string IGfxComponent::NO_MATERIAL("NO_MATERIAL");

IGfxComponent::IGfxComponent(std::string i_szOwnerID, bool i_bIsCreatedFromTemplate)
	: Component(i_szOwnerID, i_bIsCreatedFromTemplate)
	, m_bShadowEnabled(false)
	, m_szMaterialName(NO_MATERIAL)
{

}


IGfxComponent::~IGfxComponent()
{
}

void IGfxComponent::SetMaterialName( const std::string& i_szMaterialName )
{
	m_szMaterialName = i_szMaterialName;
}

void IGfxComponent::SetShadowEnabled( bool i_bShadowEnabled )
{
	m_bShadowEnabled = i_bShadowEnabled;
}

bool IGfxComponent::SetupFromXml( const tinyxml2::XMLElement* pNode )
{
	if(pNode)
	{
		const tinyxml2::XMLElement* pMaterialname = pNode->FirstChildElement("MaterialName");
		if (pMaterialname)
		{
			SetMaterialName(std::string(pMaterialname->Attribute("value")));
		}
		const tinyxml2::XMLElement* pEnableShadow = pNode->FirstChildElement("Shadow");
		if (pEnableShadow)
		{
			SetShadowEnabled(pEnableShadow->BoolAttribute("enable"));
		}
	}

	return Component::SetupFromXml(pNode);
}

