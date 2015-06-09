#include "RenderingSystem.h"
#include "OgreEntity.h"
#include "OgreSceneNode.h"
#include <string>
#include "OgreConfigFile.h"
#include "OgreResourceGroupManager.h"
#include "..\Graphics\BaseGfxComponent.h"
#include "..\Graphics\MeshGfxComponent.h"
#include "..\OtherComponents\TransformationComponent.h"
#include "..\Graphics\CameraComponent.h"
#include "..\Graphics\LightComponent.h"
#include "../Common/Utilities.h"
#include "RendererFactory.h"

#include "../LOG/LOGManager.h"
#include "../Manager/SystemManager.h"

ID_IMPLEMENTATION(RenderingSystem);

RenderingSystem::RenderingSystem(Ogre::RenderWindow* i_pMainWindow, Ogre::SceneManager* i_pSceneManager)
	:System()
	, m_pMainWindow(i_pMainWindow)
	, m_pSceneManager(i_pSceneManager)
	, m_pActiveCamera(NULL)
{
	m_oMeshGfxEvent.Subscribe(this,&RenderingSystem::CreateMesh,ObjectId("Registered/MeshGfxComponent"));
	m_oBaseGfxEvent.Subscribe(this,&RenderingSystem::CreateBaseGfx,ObjectId("Registered/BaseGfxComponent"));
	m_oCameraEvent.Subscribe(this,&RenderingSystem::CreateLights,ObjectId("Registered/LightComponent"));
	m_oLightEvent.Subscribe(this,&RenderingSystem::CreateCameras,ObjectId("Registered/CameraComponent"));

	m_oDeleteMeshGfxEvent.Subscribe(this,&RenderingSystem::DeleteMesh,ObjectId("Unregistered/MeshGfxComponent"));
	m_oDeleteBaseGfxEvent.Subscribe(this,&RenderingSystem::DeleteBaseGfx,ObjectId("Unregistered/BaseGfxComponent"));
	m_oDeleteCameraEvent.Subscribe(this,&RenderingSystem::DeleteLights,ObjectId("Unregistered/LightComponent"));
	m_oDeleteLightEvent.Subscribe(this,&RenderingSystem::DeleteCameras,ObjectId("Unregistered/CameraComponent"));
}

RenderingSystem::~RenderingSystem()
{
	MGD_LOG::LOGManager::GetSingleton().WriteLog(MGD_LOG::MGD_INFO, SYSTEMS_CONTEXT, "RenderingSystem destroyed!");
}

void RenderingSystem::Reset()
{
	if(m_pSceneManager)
	{
		m_pSceneManager->clearScene();
	}
}

void RenderingSystem::Init()
{
	if(m_pSceneManager)
	{
		Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

		MGD_LOG::LOGManager::GetSingleton().WriteLog(MGD_LOG::MGD_INFO, SYSTEMS_CONTEXT, "RenderingSystem initialized!");
	}
	else
	{
		MGD_LOG::LOGManager::GetSingleton().WriteLog(MGD_LOG::MGD_ERROR, SYSTEMS_CONTEXT, "SceneManager is missing!");
	}
}

/*
Create functions
*/
void RenderingSystem::CreateCameras()
{
	const EntityComponentTable* pCameraComponentMap=  SystemManager::GetSingleton().GetComponentTable(CameraComponent::ID);
	if(pCameraComponentMap == NULL)
	{	
		MGD_LOG::LOGManager::GetSingleton().WriteLog(MGD_LOG::MGD_WARNING, SYSTEMS_CONTEXT, "No camera!");
		return;
	}
	for(EntityComponentTable::const_iterator ecIt= pCameraComponentMap->begin(); ecIt != pCameraComponentMap->end(); ++ecIt)
	{
		MGDVector<Component*>::const_iterator itCameraComponent = ecIt->second.begin();
		for(; itCameraComponent != ecIt->second.end(); ++itCameraComponent)		
		{
			const CameraComponent* pCameraComponent= static_cast<CameraComponent*>(*itCameraComponent);
			if(pCameraComponent)
			{
				try
				{
					if(m_pSceneManager && !m_pSceneManager->hasCamera(pCameraComponent->GetTextName()))
					{
						Ogre::Camera* pCamera = m_pSceneManager->createCamera(pCameraComponent->GetTextName());
						if(pCamera)
						{
							//Improve
							pCamera->setAspectRatio(pCameraComponent->GetAspectRatio());
							//******

							if (pCameraComponent->IsOrthographic())
							{
								pCamera->setProjectionType(Ogre::PT_ORTHOGRAPHIC);
							}
							else
							{
								pCamera->setProjectionType(Ogre::PT_PERSPECTIVE);
							}

							const TransformationComponent* pTransformationComponent= NULL;
							const Component* pComponent = SystemManager::GetSingleton().EditSingleComponent(TransformationComponent::ID, pCameraComponent->GetOwnerID());
							if(pComponent)
							{
								pTransformationComponent = static_cast<const TransformationComponent*>(pComponent);
								if(pTransformationComponent)
								{
									pCamera->setPosition( pTransformationComponent->GetPosition() );		
									pCamera->lookAt( pTransformationComponent->GetDirection() );

									m_vCameras[pCameraComponent->GetTextName()] = CameraInfo(pTransformationComponent, pCameraComponent);
								}
							}

							pCamera->setNearClipDistance(pCameraComponent->GetNearClipDistance());
						}
					}
					else
					{
						MGD_LOG::LOGManager::GetSingleton().WriteLog(MGD_LOG::MGD_ERROR, SYSTEMS_CONTEXT, "The camera %s  already exists!", pCameraComponent->GetTextName().c_str());
					}
				}
				catch(Ogre::Exception& e)
				{
					MGD_LOG::LOGManager::GetSingleton().WriteLog(MGD_LOG::MGD_ERROR, SYSTEMS_CONTEXT, e.getFullDescription().c_str());
				}
			}
		}
	}
}

void RenderingSystem::CreateLights()
{
	const EntityComponentTable* pLightComponentMap=  SystemManager::GetSingleton().GetComponentTable(LightComponent::ID);
	if(pLightComponentMap == NULL)
	{	
		MGD_LOG::LOGManager::GetSingleton().WriteLog(MGD_LOG::MGD_WARNING, SYSTEMS_CONTEXT, "No Lights");
		return;
	}
	for(EntityComponentTable::const_iterator ecIt= pLightComponentMap->begin(); ecIt != pLightComponentMap->end(); ++ecIt)
	{
		MGDVector<Component*>::const_iterator itLightComponent = ecIt->second.begin();
		for(; itLightComponent != ecIt->second.end(); ++itLightComponent)
		{
			LightComponent* pLightComponent= static_cast<LightComponent*>(*itLightComponent);
			if(pLightComponent)
			{
				try
				{
					if(m_pSceneManager && !m_pSceneManager->hasLight(pLightComponent->GetTextName()))
					{
						// Create a Light and set its position
						Ogre::Light* pLight = m_pSceneManager->createLight(pLightComponent->GetTextName());
						if(pLight)
						{
							pLight->setType(pLightComponent->GetType());
							pLight->setVisible(pLightComponent->IsEnable());
							pLight->setDiffuseColour(pLightComponent->GetDiffuseColor());
							pLight->setSpecularColour(pLightComponent->GetSpecularColor());
							pLight->setSpotlightRange(Ogre::Radian(Ogre::Degree(pLightComponent->GetRangeX()).valueRadians()), Ogre::Radian(Ogre::Degree(pLightComponent->GetRangeY()).valueRadians()));

							const TransformationComponent* pTransformationComponent= NULL;
							const Component* pComponent = SystemManager::GetSingleton().EditSingleComponent(TransformationComponent::ID, pLightComponent->GetOwnerID());
							if(pComponent)
							{
								pTransformationComponent = static_cast<const TransformationComponent*>(pComponent);
								if(pTransformationComponent)
								{
									pLight->setPosition(pTransformationComponent->GetPosition());
									pLight->setDirection(pTransformationComponent->GetDirection());

									m_vLights[pLightComponent->GetTextName()] = LightInfo(pTransformationComponent, pLightComponent);
								}
							}		
						}
					}
					else
					{
						MGD_LOG::LOGManager::GetSingleton().WriteLog(MGD_LOG::MGD_ERROR, SYSTEMS_CONTEXT, "The light %s already exists!", pLightComponent->GetTextName().c_str());
					}

				}
				catch(Ogre::Exception& e)
				{
					MGD_LOG::LOGManager::GetSingleton().WriteLog(MGD_LOG::MGD_ERROR, SYSTEMS_CONTEXT, e.getFullDescription().c_str());
				}
			}
		}
	}
}

void RenderingSystem::CreateMesh()
{
	const EntityComponentTable* pGfxComponentMap=  SystemManager::GetSingleton().GetComponentTable(MeshGfxComponent::ID);
	if(pGfxComponentMap == NULL)
	{	
		MGD_LOG::LOGManager::GetSingleton().WriteLog(MGD_LOG::MGD_INFO, SYSTEMS_CONTEXT, "No MeshGfxComponent to render");		
	}
	else
	{
		for(EntityComponentTable::const_iterator ecIt= pGfxComponentMap->begin(); ecIt != pGfxComponentMap->end(); ++ecIt)
		{
			try
			{
				MeshGfxComponent* pGfxComponent= static_cast<MeshGfxComponent*>(SystemManager::GetSingleton().EditSingleComponent(MeshGfxComponent::ID, ecIt->first));
				if(pGfxComponent  && !pGfxComponent->IsInitialize() && m_pSceneManager && !m_pSceneManager->hasEntity(pGfxComponent->GetTextName()))
				{
					Ogre::SceneNode* pNode = m_pSceneManager->getRootSceneNode()->createChildSceneNode(pGfxComponent->GetTextName());
					Ogre::Entity* pEntity = m_pSceneManager->createEntity(pGfxComponent->GetTextName(), pGfxComponent->GetMeshName());
					pEntity->setMaterialName(pGfxComponent->GetMaterialName());
					pEntity->setCastShadows(pGfxComponent->GetShadowEnabled());
					pNode->attachObject(pEntity);

					const TransformationComponent* pTransformationComponent= NULL;
					const Component* component= SystemManager::GetSingleton().EditSingleComponent(TransformationComponent::ID, pGfxComponent->GetOwnerID());
					if(component != NULL)
					{
						pTransformationComponent = static_cast<const TransformationComponent*>(component);
						if(pTransformationComponent)
						{
							pNode->setPosition(pTransformationComponent->GetPosition());
							pNode->setOrientation(pTransformationComponent->GetRotation());
							pNode->setScale(pTransformationComponent->GetScale());
						}

						m_vMeshGfx[pGfxComponent->GetTextName()] = pTransformationComponent;
					}

					pGfxComponent->SetInit();
				}

			}
			catch(Ogre::Exception& e)
			{
				MGD_LOG::LOGManager::GetSingleton().WriteLog(MGD_LOG::MGD_ERROR, SYSTEMS_CONTEXT, e.getFullDescription().c_str());
			}
		}
	}
}

void RenderingSystem::CreateBaseGfx()
{
	const EntityComponentTable* pBaseGfxComponentMap=  SystemManager::GetSingleton().GetComponentTable(BaseGfxComponent::ID);
	if(pBaseGfxComponentMap == NULL)
	{	
		MGD_LOG::LOGManager::GetSingleton().WriteLog(MGD_LOG::MGD_INFO, SYSTEMS_CONTEXT, "No BaseGfxComponent to render");
	}
	else
	{
		for(EntityComponentTable::const_iterator ecIt= pBaseGfxComponentMap->begin(); ecIt != pBaseGfxComponentMap->end(); ++ecIt)
		{
			try
			{
				BaseGfxComponent* pGfxComponent= static_cast<BaseGfxComponent*>(SystemManager::GetSingleton().EditSingleComponent(BaseGfxComponent::ID, ecIt->first));
				if(pGfxComponent && !pGfxComponent->IsInitialize() && m_pSceneManager && !m_pSceneManager->hasEntity(pGfxComponent->GetTextName()))
				{
					Ogre::SceneNode* pNode = m_pSceneManager->getRootSceneNode()->createChildSceneNode(pGfxComponent->GetTextName());

					Ogre::Entity* pEntity = NULL;
					switch(pGfxComponent->GetPrefabType())
					{
					case  BaseGfxComponent::CUBE:
						pEntity = m_pSceneManager->createEntity(pGfxComponent->GetTextName(), Ogre::SceneManager::PT_CUBE);		
						break;

					case  BaseGfxComponent::PLANE:
						pEntity = m_pSceneManager->createEntity(pGfxComponent->GetTextName(), Ogre::SceneManager::PT_PLANE);		
						break;

					case  BaseGfxComponent::SPHERE:
						pEntity = m_pSceneManager->createEntity(pGfxComponent->GetTextName(), Ogre::SceneManager::PT_SPHERE);		
						break;
					default:
						MGD_ASSERT(0);
						return;
					}

					pEntity->setMaterialName(pGfxComponent->GetMaterialName());
					pEntity->setCastShadows(pGfxComponent->GetShadowEnabled());
					pNode->attachObject(pEntity);

					const TransformationComponent* pTransformationComponent= NULL;
					const Component* component= SystemManager::GetSingleton().EditSingleComponent(TransformationComponent::ID, pGfxComponent->GetOwnerID());
					if(component != NULL)
					{
						pTransformationComponent = static_cast<const TransformationComponent*>(component);
						if(pTransformationComponent)
						{
							pNode->setPosition(pTransformationComponent->GetPosition());
							pNode->setOrientation(pTransformationComponent->GetRotation());
							pNode->setScale(pTransformationComponent->GetScale());

							m_vBaseGfx[pGfxComponent->GetTextName()] = pTransformationComponent;
						}
					}

					pGfxComponent->SetInit();
				}		

			}
			catch(Ogre::Exception& e)
			{
				MGD_LOG::LOGManager::GetSingleton().WriteLog(MGD_LOG::MGD_ERROR, SYSTEMS_CONTEXT, e.getFullDescription().c_str());
			}
		}
	}
}

/*
Delete functions
*/
void RenderingSystem::DeleteCameras(const char* i_szName)
{
	const EntityComponentTable* pCameraComponentMap=  SystemManager::GetSingleton().GetComponentTable(CameraComponent::ID);
	if(pCameraComponentMap)
	{	
		for(EntityComponentTable::const_iterator ecIt= pCameraComponentMap->begin(); ecIt != pCameraComponentMap->end(); ++ecIt)
		{
			CameraComponent* pCameraComponent= static_cast<CameraComponent*>(SystemManager::GetSingleton().EditSingleComponent(CameraComponent::ID, ecIt->first));
			if(pCameraComponent)
			{
				if(m_pSceneManager && pCameraComponent->GetTextName().compare(i_szName) == 0)
				{
					try
					{
						m_pSceneManager->destroyCamera(i_szName);
						m_vCameras.erase(i_szName);
					}
					catch(Ogre::Exception& e)
					{
						MGD_LOG::LOGManager::GetSingleton().WriteLog(MGD_LOG::MGD_ERROR, SYSTEMS_CONTEXT, e.getFullDescription().c_str());
					}
				}
			}
		}
	}
}

void RenderingSystem::DeleteLights(const char* i_szName)
{
	const EntityComponentTable* pLightComponentMap=  SystemManager::GetSingleton().GetComponentTable(LightComponent::ID);
	if(pLightComponentMap)
	{
		for(EntityComponentTable::const_iterator ecIt = pLightComponentMap->begin(); ecIt != pLightComponentMap->end(); ++ecIt)
		{
			LightComponent* pLightComponent= static_cast<LightComponent*>(SystemManager::GetSingleton().EditSingleComponent(LightComponent::ID, ecIt->first));
			if(pLightComponent)
			{
				if(m_pSceneManager && pLightComponent->GetTextName().compare(i_szName) == 0)
				{
					try
					{
						m_pSceneManager->destroyLight(i_szName);
						m_vLights.erase(i_szName);
					}
					catch(Ogre::Exception& e)
					{
						MGD_LOG::LOGManager::GetSingleton().WriteLog(MGD_LOG::MGD_ERROR, SYSTEMS_CONTEXT, e.getFullDescription().c_str());
					}
				}
			}
		}
	}
}

void RenderingSystem::DeleteMesh(const char* i_szName)
{
	const EntityComponentTable* pGfxComponentMap=  SystemManager::GetSingleton().GetComponentTable(MeshGfxComponent::ID);
	if(pGfxComponentMap)
	{
		for(EntityComponentTable::const_iterator ecIt= pGfxComponentMap->begin(); ecIt != pGfxComponentMap->end(); ++ecIt)
		{
			MeshGfxComponent* pGfxComponent= static_cast<MeshGfxComponent*>(SystemManager::GetSingleton().EditSingleComponent(MeshGfxComponent::ID, ecIt->first));
			if(pGfxComponent)
			{
				if(m_pSceneManager && pGfxComponent->GetTextName().compare(i_szName) == 0)
				{
					try
					{
						m_pSceneManager->destroyEntity(i_szName);
						m_pSceneManager->destroySceneNode(i_szName);

						m_vMeshGfx.erase(i_szName);

					}
					catch(Ogre::Exception& e)
					{
						MGD_LOG::LOGManager::GetSingleton().WriteLog(MGD_LOG::MGD_ERROR, SYSTEMS_CONTEXT, e.getFullDescription().c_str());
					}
				}
			}
		}
	}	
}

void RenderingSystem::DeleteBaseGfx(const char* i_szName)
{
	const EntityComponentTable* pBaseGfxComponentMap=  SystemManager::GetSingleton().GetComponentTable(BaseGfxComponent::ID);
	if(pBaseGfxComponentMap)
	{
		for(EntityComponentTable::const_iterator ecIt = pBaseGfxComponentMap->begin(); ecIt != pBaseGfxComponentMap->end(); ++ecIt)
		{
			BaseGfxComponent* pGfxComponent= static_cast<BaseGfxComponent*>(SystemManager::GetSingleton().EditSingleComponent(BaseGfxComponent::ID, ecIt->first));
			if(pGfxComponent)
			{
				if(m_pSceneManager && pGfxComponent->GetTextName().compare(i_szName) == 0)
				{
					try
					{
						m_pSceneManager->destroyEntity(i_szName);
						m_pSceneManager->destroySceneNode(i_szName);
						m_vBaseGfx.erase(i_szName);
					}
					catch(Ogre::Exception& e)
					{
						MGD_LOG::LOGManager::GetSingleton().WriteLog(MGD_LOG::MGD_ERROR, SYSTEMS_CONTEXT, e.getFullDescription().c_str());
					}
				}
			}			
		}
	}
}

/*
Update functions
*/

void RenderingSystem::UpdateMeshGfx()
{
	if(m_pSceneManager)
	{
		for (MGDMap<std::string, const TransformationComponent*>::const_iterator ecIt = m_vMeshGfx.begin(); ecIt != m_vMeshGfx.end(); ++ecIt)
		{
			Ogre::SceneNode* pNode = m_pSceneManager->getSceneNode(ecIt->first);
			if (pNode)
			{
				const TransformationComponent* pTransformationComponent = ecIt->second;
				if (pTransformationComponent)
				{
					pNode->setPosition(pTransformationComponent->GetPosition());
					pNode->setOrientation(pTransformationComponent->GetRotation());
					pNode->setScale(pTransformationComponent->GetScale());
				}
			}
		}
	}
}

void RenderingSystem::UpdateBaseGfx()
{
	if(m_pSceneManager)
	{
		for (MGDMap<std::string, const TransformationComponent*>::const_iterator ecIt = m_vBaseGfx.begin(); ecIt != m_vBaseGfx.end(); ++ecIt)
		{
			Ogre::SceneNode* pNode = m_pSceneManager->getSceneNode(ecIt->first);
			if (pNode)
			{
				const TransformationComponent* pTransformationComponent = ecIt->second;
				if (pTransformationComponent)
				{
					pNode->setPosition(pTransformationComponent->GetPosition());
					pNode->setOrientation(pTransformationComponent->GetRotation());
					pNode->setScale(pTransformationComponent->GetScale());
				}
			}
		}
	}
}

void RenderingSystem::UpdateLights()
{
	for (MGDMap<std::string, LightInfo>::const_iterator ecIt = m_vLights.begin(); ecIt != m_vLights.end(); ++ecIt)
	{
		const LightComponent* pLightComponent = ecIt->second.m_pLightComponent;
		if (m_pSceneManager && pLightComponent)
		{
			try
			{
				Ogre::Light* pLight = m_pSceneManager->getLight(pLightComponent->GetTextName());
				if (pLight)
				{
					pLight->setType(pLightComponent->GetType());
					pLight->setVisible(pLightComponent->IsEnable());
					pLight->setDiffuseColour(pLightComponent->GetDiffuseColor());
					pLight->setSpecularColour(pLightComponent->GetSpecularColor());
					pLight->setSpotlightRange(Ogre::Radian(Ogre::Degree(pLightComponent->GetRangeX()).valueRadians()), Ogre::Radian(Ogre::Degree(pLightComponent->GetRangeY()).valueRadians()));


					const TransformationComponent* pTransformationComponent = ecIt->second.m_pTransformationComponent;
					if (pTransformationComponent)
					{
						pLight->setPosition(pTransformationComponent->GetPosition());
						pLight->setDirection(pTransformationComponent->GetDirection());
					}
				}
			}
			catch (Ogre::Exception& e)
			{
				MGD_LOG::LOGManager::GetSingleton().WriteLog(MGD_LOG::MGD_ERROR, SYSTEMS_CONTEXT, e.getFullDescription().c_str());
			}
		}
	}
}

void RenderingSystem::UpdateCameras()
{
	for (MGDMap<std::string, CameraInfo>::const_iterator ecIt = m_vCameras.begin(); ecIt != m_vCameras.end(); ++ecIt)
	{
		const CameraComponent* pCameraComponent = ecIt->second.m_pCameraComponent;
		if (m_pSceneManager && pCameraComponent && pCameraComponent->IsEnable())
		{
			Ogre::Camera* pCamera = m_pSceneManager->getCamera(pCameraComponent->GetTextName());
			if (pCamera)
			{
				//Improve
				if (m_pActiveCamera != pCameraComponent)
				{
					if (m_pActiveCamera)
					{
						m_pActiveCamera->SetEnable(false);
					}

					m_pActiveCamera = const_cast<CameraComponent*>(pCameraComponent);
					if(RendererFactory::GetRendererPtr()->GetViewport())
					{
						RendererFactory::GetRendererPtr()->GetViewport()->setCamera(pCamera);
					}
				}
				//****
				const TransformationComponent* pTransformationComponent = ecIt->second.m_pTransformationComponent;
				if (pTransformationComponent)
				{
					pCamera->setPosition(pTransformationComponent->GetPosition());
					if (pCameraComponent->IsFixedAxis())
					{
						pCamera->lookAt(pTransformationComponent->GetDirection());
					}
				}
			}
		}
	}
}


void RenderingSystem::Update(real /*i_fFrametime*/, real i_fTimestep)
{	
	UpdateCameras();
	UpdateLights();
	UpdateMeshGfx();
	UpdateBaseGfx();
}