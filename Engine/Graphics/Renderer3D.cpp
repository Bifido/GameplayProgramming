#include "Renderer3D.h"
#include "../LOG/LOGManager.h"
#include "../Animation/Motion.h"
#include "tinyxml2.h"
#include "../Common/Common.h"

#ifdef DEBUG
	std::string Renderer3D::PLUGIN_CFG("Data/Plugins/Plugins_d.cfg");
#else
	std::string Renderer3D::PLUGIN_CFG("Data/Plugins/Plugins.cfg");
#endif

	IRenderer* Renderer3D::CreateInstance()
	{		
		return new Renderer3D();
	}

	Renderer3D::Renderer3D()
		: m_pRoot(new Ogre::Root(PLUGIN_CFG.c_str()))
		, m_pMainWindow(NULL)
	{
		if(m_pRoot->restoreConfig() || m_pRoot->showConfigDialog())
		{
			m_pMainWindow = m_pRoot->initialise(true, "Window");

			m_pViewPort = m_pMainWindow->addViewport(NULL);
			if(m_pViewPort)
			{
				m_pViewPort->setBackgroundColour(Ogre::ColourValue(0.0f,0.0f,0.0f));
			}

			//TEMP: We could use multiple sceneManager but now we have some architecture problem about the instantiation of the cameras and viewports
			ConfigureSceneManager();
		}
		else
		{
			MGD_LOG::LOGManager::GetSingleton().WriteLog(MGD_LOG::MGD_ERROR, RENDERING_CONTEXT, "OgreRendering creation failed!");
			return;
		}
	}

	Renderer3D::~Renderer3D()
	{

	}

	void Renderer3D::ConfigureSceneManager()
	{
		m_pSceneManager = m_pRoot->createSceneManager(Ogre::ST_GENERIC);
		MGD_ASSERT(m_pSceneManager);
		if(m_pSceneManager)
		{
			m_pSceneManager->setShadowUseInfiniteFarPlane(false);

			tinyxml2::XMLDocument oDocument;
			tinyxml2::XMLError oErr = oDocument.LoadFile(CONFIG.c_str());
			if(oErr != tinyxml2::XML_NO_ERROR)
			{
				MGD_LOG::LOGManager::GetSingleton().WriteLog(MGD_LOG::MGD_ERROR, RENDERING_CONTEXT, "Error loading file: %s", CONFIG.c_str());
				return;
			}
			const tinyxml2::XMLElement* pRoot = oDocument.RootElement();
			if(pRoot)
			{
				const tinyxml2::XMLElement* pAmbientColor = pRoot->FirstChildElement("AmbientColor");
				if(pAmbientColor)
				{
					real r = pAmbientColor->FloatAttribute("r");
					real g = pAmbientColor->FloatAttribute("g");
					real b = pAmbientColor->FloatAttribute("b");
					real a = pAmbientColor->FloatAttribute("a");

					m_pSceneManager->setAmbientLight(Ogre::ColourValue(r, g, b, a));
				}

				const tinyxml2::XMLElement* pShadowTechnique = pRoot->FirstChildElement("ShadowTechnique");
				if(pShadowTechnique)
				{
					const char* pTechnique = pShadowTechnique->Attribute("value");
					if(pTechnique)
					{
						if(strcmp(pTechnique,"Stencil-Additive") == 0)
						{
							m_pSceneManager->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);
						}
						else if(strcmp(pTechnique,"Texture-Additive") == 0)
						{
							m_pSceneManager->setShadowTechnique(Ogre::SHADOWTYPE_TEXTURE_ADDITIVE);
						}
						else if(strcmp(pTechnique,"Stencil-Modulative") == 0)
						{
							m_pSceneManager->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_MODULATIVE);
						}
						else if(strcmp(pTechnique,"Texture-Modulative") == 0)
						{
							m_pSceneManager->setShadowTechnique(Ogre::SHADOWTYPE_TEXTURE_MODULATIVE);
						}
						else
						{
							m_pSceneManager->setShadowTechnique(Ogre::SHADOWTYPE_NONE);
						}
					}
				}
			}
		}
	}

	//Ogre::SceneManager* OgreRendering::CreateScene(Ogre::SceneType i_oSceneType)
	//{
	//	return m_pRoot->createSceneManager(i_oSceneType);
	//}

	Ogre::SceneManager* Renderer3D::GetScene() const
	{
		return m_pSceneManager;
	}

	void Renderer3D::Update(real /*i_fFrametime*/, real i_fTimestep)
	{
		if (m_pMainWindow->isActive())
		{		
			m_pRoot->renderOneFrame(i_fTimestep);
		}
		else if (m_pMainWindow->isVisible())
		{
			m_pMainWindow->update();

			//CEGUI::System::getSingleton().renderGUI();
		}
	}

	void Renderer3D::PreDraw( real i_fFrametime, real i_fTimestep )
	{

	}
