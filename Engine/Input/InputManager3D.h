#pragma once
#include "../Common/DataTypes.h"
#include "OgreRenderWindow.h"
#include "../OISInputManager.h"
#include "../OISPrereqs.h"
#include "../OISKeyboard.h"
#include "../OISMouse.h"
#include "OgreWindowEventUtilities.h"
#include "CEGUIInputEvent.h"
#include "IInputManager.h"

class InputManager3D : public IInputManager, public OIS::MouseListener, public OIS::KeyListener, public Ogre::WindowEventListener
{
public:
	static IInputManager* CreateInstance(Ogre::RenderWindow* i_pWindow);

	virtual		 ~InputManager3D();

	virtual void Update(real i_fFrametime, real i_fTimestep);
	
	bool IsShutDown() const;

	virtual bool mouseMoved( const OIS::MouseEvent &arg );
	virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	virtual bool keyPressed(const OIS::KeyEvent &arg);
	virtual bool keyReleased(const OIS::KeyEvent &arg);

	static bool IsKeyDown(const char* i_Key);
	static bool IsKeyUp(const char* i_Key);
	static bool IsMouseButtonDown(const char* i_MouseButton);
	static bool IsMouseButtonUp(const char* i_MouseButton);

private:
	InputManager3D(Ogre::RenderWindow* i_pWindow);

	Ogre::RenderWindow* m_pWindow;
	OIS::InputManager*	m_pInputManager;
	OIS::Keyboard*		m_pKeyboard;
	OIS::Mouse*			m_pMouse;

	bool				m_bShoutdown;

	MGDVector<CEGUI::MouseButton>	m_MouseButtonDown;
	MGDVector<OIS::KeyCode>				m_KeyButtonDown;
};

