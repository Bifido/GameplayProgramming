#pragma once
#include "Interface\IGUIWidget.h"

namespace CEGUI
{
	class CheckBox;
}

class GUIViewComponent;

class CheckBox : public IGUIWidgets
{
public:
	CheckBox(GUIViewComponent* i_pParent);
	virtual ~CheckBox();

	virtual bool SetupFromXml(const tinyxml2::XMLElement* pNode);
	virtual void SetupFromScript(LuaPlus::LuaObject i_oScript);

	//void SetOnSelectScriptFunction(LuaPlus::LuaObject oScriptFunc);
	bool OnCheckStateChanged(const CEGUI::EventArgs& i_oParam);

	static void RegisterScriptFunctions();
	static CEGUI::Checkbox* Create(IGUIWidgets* i_oWidget, CEGUI::Window* i_pObject);
private:
	void SetTextFromScript(const char* i_szText);
	void SetSelectedFromScript(bool i_bSelected);
	bool IsSelectedFromScript();

	void SetCheckable(bool i_bCheckable);

	void CreateLuaObject();
	bool m_bSelected;
	bool m_bCheckable;
	CEGUI::Checkbox* m_pObject;

	static const std::string CHECKBOX_WIDGET_TABLE;
};