#include "InputManager2D.h"
#include "../LOG/LogManager.h"
#include "SDL.h"
#include "SDL_keycode.h"
#include "LuaObject.h"
#include "IInputManager.h"
#include "InputManagerFactory.h"
#include "../Manager/LuaManager.h"
#include "CEGUISystem.h"
#include "CEGUIRenderingRoot.h"

IInputManager* InputManager2D::CreateInstance(Ogre::RenderWindow* /*i_pWindow*/)
{
	return new InputManager2D();
}


InputManager2D::InputManager2D()
{		

	LuaPlus::LuaObject pGlobalVars = LuaManager::GetSingleton().GetGlobalVars();
	pGlobalVars.RegisterDirect("IsKeyDown", &InputManager2D::IsKeyDown);		
}

InputManager2D::~InputManager2D()
{
}

void InputManager2D::OnEvent( void* i_pEvent )
{
	SDL_Event* pEvent = static_cast<SDL_Event*>(i_pEvent);
	if(!pEvent)
	{
		return;
	}
	switch(pEvent->type) 
	{
		/*case SDL_ACTIVEEVENT: {
		switch(Event->active.state) {
		case SDL_APPMOUSEFOCUS: {
		if ( Event->active.gain )    OnMouseFocus();
		else                OnMouseBlur();

		break;
		}
		case SDL_APPINPUTFOCUS: {
		if ( Event->active.gain )    OnInputFocus();
		else                OnInputBlur();

		break;
		}
		case SDL_APPACTIVE:    {
		if ( Event->active.gain )    OnRestore();
		else                OnMinimize();

		break;
		}
		}
		break;
		}*/
	case SDL_KEYDOWN:
		{
		OnKeyDown(pEvent->key.keysym.sym,pEvent->key.keysym.mod,pEvent->key.keysym.scancode);
		break;
		}

	case SDL_KEYUP: {
		OnKeyUp(pEvent->key.keysym.sym,pEvent->key.keysym.mod,pEvent->key.keysym.scancode);
		break;
					}

	case SDL_MOUSEMOTION: {
		OnMouseMove(pEvent->motion.x,pEvent->motion.y,pEvent->motion.xrel,pEvent->motion.yrel,(pEvent->motion.state&SDL_BUTTON(SDL_BUTTON_LEFT))!=0,(pEvent->motion.state&SDL_BUTTON(SDL_BUTTON_RIGHT))!=0,(pEvent->motion.state&SDL_BUTTON(SDL_BUTTON_MIDDLE))!=0);
		break;
						  }

	case SDL_MOUSEBUTTONDOWN: {
		switch(pEvent->button.button) {
		case SDL_BUTTON_LEFT: {
			OnLButtonDown(pEvent->button.x,pEvent->button.y);
			break;
							  }
		case SDL_BUTTON_RIGHT: {
			OnRButtonDown(pEvent->button.x,pEvent->button.y);
			break;
							   }
		case SDL_BUTTON_MIDDLE: {
			OnMButtonDown(pEvent->button.x,pEvent->button.y);
			break;
								}
		}
		break;
							  }

	case SDL_MOUSEBUTTONUP:    {
		switch(pEvent->button.button) {
		case SDL_BUTTON_LEFT: {
			OnLButtonUp(pEvent->button.x,pEvent->button.y);
			break;
							  }
		case SDL_BUTTON_RIGHT: {
			OnRButtonUp(pEvent->button.x,pEvent->button.y);
			break;
							   }
		case SDL_BUTTON_MIDDLE: {
			OnMButtonUp(pEvent->button.x,pEvent->button.y);
			break;
								}
		}
		break;
							   }

	case SDL_JOYAXISMOTION: {
		OnJoyAxis(pEvent->jaxis.which,pEvent->jaxis.axis,pEvent->jaxis.value);
		break;
							}

	case SDL_JOYBALLMOTION: {
		OnJoyBall(pEvent->jball.which,pEvent->jball.ball,pEvent->jball.xrel,pEvent->jball.yrel);
		break;
							}

	case SDL_JOYHATMOTION: {
		OnJoyHat(pEvent->jhat.which,pEvent->jhat.hat,pEvent->jhat.value);
		break;
						   }
	case SDL_JOYBUTTONDOWN: {
		OnJoyButtonDown(pEvent->jbutton.which,pEvent->jbutton.button);
		break;
							}

	case SDL_JOYBUTTONUP: {
		OnJoyButtonUp(pEvent->jbutton.which,pEvent->jbutton.button);
		break;
						  }

	case SDL_QUIT: {
		OnExit();
		break;
				   }

	case SDL_SYSWMEVENT: {
		//Ignore
		break;
						 }

	//case SDL_VIDEORESIZE: {
	//	OnResize(Event->resize.w,Event->resize.h);
	//	break;
	//					  }

	//case SDL_VIDEOEXPOSE: {
	//	OnExpose();
	//	break;
	//					  }

	default: {
		OnUser(pEvent->user.type,pEvent->user.code,pEvent->user.data1,pEvent->user.data2);
		break;
			 }
	}
}

void InputManager2D::OnInputFocus()
{
}

void InputManager2D::OnInputBlur()
{
}

void InputManager2D::OnKeyDown(SDL_Keycode sym, Uint16 mod, Uint16 unicode)
{
	m_KeyButtonDown.push_back(sym);
}

void InputManager2D::OnKeyUp(SDL_Keycode sym, Uint16 mod, Uint16 unicode)
{
	MGDVector<int32>::iterator it = m_KeyButtonDown.begin();
	while(it != m_KeyButtonDown.end())
	{
		if(*it == sym)
		{
			it = m_KeyButtonDown.erase(it);
		}
		else
		{
			++it;
		}
	}	
}

void InputManager2D::OnMouseFocus()
{
}

void InputManager2D::OnMouseBlur()
{
}

void InputManager2D::OnMouseMove(int mX, int mY, int relX, int relY, bool Left,bool Right,bool Middle)
{
	CEGUI::System::getSingleton().injectMousePosition(static_cast<float>(mX), static_cast<float>(mY));
}

void InputManager2D::OnMouseWheel(bool Up, bool Down)
{
	if(Up)
	{
		CEGUI::System::getSingleton().injectMouseWheelChange(+1);
	}
	else if(Down)
	{
		CEGUI::System::getSingleton().injectMouseWheelChange(-1);
	}
}

void InputManager2D::OnLButtonDown(int mX, int mY)
{
	CEGUI::System::getSingleton().injectMouseButtonDown(CEGUI::LeftButton);
}

void InputManager2D::OnLButtonUp(int mX, int mY)
{
	CEGUI::System::getSingleton().injectMouseButtonUp(CEGUI::LeftButton);
}

void InputManager2D::OnRButtonDown(int mX, int mY)
{
	CEGUI::System::getSingleton().injectMouseButtonDown(CEGUI::RightButton);
}

void InputManager2D::OnRButtonUp(int mX, int mY)
{
	CEGUI::System::getSingleton().injectMouseButtonUp(CEGUI::RightButton);
}

void InputManager2D::OnMButtonDown(int mX, int mY)
{
	CEGUI::System::getSingleton().injectMouseButtonDown(CEGUI::MiddleButton);
}

void InputManager2D::OnMButtonUp(int mX, int mY)
{
	CEGUI::System::getSingleton().injectMouseButtonUp(CEGUI::MiddleButton);
}

void InputManager2D::OnJoyAxis(Uint8 which,Uint8 axis,Sint16 value)
{
}

void InputManager2D::OnJoyButtonDown(Uint8 which,Uint8 button)
{
}

void InputManager2D::OnJoyButtonUp(Uint8 which,Uint8 button)
{
}

void InputManager2D::OnJoyHat(Uint8 which,Uint8 hat,Uint8 value)
{
}

void InputManager2D::OnJoyBall(Uint8 which,Uint8 ball,Sint16 xrel,Sint16 yrel)
{
}

void InputManager2D::OnMinimize()
{
}

void InputManager2D::OnRestore()
{
}

void InputManager2D::OnResize(int w,int h)
{
}

void InputManager2D::OnExpose()
{
}

void InputManager2D::OnUser(Uint8 type, int code, void* data1, void* data2)
{
}

void InputManager2D::Update( real /*i_fFrametime*/, real /*i_fTimestep*/ )
{
	SDL_Event pEvent;
	while(SDL_PollEvent(&pEvent)) {
		OnEvent(&pEvent);
	}
}

bool InputManager2D::IsKeyDown( const char* i_szKey )
{
	MGDVector<int32>::iterator it = InputManagerFactory::GetInputManagerPtr()->m_KeyButtonDown.begin();
	while(it != InputManagerFactory::GetInputManagerPtr()->m_KeyButtonDown.end())
	{
		const char* a = SDL_GetKeyName(*it);
		if(strcmp(a, i_szKey) == 0)
		{
			return true;
		}
		++it;
	}	
	return false;
}