/*
*	This is the class that is used to register a function to be notified when the event occurs
*/


#pragma once

#include "EventRegistry.h"

template < typename T >
class RegistryEventPublisher
{
public:
	RegistryEventPublisher()
	{
	}
	
	virtual ~RegistryEventPublisher()
	{
	}

	void SetPath(const ObjectId& i_oEventPath)
	{
		m_oEventPath = i_oEventPath;
	}

	const ObjectId& GetPath() const
	{
		return m_oEventPath;
	}

	void Raise(const T& i_pParam)
	{
		MGDMap<ObjectId, EventRegistry::EventHandler*>::iterator it = EventRegistry::GetSingleton().EditRegistry().find(GetPath());
		if (it != EventRegistry::GetSingleton().GetRegistry().end())
		{
			EventRegistry::EventHandler* pNode = (*it).second;
			while (pNode)
			{
				pNode->Invoke(T::ID, static_cast<void*>(&const_cast<T&>(i_pParam)));
				pNode = pNode->GetNextHandler();
			}
		}
		else
		{
			MGD_ASSERT(0);
		}
	}

private:
	ObjectId	m_oEventPath;
};

template<>
class RegistryEventPublisher<void>
{
public:
	RegistryEventPublisher()		
	{
	}

	virtual ~RegistryEventPublisher()
	{
	}

	void SetPath(const ObjectId& i_oEventPath)
	{
		m_oEventPath = i_oEventPath;
	}

	const ObjectId& GetPath() const
	{
		return m_oEventPath;
	}


	void Raise()
	{
		MGDMap<ObjectId,EventRegistry::EventHandler*>::const_iterator it =  EventRegistry::GetSingleton().GetRegistry().find(GetPath());
		if(it != EventRegistry::GetSingleton().GetRegistry().end())
		{
			EventRegistry::EventHandler* pNode = (*it).second;
			while (pNode)
			{
				pNode->Invoke(INVALID_HASH, NULL);
				pNode = pNode->GetNextHandler();
			}
		}
	}

private:
	ObjectId	m_oEventPath;
};

template<>
class RegistryEventPublisher<const char*>
{
public:
	RegistryEventPublisher()		
	{
	}

	virtual ~RegistryEventPublisher()
	{
	}

	void SetPath(const ObjectId& i_oEventPath)
	{
		m_oEventPath = i_oEventPath;
	}

	const ObjectId& GetPath() const
	{
		return m_oEventPath;
	}

	void Raise(const char* i_pParam)
	{
		MGDMap<ObjectId,EventRegistry::EventHandler*>::const_iterator it =  EventRegistry::GetSingleton().GetRegistry().find(GetPath());
		if(it != EventRegistry::GetSingleton().GetRegistry().end())
		{
			EventRegistry::EventHandler* pNode = (*it).second;
			MGD_ASSERT(pNode);
			while (pNode)
			{
				pNode->Invoke(ObjectId("const char*"), static_cast<void*>(const_cast<char*>(i_pParam)));
				pNode = pNode->GetNextHandler();
			}
		}
	}

private:
	ObjectId	m_oEventPath;
};


class LuaRegistryEventPublisher
{
public:
	LuaRegistryEventPublisher()
	{
	}

	virtual ~LuaRegistryEventPublisher()
	{
	}

	void SetPath(const ObjectId& i_oEventPath)
	{
		m_oEventPath = i_oEventPath;
	}

	const ObjectId& GetPath() const
	{
		return m_oEventPath;
	}

	void Raise(LuaPlus::LuaObject i_pParam)
	{
		MGDMap<ObjectId, EventRegistry::EventHandler*>::iterator it = EventRegistry::GetSingleton().EditRegistry().find(GetPath());
		if (it != EventRegistry::GetSingleton().EditRegistry().end())
		{
			EventRegistry::EventHandler* pNode = (*it).second;
			while (pNode)
			{
				pNode->Invoke(ObjectId("LuaObject"), &i_pParam);
				pNode = pNode->GetNextHandler();
			}
		}
		else
		{
			MGD_ASSERT(0);
		}
	}

private:
	ObjectId	m_oEventPath;
};