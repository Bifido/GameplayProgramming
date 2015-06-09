#pragma once
#include "Renderer3D.h"
#include "Renderer2D.h"

class IRenderer;

class RendererFactory
{
public:
	virtual ~RendererFactory()
	{
	}

	static void CreateRenderer(bool i_bIs3D)
	{
		if(i_bIs3D)
		{
			m_pRenderer = Renderer3D::CreateInstance();
		}
		else
		{
			m_pRenderer = Renderer2D::CreateInstance();
		}
	}

	static IRenderer* GetRendererPtr()
	{
		return m_pRenderer;
	}

private:
	static IRenderer* m_pRenderer;
};
