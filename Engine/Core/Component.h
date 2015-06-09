/*
* This is the base class for components.
* A component is a specific behaviour that we add to an actor.
* This is managed by a specific system.
*/
#pragma once
#include "../Common/DataTypes.h"
#include "LuaPlus.h"
#include "../Common/MGDAssert.h"
#include "tinyxml2.h"

class Actor;
class Component
{
public:
	ID_DECLARATION;
	Component(std::string i_szOwnerID, bool i_bIsCreatedFromTemplate = false);
	virtual ~Component();

	virtual void Init() = 0;
	virtual bool SetupFromXml(const tinyxml2::XMLElement* pNode);

	void				SetInit();
	bool				IsInitialize() const;
	const ObjectId&		GetOwnerID() const;
	const ObjectId&		GetName() const;
	const std::string&	GetTextName() const;

	void				SetOwnerID(const ObjectId& i_oOwner);

	void				CreateName();

	void SetEnableFromScript(LuaPlus::LuaObject i_bEnable);
	LuaPlus::LuaObject IsEnableFromScript() const;

	bool IsCreatedFromTemplate() const
	{
		return m_bIsCreatedFromTemplate;
	}
	//LUA
	virtual LuaPlus::LuaObject GetLuaObject()
	{
		MGD_ASSERT(!m_oLuaObject.IsNil());
		return m_oLuaObject;
	}
	//

	void Remove();
	bool IsRemovable() const;

	virtual void CreateFromTemplate(Component* i_pComponent, const ObjectId& i_oOwner) = 0;
	void CreateFromTemplate( const ObjectId& i_oOwner );

	bool IsEnabled() const
	{
		return m_bIsEnable;
	}

	void SetEnable(bool i_bEnable)
	{
		m_bIsEnable = i_bEnable;
	}

protected:
	LuaPlus::LuaObject  m_oLuaObject;

private:
	virtual void CreateLuaObject();

	ObjectId	m_oOwnerId;
	ObjectId	m_oName;	
	std::string m_szTextName;	
	bool		m_bIsEnable;

	bool		m_bInit;
	bool		m_bIsRemovable;
	bool		m_bIsCreatedFromTemplate;

	static int8 DEFAULT_COMPONENT_COUNT;
};

inline void	Component::SetInit() 
{
	m_bInit = true;
}

inline bool Component::IsInitialize() const 
{
	return m_bInit;
}

inline const ObjectId& Component::GetOwnerID() const
{
	return m_oOwnerId;
}

inline void Component::SetOwnerID(const ObjectId& i_oOwner)
{
	m_oOwnerId = i_oOwner;
}

inline void Component::Remove()
{
	m_bIsRemovable = TRUE;
}

inline bool Component::IsRemovable() const
{
	return m_bIsRemovable;
}

inline const std::string& Component::GetTextName() const
{
	return m_szTextName;
}

inline const ObjectId& Component::GetName() const
{
	return m_oName;
}

inline void Component::CreateFromTemplate(const ObjectId& i_oOwner)
{
	CreateName();
	m_oOwnerId = i_oOwner;
}