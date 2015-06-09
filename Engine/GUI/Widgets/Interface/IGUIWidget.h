#pragma once
#include "../../../LOG/LOGManager.h"
#include "tinyxml2.h"
#include "../../../Common/Utilities.h"
#include "LuaObject.h"
#include "CEGUIEventArgs.h"

class GUIViewComponent;

class IGUIWidgets
{
public:
	IGUIWidgets(GUIViewComponent* i_pParent);
	virtual ~IGUIWidgets();

	virtual bool SetupFromXml( const tinyxml2::XMLElement* pNode );

	virtual void SetupFromScript( LuaPlus::LuaObject /*i_oScript*/ ) = 0;

	void SetName(std::string);
	std::string GetName() const;
	void SetType(std::string);
	std::string GetType() const;
	void SetStyle(std::string);
	std::string GetStyle() const;
	void SetText(std::string);
	std::string GetText() const;
	void SetSize(const Vec2& i_oSize);
	const Vec2& GetSize() const;
	void SetPosition(const Vec2& i_oPosition);
	const Vec2& GetPosition() const;	
	
	LuaPlus::LuaObject GetLuaObject()
	{
		return m_oLuaObject;
	}

protected:
	GUIViewComponent* m_pParent;
	LuaPlus::LuaObject	m_oLuaObject;
	std::string m_szName;
	std::string	m_szType;
	std::string	m_szStyle;
	std::string m_szText;
	Vec2		m_Size;
	Vec2		m_Position;
};

inline void IGUIWidgets::SetName(const std::string i_szName)
{
	m_szName = i_szName;
}

inline std::string IGUIWidgets::GetName() const
{
	return m_szName;
}

inline void IGUIWidgets::SetText(std::string i_szText)
{
	m_szText = i_szText;
}

inline std::string IGUIWidgets::GetText() const
{
	return m_szText;
}

inline void IGUIWidgets::SetStyle(const std::string i_szStyle)
{
	m_szStyle = i_szStyle;
}

inline std::string IGUIWidgets::GetStyle() const
{
	return m_szStyle;
}

inline void IGUIWidgets::SetType(const std::string i_szType)
{
	m_szType = i_szType;
}

inline std::string IGUIWidgets::GetType() const
{
	return m_szType;
}

inline void IGUIWidgets::SetSize(const Vec2& i_oSize)
{
	m_Size = i_oSize;
}

inline const Vec2& IGUIWidgets::GetSize() const
{
	return m_Size;
}

inline void IGUIWidgets::SetPosition(const Vec2& i_oPosition)
{
	m_Position = i_oPosition;
}

inline const Vec2& IGUIWidgets::GetPosition() const
{
	return m_Position;
}