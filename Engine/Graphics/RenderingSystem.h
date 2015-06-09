/*
*	This system is used to manage all the gfx component as MeshGfxComponents, baseGfxComponents, CameraComponents and LightComponent
*/

#pragma once
#include "../Core/System.h"
#include "../Common/Utilities.h"

#include "OgreSceneManager.h"
#include "OgreRoot.h"
#include "OgreRenderWindow.h"
#include "../Event/RegistryEventHandler.h"
#include "../OtherComponents/TransformationComponent.h"
#include "CameraComponent.h"
#include "LightComponent.h"

class RenderingSystem : public System
{
public:
	ID_DECLARATION;

	RenderingSystem(Ogre::RenderWindow* i_pMainWindow, Ogre::SceneManager* i_pSceneManager);
	virtual ~RenderingSystem();

	virtual void	Init();
	virtual void	Update(real i_fFrametime, real i_fTimestep);

private:
	struct CameraInfo
	{
		CameraInfo() {}
		CameraInfo(const TransformationComponent* i_pTransformationComponent, const CameraComponent* i_pCameraComponent)
		: m_pTransformationComponent(i_pTransformationComponent)
		, m_pCameraComponent(i_pCameraComponent)
		{
		}

		const TransformationComponent* m_pTransformationComponent;
		const CameraComponent* m_pCameraComponent;
	};

	struct LightInfo
	{
		LightInfo() {}
		LightInfo( const TransformationComponent* i_pTransformationComponent, const LightComponent* i_pLightComponent )
		: m_pTransformationComponent(i_pTransformationComponent)
		, m_pLightComponent(i_pLightComponent)
		{
		}

		const TransformationComponent* m_pTransformationComponent;
		const LightComponent* m_pLightComponent;
	};

	void CreateCameras();
	void CreateLights();
	void CreateMesh();
	void CreateBaseGfx();

	void DeleteCameras(const char* i_szName);
	void DeleteLights(const char* i_szName);
	void DeleteMesh(const char* i_szName);
	void DeleteBaseGfx(const char* i_szName);

	void UpdateCameras();
	void UpdateLights();
	void UpdateBaseGfx();
	void UpdateMeshGfx();

	void Reset();
	Ogre::RenderWindow* m_pMainWindow;
	Ogre::SceneManager* m_pSceneManager;
	CameraComponent*	m_pActiveCamera;

	MGDMap<std::string, const TransformationComponent*> m_vMeshGfx;
	MGDMap<std::string, const TransformationComponent*> m_vBaseGfx;
	MGDMap<std::string, CameraInfo> m_vCameras;
	MGDMap<std::string, LightInfo> m_vLights;

	RegistryEventHandler<RenderingSystem> m_oMeshGfxEvent;
	RegistryEventHandler<RenderingSystem> m_oBaseGfxEvent;
	RegistryEventHandler<RenderingSystem> m_oCameraEvent;
	RegistryEventHandler<RenderingSystem> m_oLightEvent;

	RegistryEventHandler<RenderingSystem, const char*> m_oDeleteMeshGfxEvent;
	RegistryEventHandler<RenderingSystem, const char*> m_oDeleteBaseGfxEvent;
	RegistryEventHandler<RenderingSystem, const char*> m_oDeleteCameraEvent;
	RegistryEventHandler<RenderingSystem, const char*> m_oDeleteLightEvent;

	RegistryEventHandler<RenderingSystem> m_oResetEvent;
};

