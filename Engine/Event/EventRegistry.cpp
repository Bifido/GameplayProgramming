#include "EventRegistry.h"
#include "../Manager/LuaManager.h"
#include "../Common/MGDAssert.h"
#include "RegistryEventPublisher.h"
#include "RegistryEventHandler.h"

void EventRegistry::CreateInstance()
{
	if(GetSingletonPtr() == NULL)
		new EventRegistry();
}

EventRegistry::EventRegistry()
{
	RegisterGlobalLuaFunction();
}

EventRegistry::~EventRegistry()
{
	EventsMapIt it = m_LuaRegistry.begin();
	for (; it != m_LuaRegistry.end(); ++it)
	{
		MGD_ASSERT(0 && "TODO: LOG -> Unsubscribe lua event before shutdown the game!");
		delete((*it).second);
	}

	m_LuaRegistry.clear();
}

void EventRegistry::RegisterGlobalLuaFunction()
{
	LuaPlus::LuaObject oLuaObject = LuaManager::GetSingleton().GetGlobalVars();

	oLuaObject.RegisterDirect("SendEvent", &EventRegistry::LuaRaiseEvent);
	oLuaObject.RegisterDirect("SendLuaEvent", &EventRegistry::LuaRaiseLuaEvent);
	oLuaObject.RegisterDirect("SubscribeEvent", &EventRegistry::LuaSubscribeEvent);
	oLuaObject.RegisterDirect("UnsubscribeEvent", &EventRegistry::LuaUnsubscribeEvent);
}

void EventRegistry::LuaRaiseEvent(const char* i_szEventPath, const char* i_oParam)
{	
	if (i_oParam && strlen(i_oParam) > 0)
	{
		RegistryEventPublisher<const char*> oEventPublisher;
		oEventPublisher.SetPath(ObjectId(i_szEventPath));
		oEventPublisher.Raise(i_oParam);
	}
	else
	{
		RegistryEventPublisher<void> oEventPublisher;
		oEventPublisher.SetPath(ObjectId(i_szEventPath));
		oEventPublisher.Raise();
	}
}

void EventRegistry::LuaRaiseLuaEvent(const char* i_szEventPath, LuaPlus::LuaObject i_oLuaObject)
{
	if (!i_oLuaObject.IsNil())
	{
		LuaRegistryEventPublisher oLuaEventPublisher;
		oLuaEventPublisher.SetPath(ObjectId(i_szEventPath));
		oLuaEventPublisher.Raise(i_oLuaObject);
	}
}


void EventRegistry::LuaSubscribeEvent(const char* i_szId, const char* i_szEventPath, LuaPlus::LuaObject i_oLuaObject)
{
	LuaRegistryEventHandler* pLuaHandler = new LuaRegistryEventHandler();
	pLuaHandler->Subscribe(i_oLuaObject, ObjectId(i_szEventPath));

	EventsMapIt it = EventRegistry::GetSingleton().m_LuaRegistry.find(ObjectId(i_szId));
	if (it != EventRegistry::GetSingleton().m_LuaRegistry.end())
	{
		MGD_ASSERT(0 && "TODO: LOG -> This ID already exist!");
	}
	else
	{
		EventRegistry::GetSingleton().m_LuaRegistry[ObjectId(i_szId)] = pLuaHandler;
	}
}

void EventRegistry::LuaUnsubscribeEvent(const char* i_szId)
{
	EventsMapIt it = EventRegistry::GetSingleton().m_LuaRegistry.find(ObjectId(i_szId));
	if (it != EventRegistry::GetSingleton().m_LuaRegistry.end())
	{
		delete((*it).second);
		EventRegistry::GetSingleton().m_LuaRegistry.erase(it);
	}
	else
	{
		MGD_ASSERT(0 && "TODO: LOG -> This ID doesn't exist!");
	}
}

/*
EVENT HANDLER
*/

EventRegistry::EventHandler::EventHandler()
	: m_pNextHandler(NULL)
{
}

EventRegistry::EventHandler::~EventHandler()
{
	UnsubscribeEventHandler();		
}

void EventRegistry::EventHandler::SetNextHandler( EventHandler* i_pNextHandler )
{
	m_pNextHandler = i_pNextHandler;
}

EventRegistry::EventHandler* EventRegistry::EventHandler::GetNextHandler() const
{
	return m_pNextHandler;
}

void EventRegistry::EventHandler::SubscribeEventHandler(const ObjectId& i_oEventPath)
{
	EventsMap& vRegistry(EventRegistry::GetSingleton().EditRegistry());

	EventsMapIt it = vRegistry.find(i_oEventPath);

	EventRegistry::EventHandler* pEventHandler = vRegistry[i_oEventPath];
	if (pEventHandler)
	{
		while(pEventHandler->GetNextHandler())
		{
			pEventHandler = pEventHandler->GetNextHandler();
		}

		pEventHandler->SetNextHandler(this);
	}
	else
	{
		vRegistry[i_oEventPath] = this;
	}

	m_oEventPath = i_oEventPath;


}

void EventRegistry::EventHandler::UnsubscribeEventHandler()
{
	EventsMapIt it = EventRegistry::GetSingleton().m_Registry.find(GetPath());
	if (it != EventRegistry::GetSingleton().m_Registry.end())
	{
		EventRegistry::EventHandler* pEventHandler = (*it).second;
		MGD_ASSERT(pEventHandler);
		if (pEventHandler)
		{
			if(pEventHandler == this)
			{
				if(this->GetNextHandler())
				{
					pEventHandler = this->GetNextHandler();
				}
				else
				{
					pEventHandler = NULL;
				}
			}
			else
			{
				while(pEventHandler->GetNextHandler() && pEventHandler->GetNextHandler() != this)
				{
					pEventHandler = pEventHandler->GetNextHandler();
				}

				pEventHandler->SetNextHandler(this->GetNextHandler());
			}
		}
	
		if (!pEventHandler)
		{
			EventRegistry::GetSingleton().m_Registry.erase(it);
		}
	}
}