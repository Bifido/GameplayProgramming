#pragma once

#include "../Core/Singleton.h"
#include "../Common/DataTypes.h"

class ResourceManager : public Singleton<ResourceManager>
{
public:
	static void CreateInstance(bool i_bIs3);
	virtual ~ResourceManager();

	void Init();

private:
	ResourceManager(bool i_bIs3D);

	void LoadResourcesConfig();
	bool m_bIs3D;
	
	std::string					m_sz3DResourcesFilename;
	std::string					m_sz2DResourcesFilename;

	static const char* RESOURCES_CFG;
	static ObjectId RESOURCE_MANAGER_SYSTEM_ID;
};

