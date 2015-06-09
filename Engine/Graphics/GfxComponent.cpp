#include "GfxComponent.h"
#include "../Common/DataTypes.h"

ID_IMPLEMENTATION(GfxComponent);

const std::string GfxComponent::NO_MESH("NO_MESH");
const std::string GfxComponent::NO_MATERIAL("NO_MATERIAL");

GfxComponent::GfxComponent(std::string i_szOwnerID)
	: Component(i_szOwnerID)
	, m_bShadowEnabled(false)
	, m_szMeshName(NO_MESH)
	, m_szMaterialName(NO_MATERIAL)
{

}


GfxComponent::~GfxComponent()
{
}

void GfxComponent::Init()
{

}

void GfxComponent::SetMeshName( const std::string& i_szMeshName )
{
	m_szMeshName = i_szMeshName;
}

void GfxComponent::SetMaterialName( const std::string& i_szMaterialName )
{
	m_szMaterialName = i_szMaterialName;
}

void GfxComponent::SetShadowEnabled( bool i_bShadowEnabled )
{
	m_bShadowEnabled = i_bShadowEnabled;
}

