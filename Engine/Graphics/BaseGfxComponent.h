/*
*	This component the property of base 3D models as CUBE, PLANE, BOX and SPHERE
*/

#pragma once
#include "IGfxComponent.h"

class BaseGfxComponent : public IGfxComponent
{
public:
	ID_DECLARATION;

	enum PrefabType
	{
		CUBE = 0,
		PLANE,
		SPHERE
	};

	BaseGfxComponent(std::string i_szOwnerID, bool i_bIsCreatedFromTemplate = false);
	explicit BaseGfxComponent( std::string i_szOwnerID, PrefabType i_oPrefabType );
	virtual ~BaseGfxComponent();

	virtual void Init();
	virtual bool SetupFromXml(const tinyxml2::XMLElement* pNode);

	PrefabType	GetPrefabType() const
	{
		return m_oPrefabType;
	}

	/******* LUA *******/
	static void RegisterScriptFunction();
	/*******************/

	virtual void CreateFromTemplate(Component* i_pComponent, const ObjectId& i_oOwner)
	{
		if (i_pComponent)
		{			
			BaseGfxComponent* i_pBaseGfxComponent = static_cast<BaseGfxComponent*>(i_pComponent);
			if (i_pBaseGfxComponent)
			{
				m_oPrefabType = i_pBaseGfxComponent->GetPrefabType();
				IGfxComponent::CreateFromTemplate(i_pComponent, i_oOwner);
			}
		}
	}

private:
	PrefabType	m_oPrefabType;

	/******* LUA *******/
	virtual void CreateLuaObject();
	/*******************/

	static const char* METATABLE_NAME;
};

