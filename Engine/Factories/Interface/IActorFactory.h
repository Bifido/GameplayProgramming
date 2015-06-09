#pragma once
#include "DataTypes.h"

class Actor;
class Component;

template <class BaseType, class SubType>
BaseType* GenericObjectCreationFunction() { return new SubType; }

template <class BaseClass, class IdType>
class GenericObjectFactory
{
	typedef BaseClass* (*ObjectCreationFunction)(void);
	MGDMap<IdType, ObjectCreationFunction> m_ConstructorCallback;

public:
	template <class SubClass>
	bool Register(IdType i_oId)
	{
		auto findIt = m_ConstructorCallback.find(i_oId);
		if (findIt == m_ConstructorCallback.end())
		{
			m_ConstructorCallback[i_oId] = &GenericObjectCreationFunction<BaseClass, SubClass>;
			return true;
		}

		return false;
	}

	BaseClass* Create(IdType i_oId)
	{
		MGDMap<IdType, ObjectCreationFunction>::iterator findIt = m_ConstructorCallback.find(i_oId);
		if (findIt != m_ConstructorCallback.end())
		{
			ObjectCreationFunction pFunc = findIt->second;
			return pFunc();
		}

		return NULL;
	}
};

class IActorFactory
{
public:
	~IActorFactory();

private:
	IActorFactory();

	typedef ObjectId ComponentID;
	GenericObjectFactory<Component,ComponentID> m_Factory;

	static void LoadFile(const char* i_szFilename);
	static void CreateActor(const tinyxml2::XMLElement* i_pActor);
	static void CreateTemplateActor(const tinyxml2::XMLElement* i_pTemplateActor);
	static Component* CreateComponent(const tinyxml2::XMLElement* i_pNode);

	static void CreateActorFromTemplate(const char* i_pTemplateActorName, const char* i_pActorName);

	void RegisterComponents();
	void RegisterGlobalLuaFunction();
};