#pragma once
#include "../Core/System.h"
#include "../Common/Utilities.h"
#include "CEGUI.h"
#include "RendererModules/Ogre/CEGUIOgreRenderer.h"
#include "RendererModules/OpenGL/CEGUIOpenGLRenderer.h"
#include "../Event/RegistryEventHandler.h"
#include "CEGUIWindow.h"
#include "Widgets/Interface/IGUIWidget.h"


/*
*	This class deals with the rendering of the scene using the info in the gfx components
*/
class GUISystem : public System
{
public:
	ID_DECLARATION;

	GUISystem();
	virtual ~GUISystem();

	virtual void	Init();
	virtual void	Update(real i_fFrametime, real i_fTimestep);

	void AddGUIComponent();
	void DeleteGUIComponent(const char* i_szName);

private:
	void CreateGUIWindow();
	void LoadResources();
	CEGUI::Window* GetGUIWidget(IGUIWidgets* i_oGUIWidget);
	CEGUI::OgreRenderer*	m_pOgreGuiRenderer;
	CEGUI::OpenGLRenderer*	m_pOpenGLGuiRenderer;
	CEGUI::WindowManager*	m_pGUIWindowManager;
	CEGUI::Window*			m_pGuiWindow;


	RegistryEventHandler<GUISystem> m_oAddGuiComponentEvent;
	RegistryEventHandler<GUISystem, const char*> m_oDeleteGuiComponentEvent;

};

