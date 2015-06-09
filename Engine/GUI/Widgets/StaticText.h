/*
*
*	Lua Functions:
*
*	void SetText(const char* Text)
*	void SetInt(int Val)
*
*/

#pragma once
#include "Interface\IGUIWidget.h"
#include "..\..\Common\Utilities.h"
#include "elements\CEGUIGUISheet.h"
#include "CEGUIWindow.h"


class GUIViewComponent;

class StaticText : public IGUIWidgets
{
public:
	StaticText(GUIViewComponent* i_pParent);
	virtual ~StaticText();

	virtual bool SetupFromXml( const tinyxml2::XMLElement* pNode );	
	virtual void SetupFromScript( LuaPlus::LuaObject i_oScript );


	const std::string& GetTextColors() const;
	const std::string& GetVertFormatting() const;
	const std::string& GetHorizFormatting() const;

	static CEGUI::DefaultWindow* Create( IGUIWidgets* i_oGUIWidget, CEGUI::Window* i_pObject );

	static void RegisterScriptFunctions();
private:	
	void SetTextFromScript(const char* i_szText);
	void SetInt(int i_iVal);
	void CreateLuaObject();

	std::string		m_szTextColors;
	std::string		m_szVertFormatting;
	std::string		m_szHorizFormatting;

	CEGUI::DefaultWindow* m_pObject;
};

inline const std::string& StaticText::GetTextColors() const
{
	return m_szTextColors;
}

inline const std::string& StaticText::GetVertFormatting() const
{
	return m_szVertFormatting;
}

inline const std::string& StaticText::GetHorizFormatting() const
{
	return m_szHorizFormatting;
}