#pragma once
#include "IInputManager.h"
#include "InputManager3D.h"
#include "InputManager2D.h"

class InputManagerFactory
{
public:
	static void  CreateInstance(Ogre::RenderWindow* i_pWindow);

	virtual		 ~InputManagerFactory();

	static void CreateInputManager(bool i_bIs3D, Ogre::RenderWindow* i_pWindow)
	{
		if(i_bIs3D)
		{
			m_pInputManager = InputManager3D::CreateInstance(i_pWindow);
		}
		else
		{
			m_pInputManager = InputManager2D::CreateInstance(NULL);
		}
	}

	static IInputManager* GetInputManagerPtr()
	{
		return m_pInputManager;
	}

private:
	static IInputManager* m_pInputManager;
};

