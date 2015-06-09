#pragma once
#include "../Core/Component.h"

class IGfxComponent : public Component
{
public:
	ID_DECLARATION;
	IGfxComponent(std::string i_szOwnerID, bool i_bIsCreatedFromTemplate = false);
	virtual ~IGfxComponent();

	virtual void Init() = 0;
	virtual bool SetupFromXml(const tinyxml2::XMLElement* pNode);

	virtual void CreateFromTemplate(Component* i_pComponent, const ObjectId& i_oOwner)
	{
		if (i_pComponent)
		{
			IGfxComponent* pGfxComponent = static_cast<IGfxComponent*>(i_pComponent);
			if (pGfxComponent)
			{
				m_szMaterialName = pGfxComponent->GetMaterialName();
				m_bShadowEnabled = pGfxComponent->GetShadowEnabled();
				Component::CreateFromTemplate(i_oOwner);
			}
		}
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
	std::string	m_szMaterialName;
	bool		m_bShadowEnabled;

	static const std::string NO_MATERIAL;
};

