#include "CheckBox.h"
#include "..\..\LOG\LOGManager.h"
#include "..\..\Manager\LuaManager.h"
#include "..\..\Script\LuaUtilities.h"
#include "elements\CEGUICheckbox.h"
#include "..\..\Common\MGDAssert.h"
#include "CEGUIEventSet.h"
#include "CEGUIEvent.h"
#include "CEGUIExceptions.h"

const std::string CheckBox::CHECKBOX_WIDGET_TABLE("CheckboxWidgetTable");

CheckBox::CheckBox(GUIViewComponent* i_pParent)
	: IGUIWidgets(i_pParent)
	, m_bSelected(FALSE)
	, m_pObject(NULL)
{
	CreateLuaObject();
}

CheckBox::~CheckBox()
{
	if (m_pObject)
	{
		m_pObject->removeAllEvents();
		m_pObject = NULL;
	}
}

bool CheckBox::SetupFromXml(const tinyxml2::XMLElement* pNode)
{
	return IGUIWidgets::SetupFromXml(pNode);
}

void CheckBox::SetupFromScript(LuaPlus::LuaObject i_oScript)
{
	if (i_oScript.IsTable())
	{

	}
	else
	{
		MGD_LOG::LOGManager::GetSingleton().WriteLog(MGD_LOG::MGD_ERROR, COMPONENTS_CONTEXT, "This script is not a table!");
	}
}

void CheckBox::SetOnSelectScriptFunction(LuaPlus::LuaObject oScriptFunc)
{

}

bool CheckBox::OnCheckStateChanged(const CEGUI::EventArgs& i_oParam)
{
	const CEGUI::WindowEventArgs& oParam = static_cast<const CEGUI::WindowEventArgs&>(i_oParam);
	CEGUI::Checkbox* pCheckbox = static_cast<CEGUI::Checkbox*>(oParam.window);

	if (pCheckbox)
	{
		m_bSelected = pCheckbox->isSelected();
	}

	if (m_oLuaObject.GetByName("OnCheckStateChanged").IsFunction())
	{
		LuaPlus::LuaFunction<void> oOnSelectFunction(m_oLuaObject.GetByName("OnCheckStateChanged"));
		oOnSelectFunction(LuaManager::GetSingleton().GetLuaState(), static_cast<bool>(m_bSelected));
		return true;
	}

	return false;
}

void CheckBox::RegisterScriptFunctions()
{
	//Create new metatable for a generic script component
	LuaPlus::LuaObject oMetatable = LuaManager::GetSingleton().GetGlobalVars().CreateTable(CHECKBOX_WIDGET_TABLE.c_str());

	oMetatable.SetObject("__index", oMetatable);

	oMetatable.RegisterObjectDirect("SetText", (CheckBox*)0, &CheckBox::SetTextFromScript);
	oMetatable.RegisterObjectDirect("SetSelected", (CheckBox*)0, &CheckBox::SetSelectedFromScript);
	oMetatable.RegisterObjectDirect("IsSelected", (CheckBox*)0, &CheckBox::IsSelectedFromScript);
}

void CheckBox::CreateLuaObject()
{
	m_oLuaObject.AssignNewTable(LuaManager::GetSingleton().GetLuaState());

	LuaPlus::LuaObject metaTable = LuaManager::GetSingleton().GetGlobalVars().GetByName(CHECKBOX_WIDGET_TABLE.c_str());

	m_oLuaObject.SetLightUserData("__object", const_cast<CheckBox*>(this));
	m_oLuaObject.SetMetaTable(metaTable);
}

CEGUI::Checkbox* CheckBox::Create(IGUIWidgets* i_oWidget, CEGUI::Window* i_pObject)
{
	try
	{
		CheckBox* pCheckBox = static_cast<CheckBox*>(i_oWidget);
		if (pCheckBox)
		{
			pCheckBox->m_pObject = static_cast<CEGUI::Checkbox*>(i_pObject);
			MGD_ASSERT(pCheckBox->m_pObject);
			if (pCheckBox->m_pObject)
			{
				pCheckBox->m_pObject->setText(pCheckBox->GetText());
				pCheckBox->m_pObject->setSize(CEGUI::UVector2(CEGUI::UDim(pCheckBox->GetSize().x, 0), CEGUI::UDim(pCheckBox->GetSize().y, 0)));
				pCheckBox->m_pObject->setPosition(CEGUI::UVector2(CEGUI::UDim(pCheckBox->GetPosition().x, 0), CEGUI::UDim(pCheckBox->GetPosition().y, 0)));
				pCheckBox->m_pObject->setSelected(pCheckBox->m_bSelected);

				pCheckBox->m_pObject->subscribeEvent(CEGUI::Checkbox::EventCheckStateChanged, CEGUI::Event::Subscriber(&CheckBox::OnCheckStateChanged, pCheckBox));
			}

			return pCheckBox->m_pObject;
		}
	}
	catch (CEGUI::Exception& e)
	{
		MGD_LOG::LOGManager::GetSingleton().WriteLog(MGD_LOG::MGD_ERROR, GUI_CONTEXT, e.getMessage().c_str());
	}

	MGD_ASSERT(0);
	return NULL;
}

void CheckBox::SetTextFromScript(const char* i_szText)
{
	m_szText = i_szText;
	m_pObject->setText(m_szText);
}

void CheckBox::SetSelectedFromScript(bool i_bSelected)
{
	m_bSelected = i_bSelected;
	m_pObject->setSelected(m_bSelected);
}

bool CheckBox::IsSelectedFromScript()
{
	return m_bSelected;
}