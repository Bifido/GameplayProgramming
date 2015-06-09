#include "InputManager3D.h"
#include "../LOG/LogManager.h"
#include "CEGUISystem.h"
#include "LuaObject.h"
#include "../Manager/LuaManager.h"
#include "CEGUIExceptions.h"
#include "InputManagerFactory.h"

CEGUI::MouseButton convertButton(OIS::MouseButtonID buttonID)
{
	switch (buttonID)
	{
	case OIS::MB_Left:
		return CEGUI::LeftButton;
		break;

	case OIS::MB_Right:
		return CEGUI::RightButton;
		break;

	case OIS::MB_Middle:
		return CEGUI::MiddleButton;
		break;

	default:
		return CEGUI::LeftButton;
		break;
	}
}

CEGUI::MouseButton convertButton(const char* buttonID)
{
	if(strcmp(buttonID,"LEFT_BUTTON") == 0)
	{
		return CEGUI::LeftButton;
	}
	else if(strcmp(buttonID,"RIGHT_BUTTON") == 0)
	{
		return CEGUI::RightButton;
	}
	else if(strcmp(buttonID,"MID_BUTTON") == 0)
	{
		return CEGUI::MiddleButton;
	}

	return CEGUI::LeftButton;
}

IInputManager* InputManager3D::CreateInstance(Ogre::RenderWindow* i_pWindow)
{
	return new InputManager3D(i_pWindow);
}


InputManager3D::InputManager3D(Ogre::RenderWindow* i_pWindow)
	: m_pWindow(i_pWindow)
	, m_pInputManager(NULL)
	, m_bShoutdown(false)
{		

	LuaPlus::LuaObject pGlobalVars = LuaManager::GetSingleton().GetGlobalVars();
	pGlobalVars.RegisterDirect("IsKeyDown", &InputManager3D::IsKeyDown);		
	pGlobalVars.RegisterDirect("IsMouseButtonDown", &InputManager3D::IsMouseButtonDown);

	OIS::ParamList pl;
	size_t windowHnd = 0;
	std::ostringstream windowHndStr;

	m_pWindow->getCustomAttribute("WINDOW", &windowHnd);
	windowHndStr << windowHnd;
	pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

	m_pInputManager = OIS::InputManager::createInputSystem( pl );
	m_pKeyboard = static_cast<OIS::Keyboard*>(m_pInputManager->createInputObject( OIS::OISKeyboard, true ));
	m_pMouse = static_cast<OIS::Mouse*>(m_pInputManager->createInputObject( OIS::OISMouse, true ));

	m_pMouse->setEventCallback(this);
	m_pKeyboard->setEventCallback(this);

	//Set initial mouse clipping size

	if(m_pWindow)
	{
		windowMoved(m_pWindow);
		windowResized(m_pWindow);
		windowFocusChange(m_pWindow);
	}

	//Register as a Window listener
	Ogre::WindowEventUtilities::addWindowEventListener(m_pWindow, this);	
}

InputManager3D::~InputManager3D()
{
}

void InputManager3D::Update(real /*i_fFrametime*/, real /*i_fTimestep*/)
{
	Ogre::WindowEventUtilities::messagePump();
	if(m_pKeyboard)
		m_pKeyboard->capture();
	if(m_pMouse)
		m_pMouse->capture();
}

bool InputManager3D::keyPressed( const OIS::KeyEvent &arg )
{
	try
	{
		CEGUI::System &sys = CEGUI::System::getSingleton();
		sys.injectKeyDown(arg.key);
		sys.injectChar(arg.text);
		m_KeyButtonDown.push_back(arg.key);
	}
	catch(CEGUI::Exception& e)
	{
		MGD_LOG::LOGManager::GetSingleton().WriteLog(MGD_LOG::MGD_ERROR, INPUT_CONTEXT, e.getMessage().c_str());
		return false;
	}
	return true;
}

bool InputManager3D::keyReleased( const OIS::KeyEvent &arg )
{
	try
	{
		CEGUI::System::getSingleton().injectKeyUp(arg.key);	

		MGDVector<OIS::KeyCode>::iterator it = m_KeyButtonDown.begin();
		while(it != m_KeyButtonDown.end())
		{
			if(*it == arg.key)
			{
				it = m_KeyButtonDown.erase(it);
			}
			else
			{
				++it;
			}
		}	
	}
	catch(CEGUI::Exception& e)
	{
		MGD_LOG::LOGManager::GetSingleton().WriteLog(MGD_LOG::MGD_ERROR, INPUT_CONTEXT, e.getMessage().c_str());
		return false;
	}
	return true;
}

bool InputManager3D::mouseMoved( const OIS::MouseEvent &arg )
{
	try	
	{
		if(CEGUI::System::getSingletonPtr())
		{
			CEGUI::System::getSingleton().injectMouseMove(arg.state.X.rel, arg.state.Y.rel);
		}
	}
	catch(CEGUI::Exception& e)
	{
		MGD_LOG::LOGManager::GetSingleton().WriteLog(MGD_LOG::MGD_ERROR, INPUT_CONTEXT, e.getMessage().c_str());
	}

	return true;
}

bool InputManager3D::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	try
	{

		if(CEGUI::System::getSingletonPtr())
		{
			CEGUI::System::getSingleton().injectMouseButtonDown(convertButton(id));
			m_MouseButtonDown.push_back(convertButton(id));
		}
	}
	catch(CEGUI::Exception& e)
	{
		MGD_LOG::LOGManager::GetSingleton().WriteLog(MGD_LOG::MGD_ERROR, INPUT_CONTEXT, e.getMessage().c_str());
		return false;
	}

	return true;
}

bool InputManager3D::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	try
	{

		if(CEGUI::System::getSingletonPtr())
		{
			CEGUI::System::getSingleton().injectMouseButtonUp(convertButton(id));
			MGDVector<CEGUI::MouseButton>::iterator it = m_MouseButtonDown.begin();
			while(it != m_MouseButtonDown.end())
			{
				if(*it == convertButton(id))
				{
					it = m_MouseButtonDown.erase(it);
				}
				else
				{
					++it;
				}
			}
		}

	}
	catch(CEGUI::Exception& e)
	{
		MGD_LOG::LOGManager::GetSingleton().WriteLog(MGD_LOG::MGD_ERROR, INPUT_CONTEXT, e.getMessage().c_str());
		return false;
	}
	return true;
}

bool InputManager3D::IsKeyDown( const char* i_szKey )
{
	MGDVector<OIS::KeyCode>::iterator it = static_cast<InputManager3D*>(InputManagerFactory::GetInputManagerPtr())->m_KeyButtonDown.begin();
	while(it != static_cast<InputManager3D*>(InputManagerFactory::GetInputManagerPtr())->m_KeyButtonDown.end())
	{
		std::string key = static_cast<InputManager3D*>(InputManagerFactory::GetInputManagerPtr())->m_pKeyboard->getAsString(*it);
		if(key.compare(i_szKey) == 0)
		{
			return true;
		}
		++it;
	}	

	return false;
}

bool InputManager3D::IsKeyUp( const char* i_Key )
{
	return false;
}

bool InputManager3D::IsMouseButtonDown( const char* i_MouseButton )
{
	CEGUI::MouseButton oMouseButton = convertButton(i_MouseButton);

	/*MGDVector<CEGUI::MouseButton>::iterator it = GetSingleton().m_MouseButtonDown.begin();
	while(it != GetSingleton().m_MouseButtonDown.end())
	{
		if(*it == oMouseButton)
		{
			return false;
		}
		else
		{
			++it;
		}
	}	*/
	return false;
}

bool InputManager3D::IsMouseButtonUp( const char* i_MouseButton )
{
	return false;
}

bool InputManager3D::IsShutDown() const
{
	return m_bShoutdown;
}