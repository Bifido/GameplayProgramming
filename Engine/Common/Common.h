/*
*	This class contains all the commmons functions used in this engine
*/

#pragma once
#if defined(_DEBUG)
#include "vld.h"
#endif
#include "../Manager/LuaManager.h"
#include "../Physics//DebugDrawer/PhysicsDebugDrawer.h"
#include "../OtherComponents/TransformationComponent.h"
#include "../Physics/PhysicsComponent.h"
#include "../Graphics/BaseGfxComponent.h"
#include "../Graphics/MeshGfxComponent.h"
#include "../Graphics/CameraComponent.h"
#include "../Graphics/LightComponent.h"
#include "../Physics\BulletWorld.h"
#include "../Input/InputManager3D.h"
#include "LuaObject.h"
#include "../Graphics/SpriteComponent.h"
#include "../Animation/AnimatorComponent.h"
#include "../Input/InputManagerFactory.h"
#include "../Input/IInputManager.h"
#include "../GUI/GUIViewComponent.h"

static std::string CONFIG("Data/Config.xml");

// Set enable/disable to the physics debug drawer
static void EnableDebugDrawer(LuaPlus::LuaObject i_oLuaObject)
{
	if(i_oLuaObject.IsBoolean())
	{
		BulletWorld::EnableDebugDrawer(i_oLuaObject.GetBoolean());
	}
}

static void OnExitApp()
{
	if(InputManagerFactory::GetInputManagerPtr())
		InputManagerFactory::GetInputManagerPtr()->OnExit();
}

//Common function to register the scripts functions for each classes tha we want to expose to script
static void RegisterCommonScriptFunctions()
{
	LuaPlus::LuaObject pGlobalVars = LuaManager::GetSingleton().GetGlobalVars();
	pGlobalVars.RegisterDirect("ExitApp", &OnExitApp);
	pGlobalVars.RegisterDirect("EnableDebugDrawer", &EnableDebugDrawer);

	TransformationComponent::RegisterScriptFunction();
	PhysicsComponent::RegisterScriptFunction();
	MeshGfxComponent::RegisterScriptFunction();
	BaseGfxComponent::RegisterScriptFunction();
	CameraComponent::RegisterScriptFunction();
	LightComponent::RegisterScriptFunction();
	SpriteComponent::RegisterScriptFunction();
	AnimatorComponent::RegisterScriptFunction();
	//Added
	GUIViewComponent::RegisterScriptFunctions();
}