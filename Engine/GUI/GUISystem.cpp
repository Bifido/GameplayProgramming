#include "GUISystem.h"
#include "OgreTexture.h"
#include "OgreRenderTexture.h"
#include "OgreHardwarePixelBuffer.h"
#include "..\GUI\GUIViewComponent.h"
#include "../Manager\SystemManager.h"
#include "../LOG/LOGManager.h"
#include "Widgets/Interface/IGUIWidget.h"
#include "Widgets/Button.h"
#include "Widgets/StaticText.h"
#include "SDL_events.h"
#include "../Common/Common.h"

ID_IMPLEMENTATION(GUISystem);

GUISystem::GUISystem()
	:System()
	, m_pOgreGuiRenderer(NULL)
	, m_pOpenGLGuiRenderer(NULL)
	, m_pGUIWindowManager(NULL)
	, m_pGuiWindow(NULL)
{
	GUIViewComponent::RegisterScriptFunctions();

}

GUISystem::~GUISystem()
{
	MGD_LOG::LOGManager::GetSingleton().WriteLog(MGD_LOG::MGD_INFO, SYSTEMS_CONTEXT, "GUISystem destroyed!");
}

void GUISystem::Init()
{	
	m_oAddGuiComponentEvent.Subscribe(this,&GUISystem::AddGUIComponent,ObjectId("Registered/GUIViewComponent"));
	m_oDeleteGuiComponentEvent.Subscribe(this,&GUISystem::DeleteGUIComponent,ObjectId("Unregistered/GUIViewComponent"));

	CreateGUIWindow();

	SDL_ShowCursor(SDL_DISABLE);

	MGD_LOG::LOGManager::GetSingleton().WriteLog(MGD_LOG::MGD_INFO, SYSTEMS_CONTEXT, "GUISystem initialized!");
}

void GUISystem::LoadResources()
{
	// initialise the required dirs for the DefaultResourceProvider
	CEGUI::DefaultResourceProvider* rp = static_cast<CEGUI::DefaultResourceProvider*>(CEGUI::System::getSingleton().getResourceProvider());
	rp->setResourceGroupDirectory("schemes", "./Data/Resources/UI/schemes/");
	rp->setResourceGroupDirectory("imagesets", "./Data/Resources/UI/imagesets/");
	rp->setResourceGroupDirectory("fonts", "./Data/Resources/UI/fonts/");
	rp->setResourceGroupDirectory("layouts", "./Data/Resources/UI/layouts/");
	rp->setResourceGroupDirectory("looknfeels", "./Data/Resources/UI/looknfeel/");
	rp->setResourceGroupDirectory("lua_scripts", "./Data/Resources/UI/lua_scripts/");

	rp->setResourceGroupDirectory("schemas", "./Data/Resources/UI/xml_schemas/");

	//Initialize resources groups
	CEGUI::Imageset::setDefaultResourceGroup("imagesets");
	CEGUI::Font::setDefaultResourceGroup("fonts");
	CEGUI::Scheme::setDefaultResourceGroup("schemes");
	CEGUI::WidgetLookManager::setDefaultResourceGroup("looknfeels");
	CEGUI::WindowManager::setDefaultResourceGroup("layouts");

	CEGUI::XMLParser* parser = CEGUI::System::getSingleton().getXMLParser();
	if (parser->isPropertyPresent("SchemaDefaultResourceGroup"))
		parser->setProperty("SchemaDefaultResourceGroup", "schemas");
}

void GUISystem::AddGUIComponent()
{
	// All the actors that contains a GUIViewComponent
	const EntityComponentTable* actorThatContainsGUIComp = SystemManager::GetSingleton().GetComponentTable(GUIViewComponent::ID);

	// For each actor
	EntityComponentTable::const_iterator actualActor = actorThatContainsGUIComp->begin();
	for (; actualActor != actorThatContainsGUIComp->end(); ++actorThatContainsGUIComp){
		
		GUIViewComponent* actualComponent = static_cast<GUIViewComponent*>(SystemManager::GetSingleton().EditSingleComponent(GUIViewComponent::ID, actualActor->first));

		if (actualComponent != nullptr && actualComponent->IsInit()){
			MGDVector<IGUIWidgets*> widgets = actualComponent->GetGUIWidgets();

			MGDVector<IGUIWidgets*>::const_iterator actualWidgets = widgets.begin();
			for (; actualWidgets != widgets.end(); ++actualWidgets){
				m_pGuiWindow->addChildWindow(GetGUIWidget(*actualWidgets));
			}
		}
	}

	//TODO -> Studente MGD
}

void GUISystem::CreateGUIWindow()
{
	try
	{
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

		//Create RENDERER
		if(is3D)
		{
			m_pOgreGuiRenderer = &CEGUI::OgreRenderer::bootstrapSystem();
		}
		else
		{
			m_pOpenGLGuiRenderer = &CEGUI::OpenGLRenderer::bootstrapSystem();

			m_pOpenGLGuiRenderer->getDefaultRenderingRoot().clearGeometry(CEGUI::RQ_OVERLAY);
			m_pOpenGLGuiRenderer->enableExtraStateSettings(true); // ??
			LoadResources();

		}


	//Setup CEGUI System
	CEGUI::SchemeManager::getSingleton().create("TaharezLook.scheme");

	CEGUI::System::getSingleton().setDefaultMouseCursor("TaharezLook", "MouseArrow");

	//Create window
	m_pGUIWindowManager = &CEGUI::WindowManager::getSingleton();
	m_pGuiWindow = m_pGUIWindowManager->createWindow("DefaultWindow", "MGD_14_GUI/Window");
	m_pGuiWindow->setText("MGD_14 - Game");

	CEGUI::System::getSingleton().setGUISheet(m_pGuiWindow);
	}
	catch (CEGUI::Exception& e)
	{
		MGD_LOG::LOGManager::GetSingleton().WriteLog(MGD_LOG::MGD_ERROR, GUI_CONTEXT, e.getMessage().c_str());
	}
}

void GUISystem::Update( real /*i_fFrametime*/, real /*i_fTimestep*/ )
{
}

void GUISystem::DeleteGUIComponent(const char* i_szName)
{
	//TODO -> Studente MGD

	//Blocco di codice che elimina una window di CEGUI
	/*
		CEGUI::Window* pObj = m_pGuiWindow->getChild(NOME_WIDGET);
		MGD_ASSERT(pObj);
		if (pObj)
		{
			m_pGuiWindow->removeChildWindow(pObj->getID());
			m_pGUIWindowManager->destroyWindow(pObj);
		}
	*/
}

CEGUI::Window* GUISystem::GetGUIWidget(IGUIWidgets* i_oGUIWidget)
{
	MGD_ASSERT(i_oGUIWidget);
	if (i_oGUIWidget)
	{
		try
		{
			CEGUI::Window* pObject = m_pGUIWindowManager->createWindow(i_oGUIWidget->GetStyle(), i_oGUIWidget->GetName());
			MGD_ASSERT(pObject);
			if (pObject)
			{
				std::string& szType(i_oGUIWidget->GetType());

				if (strcmp(szType.c_str(), "Button") == 0)
				{
					return Button::Create(i_oGUIWidget, pObject);
				}
				else if (strcmp(szType.c_str(), "StaticText") == 0)
				{
					return StaticText::Create(i_oGUIWidget, pObject);
				}
				else
				{
					MGD_ASSERT(0);
					MGD_LOG::LOGManager::GetSingleton().WriteLog(MGD_LOG::MGD_ERROR, GUI_CONTEXT, "GUIWidget type '%s' doesn't exist", szType.c_str());
				}
			}
		}
		catch (CEGUI::Exception& e)
		{
			MGD_ASSERT(0);
			MGD_LOG::LOGManager::GetSingleton().WriteLog(MGD_LOG::MGD_ERROR, GUI_CONTEXT, e.getMessage().c_str());
		}

	}

	return NULL;
}