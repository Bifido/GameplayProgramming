#pragma once
#include "..\Common\DataTypes.h"

namespace Ogre
{
	class SceneManager;
	class RenderWindow;
	class Root;
	class Viewport;
}

class IRenderer
{
public:
	IRenderer()
		: m_bFullscreen(false)
		, m_fPosX(0)
		, m_fPosY(0)
		, m_fWidth(640)
		, m_fHeight(480)
	{
		m_fClearColor[0] = 0.0f;
		m_fClearColor[1] = 0.0f;
		m_fClearColor[2] = 0.0f;
		m_fClearColor[3] = 1.0f;

		m_fOrtho[0] = -1.0f;
		m_fOrtho[1] = 1.0f;
		m_fOrtho[2] = -1.0f;
		m_fOrtho[3] = 1.0f;
		m_fOrtho[4] = -1.0f;
		m_fOrtho[5] = 1.0f;
	}
	virtual ~IRenderer() {}

	virtual Ogre::SceneManager* GetScene() const = 0;
	virtual Ogre::RenderWindow* GetWindow() const  = 0;
	virtual Ogre::Root*	GetRenderRoot() const  = 0;
	virtual Ogre::Viewport*	GetViewport() const  = 0;
	virtual void PreDraw(real i_fFrametime, real i_fTimestep) = 0;
	virtual void Update(real i_fFrametime, real i_fTimestep) = 0;

protected:
	bool m_bFullscreen;
	real m_fPosX;
	real m_fPosY;
	real m_fWidth; 
	real m_fHeight;
	real m_fClearColor[4];
	real m_fOrtho[6];
};