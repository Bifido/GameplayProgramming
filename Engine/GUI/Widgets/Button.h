#pragma once
#include "LuaPlus.h"
#include "CEGUIEventArgs.h"
#include "Interface\IGUIWidget.h"

namespace CEGUI
{
	class Window;
	class PushButton;
}

class GUIViewComponent;

class Button : public IGUIWidgets
{
public:
	Button(GUIViewComponent* i_pParent);
	virtual ~Button();

	virtual bool SetupFromXml( const tinyxml2::XMLElement* pNode );
	virtual void SetupFromScript( LuaPlus::LuaObject i_oScript );


	void SetOnClickScriptFunction( LuaPlus::LuaObject oScriptFunc );
	bool OnClick(const CEGUI::EventArgs& i_oParam);

	static void RegisterScriptFunctions();
	static CEGUI::PushButton* Create( IGUIWidgets* i_oWidget, CEGUI::Window* i_pObject );

private:
	void CreateLuaObject();

	CEGUI::PushButton* m_pObject;

	static const std::string BUTTON_WIDGET_TABLE;
};