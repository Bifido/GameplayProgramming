//#include <windows.h>
#include <signal.h>
#include "Manager\SystemManager.h"
#include "Manager\LuaManager.h"
#include "Event/EventRegistry.h"
#include "Factories\ActorFactory.h"
#include "FSM\FSMManager.h"
#include "Factories\FSMFactory.h"
#include "Common\Common.h"
#include "Resources\ResourceManager.h"
#include "Graphics\RenderingSystem.h"
#include "Physics\PhysicsWorld.h"
#include "Physics\PhysicsSystem.h"
#include "GUI\GUISystem.h"
#include "Script\ScriptSystem.h"
#include "LOG\LOGManager.h"
#include "glog\logging.h"
#include "Graphics\RendererFactory.h"
#include "Input\InputManagerFactory.h"
#include "Animation\AnimatorSystem.h"
#include "Physics\Box2DWorld.h"
#include "Common\TextureManager.h"
#include "Audio\AudioSystem.h"

RenderingSystem* pRenderingSystem = NULL;
PhysicsSystem* pPhysicsSystem = NULL;
GUISystem* pGuiSystem = NULL;
ScriptSystem* pScriptSystem = NULL;
AnimatorSystem* pAnimatorSystem = NULL;
AudioSystem* pAudioSystem = NULL;

void InitGlog()
{
	// Initialize Google's logging library. See http://google-glog.googlecode.com/svn/trunk/doc/glog.html
	google::SetLogDestination(google::GLOG_INFO,	"./logs/info.log");
	google::SetLogDestination(google::GLOG_WARNING, "./logs/warning.log");
	google::SetLogDestination(google::GLOG_FATAL,	"./logs/fatal.log");
	google::SetStderrLogging(google::GLOG_INFO);
	FLAGS_v= 2;
	google::InitGoogleLogging("");
}

void InitInstance()
{
	MGD_LOG::LOGManager::CreateInstance();

	bool is3D = false;

	tinyxml2::XMLDocument oDocument;
	tinyxml2::XMLError oErr = oDocument.LoadFile(CONFIG.c_str());
	if(oErr != tinyxml2::XML_NO_ERROR)
	{
		MGD_LOG::LOGManager::GetSingleton().WriteLog(MGD_LOG::MGD_ERROR, MAIN_CONTEXT, "Error loading file: %s", CONFIG.c_str());
		return;
	}
	const tinyxml2::XMLElement* pRoot = oDocument.RootElement();
	if(pRoot)
	{
		const tinyxml2::XMLElement* p3D = pRoot->FirstChildElement("Is3D");
		if(p3D)
		{
			is3D = p3D->BoolAttribute("enable");
		}
	}

	SystemManager::CreateInstance(10,10);
	LuaManager::CreateInstance();

	RendererFactory::CreateRenderer(is3D);
	InputManagerFactory::CreateInputManager(is3D, RendererFactory::GetRendererPtr()->GetWindow());

	ResourceManager::CreateInstance(is3D);
	EventRegistry::CreateInstance();
	ActorFactory::CreateInstance();
	FSMManager::CreateInstance();
	FSMFactory::CreateInstance();

	RegisterCommonScriptFunctions();

	pRenderingSystem = new RenderingSystem(RendererFactory::GetRendererPtr()->GetWindow(), RendererFactory::GetRendererPtr()->GetScene());

	PhysicsWorldFactory* fact = NULL;
	if(is3D)
	{
		fact = new BulletWorldFactory();
	}
	else
	{
		fact = new Box2DWorldFactory();
	}
	pPhysicsSystem = new PhysicsSystem(fact);
	delete fact;

	pGuiSystem = new GUISystem();
	pScriptSystem = new ScriptSystem();

	pAnimatorSystem = new AnimatorSystem();

	pAudioSystem = new AudioSystem();

	SystemManager::GetSingleton().AddSystem(pRenderingSystem);
	SystemManager::GetSingleton().AddSystem(pPhysicsSystem);
	SystemManager::GetSingleton().AddSystem(pScriptSystem);
	SystemManager::GetSingleton().AddSystem(pAnimatorSystem);
	SystemManager::GetSingleton().AddSystem(pGuiSystem);
	SystemManager::GetSingleton().AddSystem(pAudioSystem);

	LuaManager::GetSingleton().ExecuteFile("Data/Scripts/StartGame.lua");
}

void AbortCallback(int signum)
{
	MGD_LOG::LOGManager::GetSingleton().WriteLog(MGD_LOG::MGD_FATAL, APPLICATION_CONTEXT, "Abort has been called! Maybe a script is bad formatted!");
}

int main(int argc, char** argv)
{
	signal(SIGABRT, AbortCallback);
	
	InitGlog();

	InitInstance();

	MSG msg;
	ZeroMemory( &msg, sizeof( msg ) );

	static const real TIMESTEP= 1.f/60.f;
	LARGE_INTEGER frequency;			// ticks per second
	LARGE_INTEGER currentTime, newTime; // ticks
	real frametime= 0.f;

	// get ticks per second
	QueryPerformanceFrequency(&frequency);
	real freq= 1.f/frequency.QuadPart;
	QueryPerformanceCounter(&currentTime);
	real dt = 1/60.0f;
	PeekMessage( &msg, NULL, 0, 0, PM_REMOVE);
	while (msg.message!=WM_QUIT && !InputManagerFactory::GetInputManagerPtr()->IsShutdown()) 
	{
		if (PeekMessage( &msg, NULL, 0, 0, PM_REMOVE)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		{
			QueryPerformanceCounter(&newTime);
			frametime += (newTime.QuadPart - currentTime.QuadPart) * freq; //sec
			//LOG_IF(WARNING, frametime > 0.2) << "fps " << 1.f/frametime;
			currentTime= newTime;
			if(frametime >= dt)
			{
				FSMManager::GetSingleton().Update(dt, TIMESTEP);
				RendererFactory::GetRendererPtr()->PreDraw(dt, TIMESTEP);
				SystemManager::GetSingleton().Update(dt, TIMESTEP);
				RendererFactory::GetRendererPtr()->Update(dt, TIMESTEP);
				InputManagerFactory::GetInputManagerPtr()->Update(dt,TIMESTEP);
				frametime -= dt;
			}
		}
	}

	delete(SystemManager::GetSingletonPtr());
	delete(FSMFactory::GetSingletonPtr());
	delete(FSMManager::GetSingletonPtr());
	delete(ActorFactory::GetSingletonPtr());
	delete(InputManagerFactory::GetInputManagerPtr());
	delete(RendererFactory::GetRendererPtr());
	delete(LuaManager::GetSingletonPtr());
	delete(EventRegistry::GetSingletonPtr());
	delete(TextureManager::Inst());
	delete(ResourceManager::GetSingletonPtr());
	delete(MGD_LOG::LOGManager::GetSingletonPtr());
	google::ShutdownGoogleLogging();

#if defined(_DEBUG)
	MGD_ASSERT(NumActors == 0);
	MGD_ASSERT(NumComponents == 0);
	printf("NUM ACTORS: %d", NumActors);
	printf("NUM COMPONENTS: %d", NumComponents);
#endif

	return 1;
}

