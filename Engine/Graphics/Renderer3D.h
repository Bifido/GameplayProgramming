/*
*	This class implements the base render functions and manage all the draw calls
*/

#pragma once
#include "OgreSceneManager.h"
#include "OgreRoot.h"
#include "OgreRenderWindow.h"
#include "../Core/Singleton.h"
#include "../Common/Utilities.h"
#include "IRenderer.h"


class Renderer3D : public IRenderer
{
public:
	static IRenderer* CreateInstance();
	virtual ~Renderer3D();

	//Ogre::SceneManager* CreateScene(Ogre::SceneType i_oSceneType = Ogre::ST_GENERIC);
	Ogre::SceneManager* GetScene() const;

	//I would like to remove this dependecy between RenderWindow and the Systems.
	Ogre::RenderWindow* GetWindow() const;

	//
	Ogre::Root*	GetRenderRoot() const;

	Ogre::Viewport*	GetViewport() const;

	virtual void Update(real i_fFrametime, real i_fTimestep);
	virtual void PreDraw(real i_fFrametime, real i_fTimestep);

private:
	Renderer3D();
	void ConfigureSceneManager();

	Ogre::Root*			m_pRoot;
	Ogre::RenderWindow* m_pMainWindow;
	Ogre::SceneManager* m_pSceneManager;
	Ogre::Viewport*		m_pViewPort;

	static std::string PLUGIN_CFG;
};

inline Ogre::RenderWindow* Renderer3D::GetWindow() const
{
	return m_pMainWindow;
}

inline Ogre::Root* Renderer3D::GetRenderRoot() const
{
	return m_pRoot;
}

inline Ogre::Viewport*	Renderer3D::GetViewport() const
{
	return m_pViewPort;
}