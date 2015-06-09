#pragma once
#include "../Core/Singleton.h"
#include "../Common/DataTypes.h"
#include "IInputManager.h"
#include "SDL_keycode.h"

namespace Ogre
{
	class RenderWindow;
}

class InputManager2D : public IInputManager
{
public:
	static IInputManager*  CreateInstance(Ogre::RenderWindow* i_pWindow);

	virtual		 ~InputManager2D();

	virtual void OnEvent(void* i_pEvent);
	virtual void Update(real i_fFrametime, real i_fTimestep);

private:
	InputManager2D();

	
	void OnInputFocus();
	void OnInputBlur();
	void OnKeyDown(SDL_Keycode sym, Uint16 mod, Uint16 unicode);
	void OnKeyUp(SDL_Keycode sym, Uint16 mod, Uint16 unicode);
	void OnMouseFocus();
	void OnMouseBlur();
	void OnMouseMove(int mX, int mY, int relX, int relY, bool Left,bool Right,bool Middle);
	void OnMouseWheel(bool Up, bool Down);    //Not implemented
	void OnLButtonDown(int mX, int mY);
	void OnLButtonUp(int mX, int mY);
	void OnRButtonDown(int mX, int mY);
	void OnRButtonUp(int mX, int mY);
	void OnMButtonDown(int mX, int mY);
	void OnMButtonUp(int mX, int mY);
	void OnJoyAxis(Uint8 which, Uint8 axis, Sint16 value);
	void OnJoyButtonDown(Uint8 which, Uint8 button);
	void OnJoyButtonUp(Uint8 which, Uint8 button);
	void OnJoyHat(Uint8 which, Uint8 hat, Uint8 value);
	void OnJoyBall(Uint8 which, Uint8 ball, Sint16 xrel, Sint16 yrel);
	void OnMinimize();
	void OnRestore();
	void OnResize(int w,int h);
	void OnExpose();
	void OnUser(Uint8 type, int code, void* data1, void* data2);
	static bool IsKeyDown( const char* i_szKey );
};

