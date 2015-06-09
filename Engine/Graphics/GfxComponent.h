/*
*	This is the base class for the compoenents BaseGfxComponent and MeshGfxComponent
*/

#pragma once
#include "../Core/Component.h"

class GfxComponent : public Component
{
public:
	ID_DECLARATION;

	GfxComponent(std::string i_szOwnerID);
	virtual ~GfxComponent();

	virtual void Init();

	void SetMeshName(const std::string& i_szMeshName);
	const std::string& GetMeshName() const
	{
		return m_szMeshName;
	}

	void SetMaterialName(const std::string& i_szMaterialName);
	const std::string& GetMaterialName() const
	{
		return m_szMaterialName;
	}

	void SetShadowEnabled(bool i_bShadowEnabled);
	bool GetShadowEnabled() const
	{
		return m_bShadowEnabled;
	}


private:
	std::string	m_szMeshName;
	std::string	m_szMaterialName;
	bool		m_bShadowEnabled;

	static const std::string NO_MESH;
	static const std::string NO_MATERIAL;
};

