/*
*
*	Lua Functions:
*
*	real GetCurrentValue()
*
*/

#pragma once
#include "Interface\IGUIWidget.h"
#include "..\..\Common\Utilities.h"
#include "elements\CEGUISlider.h"
#include "LuaPlus.h"
#include "CEGUIEventArgs.h"

class GUIViewComponent;

class Slider : public IGUIWidgets
{
public:
	Slider(GUIViewComponent* i_pParent);
	virtual ~Slider();

	virtual bool SetupFromXml( const tinyxml2::XMLElement* pNode );	
	virtual void SetupFromScript( LuaPlus::LuaObject i_oScript );

	static CEGUI::Slider* Create( IGUIWidgets* i_oGUIWidget, CEGUI::Window* i_pObject );

	real GetMaxValue() const;
	real GetClickStep() const;
	real GetCurrentValue() const;

	//void SetOnThumbTrackStartedScriptFunction( LuaPlus::LuaObject oScriptFunc );
	//void SetOnThumbTrackEndedScriptFunction( LuaPlus::LuaObject oScriptFunc );
	//void SetOnValueChangedScriptFunction( LuaPlus::LuaObject oScriptFunc );

	bool OnThumbTrackStarted(const CEGUI::EventArgs& i_oParam);
	bool OnThumbTrackEnded(const CEGUI::EventArgs& i_oParam);
	bool OnValueChanged(const CEGUI::EventArgs& i_oParam);

	static void RegisterScriptFunctions();

private:	
	void CreateLuaObject();

	real m_fMaxValue;
	real m_fClickStep;
	real m_fCurrentValue;
	CEGUI::Slider* m_pObject;

	LuaPlus::LuaObject m_oLuaObject;

	//LuaPlus::LuaObject m_oScriptOnThumbTrackStarted;
	//LuaPlus::LuaObject m_oScriptOnThumbTrackEnded;
	//LuaPlus::LuaObject m_oScriptOnValueChangedFunction;

	static const std::string SLIDER_WIDGET_TABLE;
};

inline real Slider::GetMaxValue() const
{
	return m_fMaxValue;
}

inline real Slider::GetClickStep() const
{
	return m_fClickStep;
}

inline real Slider::GetCurrentValue() const
{
	return m_fCurrentValue;
}