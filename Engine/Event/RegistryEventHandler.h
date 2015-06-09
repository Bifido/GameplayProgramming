/*
*	This is the class that is used to register a function to be notified when the event occurs
*/


#pragma once

#include "EventRegistry.h"

template < typename Class, typename ParamType = void >
class RegistryEventHandler;

//Event handler with param
template < typename Class, typename ParamType>
class RegistryEventHandler : public EventRegistry::EventHandler
{
public:
	typedef void (Class::*CallbackFunc)(const ParamType&);

	RegistryEventHandler()
		: EventRegistry::EventHandler()
		,	m_pTargetObject(NULL)
		,	m_pMethod(NULL)
	{
	}

	~RegistryEventHandler()
	{
		Unsubscribe();
	}

	void Subscribe( Class* i_pTargetObject, CallbackFunc i_pMethod, const ObjectId i_oEventPath )
	{
		m_pTargetObject = i_pTargetObject;
		m_pMethod = i_pMethod;

		SubscribeEventHandler(i_oEventPath);
	}

	void Unsubscribe()
	{
		UnsubscribeEventHandler();
	}

	void Invoke( const ObjectId& pParamType, void* i_pParam )
	{
		if (m_pTargetObject)
		{
			const ParamType& oParam = *static_cast<const ParamType*>(i_pParam);

			(m_pTargetObject->*m_pMethod)(oParam);
		}
	}

private: // data members	
	Class* m_pTargetObject;
	CallbackFunc m_pMethod;
};

//Event handler without param
template < typename Class >
class RegistryEventHandler<Class,void> : public EventRegistry::EventHandler
{
public:
	typedef void (Class::*CallbackFunc)();

	RegistryEventHandler()
		: EventRegistry::EventHandler()
		, m_pTargetObject(NULL)
		, m_pMethod(NULL)
	{
		}

	~RegistryEventHandler()
	{
		Unsubscribe();
	}

	void Subscribe(Class* i_pTargetObject, CallbackFunc i_pMethod, const ObjectId i_oEventPath)
	{
		m_pTargetObject = i_pTargetObject;
		m_pMethod = i_pMethod;

		SubscribeEventHandler(i_oEventPath);
	}

	void Unsubscribe()
	{
		UnsubscribeEventHandler();
	}

	void Invoke( const ObjectId& pParamType, void* i_pParam )
	{
		if (m_pTargetObject)
		{
			(m_pTargetObject->*m_pMethod)();
		}
	}

private: // data members	
	Class* m_pTargetObject;
	CallbackFunc m_pMethod;
};

//Event handler with param const char*
template < typename Class >
class RegistryEventHandler<Class,const char*> : public EventRegistry::EventHandler
{
public:
	typedef void (Class::*CallbackFunc)(const char*);

	RegistryEventHandler()
		: EventRegistry::EventHandler()
		, m_pTargetObject(NULL)
		, m_pMethod(NULL)
	{
	}

	~RegistryEventHandler()
	{
		Unsubscribe();
	}

	void Subscribe(Class* i_pTargetObject, CallbackFunc i_pMethod, const ObjectId i_oEventPath)
	{
		m_pTargetObject = i_pTargetObject;
		m_pMethod = i_pMethod;

		SubscribeEventHandler(i_oEventPath);
	}

	void Unsubscribe()
	{
		UnsubscribeEventHandler();
	}

	void Invoke( const ObjectId& pParamType, void* i_pParam )
	{
		if (m_pTargetObject)
		{
			const char* pParam = static_cast<const char*>(i_pParam);
			(m_pTargetObject->*m_pMethod)(pParam);
		}
	}

private: // data members	
	Class* m_pTargetObject;
	CallbackFunc m_pMethod;
};


//Event handler
class LuaRegistryEventHandler : public EventRegistry::EventHandler
{
public:	

	LuaRegistryEventHandler()
		: EventRegistry::EventHandler()		
	{
	}

	~LuaRegistryEventHandler()
	{
		Unsubscribe();
	}

	void Subscribe(LuaPlus::LuaObject i_oLuaMethod, const ObjectId i_oEventPath)
	{
		m_oScriptFunction = i_oLuaMethod;

		SubscribeEventHandler(i_oEventPath);
	}

	void Unsubscribe()
	{
		UnsubscribeEventHandler();
	}

	void Invoke(const ObjectId& pParamType, void* i_pParam)
	{
		if (m_oScriptFunction.IsFunction())
		{
			LuaPlus::LuaFunction<void> oFunction(m_oScriptFunction);
			if (i_pParam)
			{
				oFunction(*static_cast<LuaPlus::LuaObject*>(i_pParam));
			}
			else
			{
				oFunction();
			}
		}
	}

private:
	LuaPlus::LuaObject	m_oScriptFunction;	
};
