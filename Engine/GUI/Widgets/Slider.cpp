#include "Slider.h"
#include "..\..\LOG\LOGManager.h"
#include "Interface\IGUIWidget.h"
#include "CEGUIEventSet.h"
#include "CEGUIEvent.h"
#include "..\..\Manager\LuaManager.h"

const std::string Slider::SLIDER_WIDGET_TABLE("SliderWidgetTable");

Slider::Slider(GUIViewComponent* i_pParent)
	: IGUIWidgets(i_pParent)
	, m_fMaxValue(100.0f)
	, m_fClickStep(1.0f)
	, m_fCurrentValue(0.0f)
	, m_pObject(NULL)
{
	CreateLuaObject();
}

Slider::~Slider()
{
	if(m_pObject)
	{
		m_pObject->removeAllEvents();
		m_pObject = NULL;
	}
}

void Slider::RegisterScriptFunctions()
{
	//Create new metatable for a generic script component
	LuaPlus::LuaObject oMetatable = LuaManager::GetSingleton().GetGlobalVars().CreateTable(SLIDER_WIDGET_TABLE.c_str());
	oMetatable.SetObject("__index", oMetatable);

	oMetatable.RegisterObjectDirect("GetCurrentValue",  (Slider*)0, &Slider::GetCurrentValue);
	//oMetatable.RegisterObjectDirect("OnThumbTrackStarted", (Slider*)0, &Slider::OnThumbTrackStarted);
	//oMetatable.RegisterObjectDirect("OnThumbTrackEnded", (Slider*)0, &Slider::OnThumbTrackEnded);
}

void Slider::CreateLuaObject()
{
	m_oLuaObject.AssignNewTable(LuaManager::GetSingleton().GetLuaState());

	LuaPlus::LuaObject metaTable = LuaManager::GetSingleton().GetGlobalVars().GetByName(SLIDER_WIDGET_TABLE.c_str());

	m_oLuaObject.SetLightUserData("__object", const_cast<Slider*>(this));
	m_oLuaObject.SetMetaTable(metaTable);
}

bool Slider::SetupFromXml( const tinyxml2::XMLElement* pNode )
{
	if(pNode)
	{
		const tinyxml2::XMLElement* pMaxValue = pNode->FirstChildElement("MaxValue");
		if (pMaxValue)
		{
			m_fMaxValue = pMaxValue->FloatAttribute("value");
		}

		const tinyxml2::XMLElement* pClickStep = pNode->FirstChildElement("ClickStep");
		if (pClickStep)
		{
			m_fClickStep = pClickStep->FloatAttribute("value");
		}

		const tinyxml2::XMLElement* pCurrentValue = pNode->FirstChildElement("CurrentValue");
		if (pCurrentValue)
		{
			m_fCurrentValue = pCurrentValue->FloatAttribute("value");
		}
	}
	return IGUIWidgets::SetupFromXml(pNode);
}

void Slider::SetupFromScript( LuaPlus::LuaObject i_oScript )
{
	if (i_oScript.IsTable())
	{
	}
	else
	{
		MGD_LOG::LOGManager::GetSingleton().WriteLog(MGD_LOG::MGD_ERROR, COMPONENTS_CONTEXT, "This script is not a table!");
	}
}

//void Slider::SetOnThumbTrackStartedScriptFunction( LuaPlus::LuaObject oScriptFunc )
//{
//	if(oScriptFunc.IsFunction())
//	{
//		m_oScriptOnThumbTrackStarted = oScriptFunc;
//	}
//	else
//	{
//		MGD_LOG::LOGManager::GetSingleton().WriteLog(MGD_LOG::MGD_ERROR, GUI_CONTEXT, "OnThumbTrackStarted script is not a function!");
//	}
//}
//
//void Slider::SetOnThumbTrackEndedScriptFunction( LuaPlus::LuaObject oScriptFunc )
//{
//	if(oScriptFunc.IsFunction())
//	{
//		m_oScriptOnThumbTrackEnded = oScriptFunc;
//	}
//	else
//	{
//		MGD_LOG::LOGManager::GetSingleton().WriteLog(MGD_LOG::MGD_ERROR, GUI_CONTEXT, "OnThumbTrackEnded script is not a function!");
//	}
//}
//
//void Slider::SetOnValueChangedScriptFunction( LuaPlus::LuaObject oScriptFunc )
//{
//	if(oScriptFunc.IsFunction())
//	{
//		m_oScriptOnValueChangedFunction = oScriptFunc;
//	}
//	else
//	{
//		MGD_LOG::LOGManager::GetSingleton().WriteLog(MGD_LOG::MGD_ERROR, GUI_CONTEXT, "OnValueChanged script is not a function!");
//	}
//}

bool Slider::OnThumbTrackStarted(const CEGUI::EventArgs& i_oParam)
{
	//if(!m_oScriptOnThumbTrackStarted.IsNil())
	if (m_oLuaObject.GetByName("OnThumbTrackStarted").IsFunction())
	{
		LuaPlus::LuaFunction<void> oOnThumbTrackStartedFunction(m_oLuaObject.GetByName("OnThumbTrackStarted"));
		oOnThumbTrackStartedFunction();
		return true;
	}

	return false;
}

bool Slider::OnThumbTrackEnded(const CEGUI::EventArgs& i_oParam)
{
	//if(!m_oScriptOnThumbTrackEnded.IsNil())
	//{
	//	LuaPlus::LuaFunction<void> oOnThumbTrackEndedFunction(m_oScriptOnThumbTrackEnded);
	//	oOnThumbTrackEndedFunction();
	//	return true;
	//}
	if (m_oLuaObject.GetByName("OnThumbTrackEnded").IsFunction())
	{
		LuaPlus::LuaFunction<void> oOnThumbTrackEndedFunction(m_oLuaObject.GetByName("OnThumbTrackEnded"));
		oOnThumbTrackEndedFunction();
		return true;
	}

	return false;
}

bool Slider::OnValueChanged(const CEGUI::EventArgs& i_oParam)
{
	const CEGUI::WindowEventArgs& oParam = static_cast<const CEGUI::WindowEventArgs&>(i_oParam);
	CEGUI::Slider* pSlider = static_cast<CEGUI::Slider*>(oParam.window);
	if(pSlider)
	{
		m_fCurrentValue = pSlider->getCurrentValue();
	}

	if (m_oLuaObject.GetByName("OnValueChanged").IsFunction())
	{
		LuaPlus::LuaFunction<void> oOnValueChangedFunction(m_oLuaObject.GetByName("OnValueChanged"));

		oOnValueChangedFunction(LuaManager::GetSingleton().GetLuaState(),static_cast<int32>(m_fCurrentValue));
		return true;
	}

	return false;
}

CEGUI::Slider* Slider::Create( IGUIWidgets* i_oWidget, CEGUI::Window* i_pObject )
{
	Slider* pSlider = static_cast<Slider*>(i_oWidget);
	MGD_ASSERT(pSlider);
	if(pSlider)
	{
		pSlider->m_pObject = static_cast<CEGUI::Slider*>(i_pObject);
		MGD_ASSERT(pSlider->m_pObject);
		if(pSlider->m_pObject)
		{
			pSlider->m_pObject->setText(pSlider->GetText());
			pSlider->m_pObject->setSize(CEGUI::UVector2(CEGUI::UDim(pSlider->GetSize().x, 0), CEGUI::UDim(pSlider->GetSize().y, 0)));
			pSlider->m_pObject->setPosition(CEGUI::UVector2(CEGUI::UDim(pSlider->GetPosition().x, 0), CEGUI::UDim(pSlider->GetPosition().y, 0)));
			pSlider->m_pObject->setMaxValue(pSlider->GetMaxValue());
			pSlider->m_pObject->setClickStep(pSlider->GetClickStep());
			pSlider->m_pObject->setCurrentValue(pSlider->GetCurrentValue());

			pSlider->m_pObject->subscribeEvent(CEGUI::Slider::EventThumbTrackStarted, CEGUI::Event::Subscriber(&Slider::OnThumbTrackStarted, pSlider));
			pSlider->m_pObject->subscribeEvent(CEGUI::Slider::EventThumbTrackEnded, CEGUI::Event::Subscriber(&Slider::OnThumbTrackEnded, pSlider));
			pSlider->m_pObject->subscribeEvent(CEGUI::Slider::EventValueChanged, CEGUI::Event::Subscriber(&Slider::OnValueChanged, pSlider));
		}

		return pSlider->m_pObject;
	}

	return NULL;
}
