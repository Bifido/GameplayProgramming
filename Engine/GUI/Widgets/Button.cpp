#include "Button.h"
#include "..\..\LOG\LOGManager.h"
#include "..\..\Manager\LuaManager.h"
#include "elements\CEGUIPushButton.h"
#include "..\..\Common\MGDAssert.h"
#include "CEGUIEventSet.h"
#include "CEGUIEvent.h"
#include "CEGUIExceptions.h"

const std::string Button::BUTTON_WIDGET_TABLE("ButtonWidgetTable");

Button::Button(GUIViewComponent* i_pParent)
	: IGUIWidgets(i_pParent)
	, m_pObject(NULL)
{
	CreateLuaObject();
}

Button::~Button() 
{
	if(m_pObject)
	{
		m_pObject->removeAllEvents();
		m_pObject = NULL;
	}
}

bool Button::SetupFromXml( const tinyxml2::XMLElement* pNode )
{
	return IGUIWidgets::SetupFromXml(pNode);
}

void Button::SetupFromScript( LuaPlus::LuaObject i_oScript )
{	
	if (i_oScript.IsTable())
	{

	}
	else
	{
		MGD_LOG::LOGManager::GetSingleton().WriteLog(MGD_LOG::MGD_ERROR, COMPONENTS_CONTEXT, "This script is not a table!");
	}
}

bool Button::OnClick(const CEGUI::EventArgs& /*i_oParam*/)
{
	if(m_oLuaObject.GetByName("OnClick").IsFunction())
	{
		LuaPlus::LuaFunction<void> oOnClickFunction(m_oLuaObject.GetByName("OnClick"));
		oOnClickFunction(m_oLuaObject);

		return true;
	}

	return false;
}

void Button::RegisterScriptFunctions()
{
	//Create new metatable for a generic script component
	LuaPlus::LuaObject oMetatable = LuaManager::GetSingleton().GetGlobalVars().CreateTable(BUTTON_WIDGET_TABLE.c_str());

	oMetatable.SetObject("__index", oMetatable);
}

void Button::CreateLuaObject()
{
	m_oLuaObject.AssignNewTable(LuaManager::GetSingleton().GetLuaState());

	LuaPlus::LuaObject metaTable =	LuaManager::GetSingleton().GetGlobalVars().GetByName(BUTTON_WIDGET_TABLE.c_str());

	m_oLuaObject.SetLightUserData("__object", const_cast<Button*>(this));
	m_oLuaObject.SetMetaTable(metaTable);
}

CEGUI::PushButton* Button::Create( IGUIWidgets* i_oWidget, CEGUI::Window* i_pObject )
{
	try
	{
		Button* pStaticText = static_cast<Button*>(i_oWidget);
		if(pStaticText)
		{
			pStaticText->m_pObject = static_cast<CEGUI::PushButton*>(i_pObject);
			MGD_ASSERT(pStaticText->m_pObject );
			if(pStaticText->m_pObject )
			{
				pStaticText->m_pObject->setText(pStaticText->GetText());
				pStaticText->m_pObject->setSize(CEGUI::UVector2(CEGUI::UDim(pStaticText->GetSize().x, 0), CEGUI::UDim(pStaticText->GetSize().y, 0)));
				pStaticText->m_pObject->setPosition(CEGUI::UVector2(CEGUI::UDim(pStaticText->GetPosition().x, 0), CEGUI::UDim(pStaticText->GetPosition().y, 0)));

				CEGUI::Event::Subscriber o(&Button::OnClick, pStaticText);
				MGD_ASSERT(pStaticText && pStaticText->m_pObject);
				pStaticText->m_pObject->subscribeEvent(CEGUI::PushButton::EventClicked, o);
			}

			return pStaticText->m_pObject;
		}
	}
	catch (CEGUI::Exception& e)
	{
		MGD_LOG::LOGManager::GetSingleton().WriteLog(MGD_LOG::MGD_ERROR, GUI_CONTEXT, e.getMessage().c_str());
	}
	
	MGD_ASSERT(0);
	return NULL;
}