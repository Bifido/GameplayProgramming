/*
*	This class implements the base render functions and manage all the draw calls
*/

#pragma once
#include "IRenderer.h"
#include "SDL_video.h"
#include "SDL_render.h"
struct SDL_Window;

class Renderer2D : public IRenderer
{
public:
	static IRenderer* CreateInstance();
	virtual ~Renderer2D();

	//Ogre::SceneManager* CreateScene(Ogre::SceneType i_oSceneType = Ogre::ST_GENERIC);
	Ogre::SceneManager* GetScene() const;

	//I would like to remove this dependecy between RenderWindow and the Systems.
	Ogre::RenderWindow* GetWindow() const;

	//
	Ogre::Root*	GetRenderRoot() const;

	Ogre::Viewport*	GetViewport() const;

	virtual void Update(real i_fFrametime, real i_fTimestep);
	virtual void PreDraw(real i_fFrametime, real i_fTimestep);
	SDL_Renderer* m_pRenderer;
private:
	Renderer2D();	
	void ConfigureSceneManager();

	SDL_Window*	m_pWindow;
	SDL_GLContext m_oGLContext;
	
};

inline Ogre::RenderWindow* Renderer2D::GetWindow() const
{
	return NULL;
}

inline Ogre::Root* Renderer2D::GetRenderRoot() const
{
	return NULL;
}

inline Ogre::Viewport*	Renderer2D::GetViewport() const
{
	return NULL;
}

inline Ogre::SceneManager* Renderer2D::GetScene() const
{
	return NULL;
}
