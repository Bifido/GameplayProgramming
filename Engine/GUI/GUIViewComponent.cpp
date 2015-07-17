#include "GUIViewComponent.h"
#include "../Manager/LuaManager.h"
#include "../LOG/LOGManager.h"
#include "Widgets/Interface/IGUIWidget.h"
#include "Widgets/Button.h"
#include "Widgets/StaticText.h"

ID_IMPLEMENTATION(GUIViewComponent);

const std::string GUIViewComponent::GUI_COMPONENT_TABLE("GuiComponentMetatable");

GUIViewComponent::GUIViewComponent(std::string i_szOwnerID, bool i_bIsCreatedFromTemplate)
	: Component(i_szOwnerID, i_bIsCreatedFromTemplate)
	, m_bInit(false)
	,m_szScript("")
{
		CreateLuaObject();
}

GUIViewComponent::~GUIViewComponent()
{
	for (uint32 uiIndex = 0; uiIndex < m_GUIWidgets.size(); ++uiIndex)
	{
		delete(m_GUIWidgets[uiIndex]);
	}
	m_GUIWidgets.clear();
}


void GUIViewComponent::Init()
{
	if(!m_szScript.empty())
	{
		LuaManager::GetSingleton().ExecuteFile(m_szScript.c_str());
	}
}

void GUIViewComponent::CreateLuaObject()
{
	m_oLuaObject.AssignNewTable(LuaManager::GetSingleton().GetLuaState());

	LuaPlus::LuaObject metaTable =	LuaManager::GetSingleton().GetGlobalVars().GetByName(GUI_COMPONENT_TABLE.c_str());

	m_oLuaObject.SetLightUserData("__object", const_cast<GUIViewComponent*>(this));
	m_oLuaObject.SetMetaTable(metaTable);
}


void GUIViewComponent::RegisterScriptFunctions()
{
	//Create new metatable for a generic script component
	LuaPlus::LuaObject oGuiComponentMetatable = LuaManager::GetSingleton().GetGlobalVars().CreateTable(GUI_COMPONENT_TABLE.c_str());

	oGuiComponentMetatable.SetObject("__index", oGuiComponentMetatable); // it’s also its own metatable
	//oGuiComponentMetatable.RegisterObjectDirect("Setup",  (GUIViewComponent*)0, &GUIViewComponent::SetupFromScript);
	oGuiComponentMetatable.RegisterObjectDirect("GetWidget",  (GUIViewComponent*)0, &GUIViewComponent::GetLuaWidget);

	Button::RegisterScriptFunctions();
	StaticText::RegisterScriptFunctions();
	CheckBox::RegisterScriptFunctions();
}

LuaPlus::LuaObject GUIViewComponent::GetLuaWidget(const char* i_szName)
{
	//TODO -> Studente MGD
	MGDVector<IGUIWidgets*>::const_iterator widgetIter = m_GUIWidgets.begin();

	// Ogni GUIWidget ha un Lua object
	for (; widgetIter != m_GUIWidgets.end(); ++widgetIter){
		if ((*widgetIter)->GetName().compare(i_szName) == 0)
		{
			return (*widgetIter)->GetLuaObject();
		}
	}

	MGD_ASSERT(0);
	return LuaPlus::LuaObject();
}

void GUIViewComponent::SetupFromScript( LuaPlus::LuaObject i_oScript )
{
	if (i_oScript.IsTable())
	{

	}
	else
	{
		MGD_LOG::LOGManager::GetSingleton().WriteLog(MGD_LOG::MGD_ERROR, COMPONENTS_CONTEXT, "This script is not a table!");
	}

}

bool GUIViewComponent::SetupFromXml( const tinyxml2::XMLElement* pNode )
{
	if(pNode)
	{
		const tinyxml2::XMLElement* pScript = pNode->FirstChildElement("Script");
		if (pScript)
		{
			m_szScript = pScript->Attribute("filename");				
		}

		for (const tinyxml2::XMLElement* pActor = pNode->FirstChildElement("GUIComponent"); pActor; pActor = pActor->NextSiblingElement("GUIComponent"))
		{
			const tinyxml2::XMLElement* pType = pActor->FirstChildElement("Type");
			if (pType)
			{
				IGUIWidgets*	pGUIWidget = NULL;
				const char* szType = pType->Attribute("value");
				if(strcmp(szType,"Button") == 0)
				{
					pGUIWidget = new Button(this);
					pGUIWidget->SetType(szType);
				}
				else if(strcmp(szType,"StaticText") == 0)
				{
					pGUIWidget = new StaticText(this);
					pGUIWidget->SetType(szType);
				}
				else if (strcmp(szType, "CheckBox") == 0)
				{
					pGUIWidget = new CheckBox(this);
					pGUIWidget->SetType(szType);
				}
				else
				{
					MGD_LOG::LOGManager::GetSingleton().WriteLog(MGD_LOG::MGD_ERROR, COMPONENTS_CONTEXT, "This type of widget doesn't exist!");
					return false;
				}

				pGUIWidget->SetupFromXml(pActor);

				m_GUIWidgets.push_back(pGUIWidget);
			}
			else
			{
				MGD_LOG::LOGManager::GetSingleton().WriteLog(MGD_LOG::MGD_ERROR, COMPONENTS_CONTEXT, "Missing type of widget!");
			}
		}
	}

	return Component::SetupFromXml(pNode);
}