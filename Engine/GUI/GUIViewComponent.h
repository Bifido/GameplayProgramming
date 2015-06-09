/*
*
*
*	Lua Functions:
*
*	LuaObject GetWidget(const char* Name) -> LuaObject = Widget
*
*/

#pragma once
#include "../Core/Component.h"
#include "../Common/Utilities.h"
#include "LuaObject.h"
#include "CEGUIEventArgs.h"
#include "../Manager/LuaManager.h"

class IGUIWidgets;

class GUIViewComponent : public Component
{
public:
	ID_DECLARATION;

	GUIViewComponent(std::string i_szOwnerID, bool i_bIsCreatedFromTemplate = false);

	virtual ~GUIViewComponent();

	virtual void Init();
	virtual bool SetupFromXml(const tinyxml2::XMLElement* pNode);
	virtual void CreateFromTemplate(Component* i_pComponent, const ObjectId& i_oOwner)
	{
		if (i_pComponent)
		{
			GUIViewComponent* pGUIViewComponent = static_cast<GUIViewComponent*>(i_pComponent);
			if (pGUIViewComponent)
			{
				m_szLayoutName = pGUIViewComponent->GetLayoutName();
				m_szScript = pGUIViewComponent->GetScript();
				m_GUIWidgets = pGUIViewComponent->GetGUIWidgets();
				Component::CreateFromTemplate(i_oOwner);
			}
		}
	}
	
	bool IsInit();
	void SetInit();

	const MGDVector<IGUIWidgets*>&	GetGUIWidgets() const;
	const std::string&	GetLayoutName() const;
	const std::string&	GetScript() const;

	static void RegisterScriptFunctions();
private:
	void SetupFromScript( LuaPlus::LuaObject i_oScript );
	void CreateLuaObject();
	LuaPlus::LuaObject GetLuaWidget(const char* i_szName);

	std::string				m_szLayoutName;
	std::string				m_szScript;
	MGDVector<IGUIWidgets*> m_GUIWidgets;
	bool m_bInit;

	static const std::string GUI_COMPONENT_TABLE;
};

inline const std::string&	GUIViewComponent::GetLayoutName() const
{
	return m_szLayoutName;
}

inline const std::string&	GUIViewComponent::GetScript() const
{
	return m_szScript;
}

inline bool GUIViewComponent::IsInit()
{
	return m_bInit;
}

inline void GUIViewComponent::SetInit()
{
	m_bInit = true;
}

inline const MGDVector<IGUIWidgets*>& GUIViewComponent::GetGUIWidgets() const
{
	return m_GUIWidgets;
}