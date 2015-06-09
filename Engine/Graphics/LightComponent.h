/*
*	This class contains the property fo each lights in game
*
*
*	Lua Functions:
*	void SetEnabled(bool)
*	bool IsEnabled(bool)
*	void SetDiffuseColor(DiffuseColor) -> DiffuseColor = LuaObject = {r = val, g = val, b = val, a = val}
*	void SetSpecularColor(SpecularColor) -> SpecularColor = LuaObject = {r = val, g = val, b = val, a = val}
*
*
*/

#pragma once
#include "../Core/Component.h"
#include "OgreColourValue.h"
#include "OgreCommon.h"
#include "OgreLight.h"
#include "../Script/LuaUtilities.h"

class LightComponent : public Component
{
public:
	ID_DECLARATION;

	LightComponent(std::string i_szOwnerID, bool i_bIsCreatedFromTemplate = false);
	virtual ~LightComponent();

	virtual void Init();
	virtual bool SetupFromXml(const tinyxml2::XMLElement* pNode);
	virtual void CreateFromTemplate(Component* i_pComponent, const ObjectId& i_oOwner)
	{
		if (i_pComponent)
		{
			LightComponent* pLightComponent = static_cast<LightComponent*>(i_pComponent);
			if (pLightComponent)
			{
				m_oDiffuseColor = pLightComponent->GetDiffuseColor();
				m_oSpecularColor = pLightComponent->GetSpecularColor();
				m_oType = pLightComponent->GetType();
				m_oShadowTecnique = pLightComponent->GetShadowTecnique();
				m_bIsEnable = pLightComponent->IsEnable();
				m_fDegreeX = pLightComponent->GetRangeX();
				m_fDegreeY = pLightComponent->GetRangeY();
				Component::CreateFromTemplate(i_oOwner);
			}
		}
	}

	const Ogre::ColourValue& GetDiffuseColor() const;	
	void SetDiffuseColor(const Ogre::ColourValue& i_oColor);
	void SetDiffuseColorFromScript(LuaPlus::LuaObject i_oLuaObject);

	const Ogre::ColourValue& GetSpecularColor() const;	
	void SetSpecularColor(const Ogre::ColourValue& i_oColor);
	void SetSpecularColorFromScript(LuaPlus::LuaObject i_oLuaObject);

	const Ogre::ShadowTechnique& GetShadowTecnique() const;
	void SetShadowtecnique( const Ogre::ShadowTechnique& i_oShadowTechnique );

	const Ogre::Light::LightTypes& GetType() const;	
	void SetType(const Ogre::Light::LightTypes& i_oType);

	real GetRangeX() const;
	real GetRangeY() const;
	void SetRange( real i_fDegreeX, real i_fDegreeY);

	bool IsEnable() const;
	void SetEnable(bool i_bIsEnable);

	static void RegisterScriptFunction();
private:
	Ogre::ColourValue			m_oDiffuseColor;
	Ogre::ColourValue			m_oSpecularColor;
	Ogre::Light::LightTypes		m_oType;
	Ogre::ShadowTechnique		m_oShadowTecnique;
	bool						m_bIsEnable;
	real						m_fDegreeX;
	real						m_fDegreeY;

	/* LUA FUNCTIONS */	
	void CreateLuaObject();
	/* ************* */

	static const std::string LIGHT_COMPONENT_METATABLE;
};

inline const Ogre::ColourValue& LightComponent::GetDiffuseColor() const
{
	return m_oDiffuseColor;
}	
inline void LightComponent::SetDiffuseColor(const Ogre::ColourValue& i_oColor)
{
	m_oDiffuseColor = i_oColor;
}

inline void LightComponent::SetDiffuseColorFromScript(LuaPlus::LuaObject i_oLuaObject)
{
	Ogre::ColourValue oColor;
	LuaUtilities::ConvertLuaObjectToColor(i_oLuaObject, oColor);
	SetDiffuseColor(oColor);
}

inline const Ogre::ColourValue& LightComponent::GetSpecularColor() const
{
	return m_oSpecularColor;
}	
inline void LightComponent::SetSpecularColor(const Ogre::ColourValue& i_oColor)
{
	m_oSpecularColor = i_oColor;
}

inline void LightComponent::SetSpecularColorFromScript(LuaPlus::LuaObject i_oLuaObject)
{
	Ogre::ColourValue oColor;
	LuaUtilities::ConvertLuaObjectToColor(i_oLuaObject, oColor);
	SetSpecularColor(oColor);
}

inline const Ogre::ShadowTechnique& LightComponent::GetShadowTecnique() const
{
	return m_oShadowTecnique;
}	

inline void LightComponent::SetShadowtecnique( const Ogre::ShadowTechnique& i_oShadowTechnique )
{
	m_oShadowTecnique = i_oShadowTechnique;
}

inline const Ogre::Light::LightTypes& LightComponent::GetType() const
{
	return m_oType;
}

inline void LightComponent::SetType(const Ogre::Light::LightTypes& i_oType)
{
	m_oType = i_oType;
}

inline real LightComponent::GetRangeX() const
{
	return m_fDegreeX;
}

inline real LightComponent::GetRangeY() const
{
	return m_fDegreeY;
}

inline void LightComponent::SetRange( real i_fDegreeX, real i_fDegreeY )
{
	m_fDegreeX = i_fDegreeX;
	m_fDegreeY = i_fDegreeY;
}

inline bool LightComponent::IsEnable() const
{
	return m_bIsEnable;
}

inline void LightComponent::SetEnable(bool i_bIsEnable)
{
	m_bIsEnable = i_bIsEnable;
}