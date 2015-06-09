/*
*	This is the manager for events.
*/
#pragma once
#include "../Common/DataTypes.h"
#include "../Core/Singleton.h"
#include "LuaPlus.h"

class EventRegistry : public Singleton<EventRegistry>
{
public:
	class EventHandler
	{
	public:
		EventHandler();
		virtual ~EventHandler();

		const ObjectId& GetPath() const
		{
			return m_oEventPath;
		}

		void SetNextHandler(EventHandler* i_pNextNode);
		EventHandler* GetNextHandler() const;

		void SubscribeEventHandler(const ObjectId& i_oEventPath);
		void UnsubscribeEventHandler();

		void AddHandler( EventHandler* i_pEventHandler );
		void RemoveHandler(EventHandler* i_pEventHandler);

		virtual void Invoke( const ObjectId& pParamType, void* i_pParam ) = 0;

	private:
		EventHandler*	m_pNextHandler;
		ObjectId		m_oEventPath;
	};

private:
	typedef MGDMap<ObjectId,EventHandler*>		EventsMap;
	typedef EventsMap::iterator					EventsMapIt;

public:
	~EventRegistry();

	static void CreateInstance();	

	const EventsMap& GetRegistry() const
	{
		return m_Registry;
	}

	EventsMap& EditRegistry()
	{
		return m_Registry;
	}

	static void LuaRaiseLuaEvent(const char* i_szEventPath, LuaPlus::LuaObject i_oLuaObject);
	static void LuaRaiseEvent(const char* i_szEventPath, const char* i_oParam );
	static void LuaSubscribeEvent(const char* i_szId, const char* i_szEventPath, LuaPlus::LuaObject i_oLuaObject);
	static void LuaUnsubscribeEvent(const char* i_szId );

private:
	EventRegistry();
	void RegisterGlobalLuaFunction();

	EventsMap m_Registry;
	EventsMap m_LuaRegistry;
};