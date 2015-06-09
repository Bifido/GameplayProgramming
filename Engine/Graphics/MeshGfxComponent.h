/*
*	This class contains the properties for each 3D Model that we have in game
*/


#pragma once
#include "IGfxComponent.h"

class MeshGfxComponent : public IGfxComponent
{
public:
	ID_DECLARATION;

	MeshGfxComponent(std::string i_szOwnerID, bool i_bIsCreatedFromTemplate = false);
	virtual ~MeshGfxComponent();

	virtual void Init();
	virtual bool SetupFromXml(const tinyxml2::XMLElement* pNode);
	virtual void CreateFromTemplate(Component* i_pComponent, const ObjectId& i_oOwner)
	{
		if (i_pComponent)
		{
			MeshGfxComponent* pMeshGfxComponent = static_cast<MeshGfxComponent*>(i_pComponent);
			if (pMeshGfxComponent)
			{
				m_szMeshName = pMeshGfxComponent->GetMeshName();
				IGfxComponent::CreateFromTemplate(i_pComponent, i_oOwner);
			}
		}
	}

	void SetMeshName(const std::string& i_szMeshName);
	const std::string& GetMeshName() const
	{
		return m_szMeshName;
	}

	/******* LUA *******/
	static void RegisterScriptFunction();
	/*******************/

private:
	std::string	m_szMeshName;

	/******* LUA *******/
	void CreateLuaObject();
	/*******************/

	static const std::string NO_MESH;
	static const char* METATABLE_NAME;
};

