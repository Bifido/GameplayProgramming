#include "IGUIWidget.h"
#include "..\..\GUIViewComponent.h"

IGUIWidgets::IGUIWidgets(GUIViewComponent* i_pParent)
	: m_pParent(i_pParent)
	, m_szName("")
	, m_szType("")
	, m_Size(Vec2::UNIT_SCALE)
	, m_Position(0.0f,0.0f)
{
}

IGUIWidgets::~IGUIWidgets()
{
}


bool IGUIWidgets::SetupFromXml( const tinyxml2::XMLElement* pNode )
{
	if(pNode)
	{

		const tinyxml2::XMLElement* pName = pNode->FirstChildElement("Name");
		if (pName)
		{
			m_szName = pName->Attribute("value");
		}
		const tinyxml2::XMLElement* pStyle = pNode->FirstChildElement("Style");
		if (pStyle)
		{
			m_szStyle = pStyle->Attribute("value");
		}
		const tinyxml2::XMLElement* pText = pNode->FirstChildElement("Text");
		if (pText)
		{
			m_szText = pText->Attribute("value");
		}
		const tinyxml2::XMLElement* pSize = pNode->FirstChildElement("Size");
		if (pSize)
		{
			real width, heigth;

			width = pSize->FloatAttribute("width");
			heigth = pSize->FloatAttribute("heigth");

			m_Size.x = width;
			m_Size.y = heigth;
		}
		const tinyxml2::XMLElement* pPosition = pNode->FirstChildElement("Position");
		if (pPosition)
		{
			real x, y;

			x = pPosition->FloatAttribute("x");
			y = pPosition->FloatAttribute("y");

			m_Position.x = x;
			m_Position.y = y;
		}
	}

	return true;
}