/*
*	This class creates and destroies components and actors from XML files
*/
#pragma once
#include "../Core/Component.h"
#include "../Core/Actor.h"
#include "../Common/DataTypes.h"
#include "../Common/MGDAssert.h"
#include "../Event/RegistryEventPublisher.h"

template <class BaseType, class SubType>
BaseType* GenericObjectCreationFunction(const char* i_pParentId, bool i_bIsCreatedFromTemplate = false) { return new SubType(i_pParentId, i_bIsCreatedFromTemplate); }

template <class BaseClass, class IdType>
class GenericObjectFactory
{
	typedef BaseClass* (*ObjectCreationFunction)(const char* i_pParentId, bool i_bIsCreatedFromTemplate);
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

	BaseClass* Create(IdType i_oId, const char* i_pParentId, bool i_bIsCreatedFromTemplate = false)
	{
		MGDMap<IdType, ObjectCreationFunction>::iterator findIt = m_ConstructorCallback.find(i_oId);
		if (findIt != m_ConstructorCallback.end())
		{
			ObjectCreationFunction pFunc = findIt->second;
			return pFunc(i_pParentId, i_bIsCreatedFromTemplate);
		}

		return NULL;
	}
};

class ActorFactory : public Singleton<ActorFactory>
{
public:
	static void CreateInstance();

	~ActorFactory();

private:
	ActorFactory();

	typedef ObjectId ComponentID;
	GenericObjectFactory<Component,ComponentID> m_Factory;

	static void InitComponents(MGDVector<Component*>& i_vNewComponents);

	static void LoadFile(const char* i_szFilename);
	static void CreateActor(const tinyxml2::XMLElement* i_pActor, MGDVector<Component*>& o_vNewComponents);
	static void CreateTemplateActor(const tinyxml2::XMLElement* i_pTemplateActor);
	static Component* CreateComponent(const tinyxml2::XMLElement* i_pNode, const char* i_pActorName);
	static void CreateActorFromTemplate(const char* i_pTemplateActorName, const char* i_pActorName);

	static void UnloadFile(const char* i_szFilename);

	static void RemoveActor(const char* i_szActor);

	void RegisterComponents();
	void RegisterGlobalLuaFunction();

	RegistryEventPublisher<const char*> m_oEventPublisher;
};