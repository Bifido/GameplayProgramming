#pragma once
#include "../Core/Component.h"
#include "Actor.h"

class SpriteComponent;

class SceneComponent : public Component
{
public:
	ID_DECLARATION;

	SceneComponent(std::string i_szOwnerID);
	virtual ~SceneComponent();

	virtual void Init();
	virtual bool SetupFromXml(const tinyxml2::XMLElement* pNode);

	/******* LUA *******/
	static void RegisterScriptFunction();
	/*******************/

	virtual void CreateFromTemplate(Component* i_pComponent, const ObjectId& i_oOwner)
	{
		if (i_pComponent)
		{			
			SceneComponent* i_pSceneComponent = static_cast<SceneComponent*>(i_pComponent);
			if (i_pSceneComponent)
			{
				Component::CreateFromTemplate(i_oOwner);
			}
		}
	}

	oxygine::Actor*	GetScene();

private:
	/******* LUA *******/
	virtual void CreateLuaObject();
	/*******************/
	std::string						m_szActor;
	oxygine::Actor					m_oScene;

	MGDVector<SpriteComponent*>		m_vSpriteComponents;

	static const char* METATABLE_NAME;
};

inline oxygine::Actor*	SceneComponent::GetScene()
{
	return &m_oScene;
}