#include "Renderer2D.h"
#include "../LOG/LOGManager.h"
#include "../Common/Utilities.h"
#include "SDL.h"
#include "tinyxml2.h"
#include "../Common/Common.h"
#include "CEGUISystem.h"

	IRenderer* Renderer2D::CreateInstance()
	{		
		return new Renderer2D();
	}

	Renderer2D::Renderer2D()
	{
		SDL_Init(SDL_INIT_VIDEO);
		
		SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

		tinyxml2::XMLDocument oDocument;
		tinyxml2::XMLError oErr = oDocument.LoadFile(CONFIG.c_str());
		if(oErr != tinyxml2::XML_NO_ERROR)
		{
			MGD_LOG::LOGManager::GetSingleton().WriteLog(MGD_LOG::MGD_ERROR, PHYSICS_CONTEXT, "Error loading file: %s", CONFIG.c_str());
			return;
		}
		const tinyxml2::XMLElement* pRoot = oDocument.RootElement();
		if(pRoot)
		{
			const tinyxml2::XMLElement* pFullscreen = pRoot->FirstChildElement("Fullscreen");
			if(pFullscreen)
			{
				m_bFullscreen = pFullscreen->BoolAttribute("enable");
			}

			const tinyxml2::XMLElement* pScreenSize = pRoot->FirstChildElement("ScreenSize");
			if(pScreenSize)
			{
				m_fPosX = pScreenSize->FloatAttribute("x");
				m_fPosY = pScreenSize->FloatAttribute("y");
				m_fWidth = pScreenSize->FloatAttribute("width");
				m_fHeight = pScreenSize->FloatAttribute("height");
			}

			const tinyxml2::XMLElement* pColorBkg = pRoot->FirstChildElement("ColorBkg");
			if(pColorBkg)
			{
				m_fClearColor[0] = pColorBkg->FloatAttribute("r");
				m_fClearColor[1] = pColorBkg->FloatAttribute("g");
				m_fClearColor[2] = pColorBkg->FloatAttribute("b");
				m_fClearColor[3] = pColorBkg->FloatAttribute("a");
			}

			const tinyxml2::XMLElement* pViewport = pRoot->FirstChildElement("Viewport");
			if(pViewport)
			{
				m_fOrtho[0] = pViewport->FloatAttribute("left");
				m_fOrtho[1] = pViewport->FloatAttribute("right");
				m_fOrtho[2] = pViewport->FloatAttribute("bottom");
				m_fOrtho[3] = pViewport->FloatAttribute("top");
				m_fOrtho[4] = pViewport->FloatAttribute("near");
				m_fOrtho[5] = pViewport->FloatAttribute("far");
			}
		}

		Uint32 uiFlags = SDL_WINDOW_OPENGL;
		if(m_bFullscreen)
		{
			uiFlags |= SDL_WINDOW_FULLSCREEN;
		}

		m_pWindow = SDL_CreateWindow("SDL Test", static_cast<int>(m_fPosX), static_cast<int>(m_fPosY), static_cast<int>(m_fWidth), static_cast<int>(m_fHeight), uiFlags);
		if (m_pWindow == nullptr){
			std::cout << SDL_GetError() << std::endl;
			return;
		}
		else
		{
			m_pRenderer = SDL_CreateRenderer( m_pWindow, -1, SDL_RENDERER_ACCELERATED );
			if( m_pRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
			}else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( m_pRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//SDL_SetRenderDrawBlendMode(m_pRenderer, SDL_BLENDMODE_BLEND);
			}
		}

		m_oGLContext = SDL_GL_CreateContext(m_pWindow);
		
		glClearDepth(1.0f);
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);

		glClearColor( m_fClearColor[0], m_fClearColor[1], m_fClearColor[2], m_fClearColor[3] );

		glOrtho(m_fOrtho[0], m_fOrtho[1], m_fOrtho[2], m_fOrtho[3], m_fOrtho[4], m_fOrtho[5]);
	}

	Renderer2D::~Renderer2D()
	{

	}

	void Renderer2D::ConfigureSceneManager()
	{
	}
	void Renderer2D::Update(real /*i_fFrametime*/, real /*i_fTimestep*/)
	{
		CEGUI::System::getSingleton().renderGUI();

		SDL_GL_SwapWindow(m_pWindow);
	}

	void Renderer2D::PreDraw( real i_fFrametime, real i_fTimestep )
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
