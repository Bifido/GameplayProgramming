#pragma once
#include "../Core/Component.h"

class SpriteComponent : public Component
{
public:
	ID_DECLARATION;

	SpriteComponent(std::string i_szOwnerID, bool i_bIsCreatedFromTemplate = false/*, oxygine::Actor* i_pParent = NULL*/);
	virtual ~SpriteComponent();

	virtual void Init();
	virtual bool SetupFromXml(const tinyxml2::XMLElement* pNode);

	const std::string&	GetPath() const
	{
		return m_szPath;
	}

	real GetWitdh() const
	{
		return m_fWidth;
	}

	real GetHeight() const
	{
		return m_fHeight;
	}

	/******* LUA *******/
	static void RegisterScriptFunction();
	/*******************/

	virtual void CreateFromTemplate(Component* i_pComponent, const ObjectId& i_oOwner)
	{
		if (i_pComponent)
		{			
			SpriteComponent* i_pSpriteComponent = static_cast<SpriteComponent*>(i_pComponent);
			if (i_pSpriteComponent)
			{
				Component::CreateFromTemplate(i_oOwner);
			}
		}
	}

private:
	/******* LUA *******/
	virtual void CreateLuaObject();
	void AddTweenFromScript(const char* i_szType, const char* i_szRes, real i_fDuration, bool i_bLoop);
	/*******************/

	std::string m_szPath;
	real m_fWidth;
	real m_fHeight;

	static const char* METATABLE_NAME;
};

