#pragma once

#include "../Core/System.h"
#include "res/Resources.h"

class ResourceManagerSystem : public System
{
public:
	ID_DECLARATION;

	ResourceManagerSystem(bool i_bIs3D);
	virtual ~ResourceManagerSystem();

	virtual void Init();
	virtual void Update(real i_fFrametime, real i_fTimestep) {}

private:
	void LoadResourcesConfig();

	bool m_bIs3D;
	oxygine::Resources			m_oResources;

	std::string					m_sz3DResourcesFilename;
	std::string					m_sz2DResourcesFilename;

	static const char* RESOURCES_CFG;
	static ObjectId RESOURCE_MANAGER_SYSTEM_ID;
};

