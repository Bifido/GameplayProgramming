#include "StaticText.h"
#include "..\..\LOG\LOGManager.h"
#include "Interface\IGUIWidget.h"
#include "..\..\Manager\LuaManager.h"

StaticText::StaticText(GUIViewComponent* i_pParent)
	: IGUIWidgets(i_pParent)
	, m_szTextColors("tl:00FFFFFF tr:00FFFFFF bl:00FFFFFF br:00FFFFFF")
	, m_szHorizFormatting("HorzCentred")
	, m_szVertFormatting("VertCentred")
	, m_pObject(NULL)
{
	CreateLuaObject();
}

StaticText::~StaticText()
{
	if(m_pObject)
	{
		m_pObject->removeAllEvents();
		m_pObject = NULL;
	}
}


void StaticText::CreateLuaObject()
{
	m_oLuaObject.AssignNewTable(LuaManager::GetSingleton().GetLuaState());

	LuaPlus::LuaObject metaTable =	LuaManager::GetSingleton().GetGlobalVars().GetByName("StaticTextMetatable");

	m_oLuaObject.SetLightUserData("__object", const_cast<StaticText*>(this));
	m_oLuaObject.SetMetaTable(metaTable);
}

void StaticText::RegisterScriptFunctions()
{
	//Create new metatable for a generic script component
	LuaPlus::LuaObject oStaticTextMetatable = LuaManager::GetSingleton().GetGlobalVars().CreateTable("StaticTextMetatable");

	oStaticTextMetatable.SetObject("__index", oStaticTextMetatable); // it’s also its own metatable
	oStaticTextMetatable.RegisterObjectDirect("SetText",  (StaticText*)0, &StaticText::SetTextFromScript);
	oStaticTextMetatable.RegisterObjectDirect("SetInt",  (StaticText*)0, &StaticText::SetInt);
}

bool StaticText::SetupFromXml( const tinyxml2::XMLElement* pNode )
{
	if(pNode)
	{
		const tinyxml2::XMLElement* pTextColours = pNode->FirstChildElement("TextColours");
		if (pTextColours)
		{
			char r[2] = {'0','0'};
			char g[2] = {'0','0'};
			char b[2] = {'0','0'};
			char a[2] = {'0','0'};

			ConvertDecToHex(static_cast<uint32>(pTextColours->IntAttribute("r")), r);
			ConvertDecToHex(static_cast<uint32>(pTextColours->IntAttribute("g")), g);
			ConvertDecToHex(static_cast<uint32>(pTextColours->IntAttribute("b")), b);
			ConvertDecToHex(static_cast<uint32>(pTextColours->IntAttribute("a")), a);

			std::string color(a,2);
			color.append(r,2);
			color.append(g,2);
			color.append(b,2);

			char szTextColors[64];
			sprintf_s(szTextColors, "tl:%s tr:%s bl:%s br:%s", color.c_str(), color.c_str(), color.c_str(), color.c_str());
			m_szTextColors = szTextColors;
		}

		const tinyxml2::XMLElement* pVertFormatting = pNode->FirstChildElement("VertFormatting");
		if (pVertFormatting)
		{
			m_szVertFormatting = pVertFormatting->Attribute("value");
		}

		const tinyxml2::XMLElement* pHorizFormatting = pNode->FirstChildElement("HorzFormatting");
		if (pHorizFormatting)
		{
			m_szHorizFormatting = pHorizFormatting->Attribute("value");
		}
	}
	return IGUIWidgets::SetupFromXml(pNode);
}

void StaticText::SetupFromScript( LuaPlus::LuaObject /*i_oScript*/ )
{
}

CEGUI::DefaultWindow* StaticText::Create( IGUIWidgets* i_oWidget, CEGUI::Window* i_pObject )
{
	StaticText* pStaticText = static_cast<StaticText*>(i_oWidget);
	if(pStaticText)
	{
		pStaticText->m_pObject = static_cast<CEGUI::DefaultWindow*>(i_pObject);
		MGD_ASSERT(pStaticText->m_pObject);
		if(pStaticText->m_pObject)
		{
			pStaticText->m_pObject->setText(pStaticText->GetText());
			pStaticText->m_pObject->setSize(CEGUI::UVector2(CEGUI::UDim(pStaticText->GetSize().x, 0), CEGUI::UDim(pStaticText->GetSize().y, 0)));
			pStaticText->m_pObject->setPosition(CEGUI::UVector2(CEGUI::UDim(pStaticText->GetPosition().x, 0), CEGUI::UDim(pStaticText->GetPosition().y, 0)));
			pStaticText->m_pObject->setProperty("TextColours", pStaticText->GetTextColors());
			pStaticText->m_pObject->setProperty("VertFormatting", pStaticText->GetVertFormatting());
			pStaticText->m_pObject->setProperty("HorzFormatting", pStaticText->GetHorizFormatting());
		}

		return pStaticText->m_pObject;
	}
	return NULL;
}

void StaticText::SetTextFromScript(const char* i_szText)
{
	m_szText = i_szText;
}

void StaticText::SetInt(int i_iVal)
{
	char szText[5];
	sprintf_s(szText,"%d", i_iVal);
	m_szText = szText;
	m_pObject->setText(m_szText);
}