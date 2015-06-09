#include "ResourceManager.h"
#include "OgreConfigFile.h"
#include "OgreResourceGroupManager.h"
#include "../Common/Utilities.h"
#include "../LOG/LogManager.h"
#include "tinyxml2.h"

const char* ResourceManager::RESOURCES_CFG("Data/Resources/Resources.cfg");
void ResourceManager::CreateInstance(bool i_bIs3)
{
	if(GetSingletonPtr() == NULL)
		new ResourceManager(i_bIs3);
}

ResourceManager::ResourceManager(bool i_bIs3D)
	: m_bIs3D(i_bIs3D)
{
	Init();
}


ResourceManager::~ResourceManager()
{
	MGD_LOG::LOGManager::GetSingleton().WriteLog(MGD_LOG::MGD_INFO, SYSTEMS_CONTEXT, "ResourceManagerSystem destroyed!");
}

void ResourceManager::Init()
{
	LoadResourcesConfig();
	if(m_bIs3D)
	{
		Ogre::ConfigFile oResourcesConfigFile;
		oResourcesConfigFile.load(m_sz3DResourcesFilename.c_str());

		// Go through all sections & settings in the file
		Ogre::ConfigFile::SectionIterator seci = oResourcesConfigFile.getSectionIterator();

		Ogre::String secName, typeName, archName;
		while (seci.hasMoreElements())
		{
			secName = seci.peekNextKey();
			Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
			Ogre::ConfigFile::SettingsMultiMap::iterator i;
			for (i = settings->begin(); i != settings->end(); ++i)
			{
				typeName = i->first;
				archName = i->second;
				Ogre::ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
			}
		}
	}
	else
	{
		if(!m_sz2DResourcesFilename.empty())
		{
			//m_oResources.loadXML(m_sz2DResourcesFilename.c_str());
		}
	}

	MGD_LOG::LOGManager::GetSingleton().WriteLog(MGD_LOG::MGD_INFO, SYSTEMS_CONTEXT, "ResourceManagerSystem initialized!");
}

void ResourceManager::LoadResourcesConfig()
{
	tinyxml2::XMLDocument oDocument;
	tinyxml2::XMLError oErr = oDocument.LoadFile(RESOURCES_CFG);
	if(oErr != tinyxml2::XML_NO_ERROR)
	{
		return;
	}
	const tinyxml2::XMLElement* pRoot = oDocument.RootElement();
	if(pRoot)
	{
		const tinyxml2::XMLElement* p3DResources = pRoot->FirstChildElement("Resources3D");
		if(p3DResources)
		{
			m_sz3DResourcesFilename = p3DResources->Attribute("filename");
		}

		const tinyxml2::XMLElement* p2DResources = pRoot->FirstChildElement("Resources2D");
		if(p2DResources)
		{
			m_sz2DResourcesFilename = p2DResources->Attribute("filename");
		}
	}
}