#pragma once
#include "../Common/Utilities.h"
#include "SDL_surface.h"
#include "../Common/TextureManager.h"

namespace tinyxml2
{
	class XMLElement;
};

class Motion
{
public:
	ID_DECLARATION;

	Motion() {}
	virtual ~Motion() {}

	virtual void Init()
	{

	}

	virtual bool SetupFromXml(const tinyxml2::XMLElement* pNode);
	virtual Motion* CreateFromTemplate() = 0;
	void CopyFromTemplate(const Motion* i_pMotion)
	{
		m_fDuration = i_pMotion->m_fDuration;
		m_bLoop = i_pMotion->m_bLoop;
	}

	virtual void Reset()
	{

	}

	real m_fDuration;
	bool	m_bLoop;

};


class MotionSpriteAnimation : public Motion
{
public:
	ID_DECLARATION;
	MotionSpriteAnimation()
		:Motion()
		, m_uiNumImages(1)
		, m_uiCurrentImage(0)
		, m_fCurrentTime(0.0f)
		, m_bIsFlipped(false)
	{
	}

	virtual void Init()
	{
		TextureManager::Inst()->SDL_GL_LoadTexture( m_szPath, ObjectId(m_szPath).GetId());
		TextureManager::Inst()->GetInfo(m_szPath, TotalDimesion[0], TotalDimesion[1]);

		SingleImageDimesion[1] = TotalDimesion[1];

		SingleImageDimesion[0] = TotalDimesion[0] / m_uiNumImages;

	}

	virtual ~MotionSpriteAnimation() 
	{
	}

	virtual bool SetupFromXml(const tinyxml2::XMLElement* pNode);
	virtual Motion* CreateFromTemplate()
	{
		MotionSpriteAnimation* pMotion = new MotionSpriteAnimation();
		if(pMotion)
		{
			pMotion->m_uiNumImages = m_uiNumImages;
			pMotion->m_uiCurrentImage = m_uiCurrentImage;
			pMotion->TotalDimesion[0] = TotalDimesion[0];
			pMotion->TotalDimesion[1] = TotalDimesion[1];

			pMotion->SingleImageDimesion[0] = SingleImageDimesion[0];
			pMotion->SingleImageDimesion[1] = SingleImageDimesion[1];

			pMotion->m_Size[0] = m_Size[0];
			pMotion->m_Size[1] = m_Size[1];

			pMotion->m_fCurrentTime = m_fCurrentTime;
			for(uint32 uiIndex = 0; uiIndex < MAX_CHAR_FOR_PATH; ++uiIndex)
			{
				pMotion->m_szPath[uiIndex] = m_szPath[uiIndex];
			}
			pMotion->m_bIsFlipped = IsFlipped();

			pMotion->CopyFromTemplate(this);
		}
		

		return pMotion;
	}

	virtual void Reset()
	{
		m_uiCurrentImage = 0;
		m_fCurrentTime = 0.0f;
	}

	bool	IsFlipped() const
	{
		return m_bIsFlipped;
	}

	static const uint32 MAX_CHAR_FOR_PATH = 128;

	Uint32		m_uiNumImages;
	Uint32		m_uiCurrentImage;
	real		TotalDimesion[2];
	real		SingleImageDimesion[2];
	real		m_Size[2];
	real		m_fCurrentTime;
	char		m_szPath[MAX_CHAR_FOR_PATH];
	bool		m_bIsFlipped;
};

class MotionInterpolationPosition : public Motion
{
public:
	ID_DECLARATION;
	MotionInterpolationPosition()
		: Motion()
	{

	}
	virtual ~MotionInterpolationPosition() {}

	virtual bool SetupFromXml(const tinyxml2::XMLElement* pNode);
	virtual Motion* CreateFromTemplate()
	{
		MotionInterpolationPosition* pMotion = new MotionInterpolationPosition();
		if(pMotion)
		{
			pMotion->m_oStartPosition = GetStartPosition();
			pMotion->m_oTargetPosition = GetTargetPosition();
		}

		Motion::CopyFromTemplate(pMotion);

		return pMotion;
	}

	const Vec2& GetTargetPosition() const
	{
		return m_oTargetPosition;
	}

	const Vec2& GetStartPosition() const
	{
		return m_oStartPosition;
	}

private:
	Vec2 m_oStartPosition;
	Vec2 m_oTargetPosition;
};

class MotionInterpolationRotation : public Motion
{
public:
	ID_DECLARATION;
	MotionInterpolationRotation()
		: Motion()
	{

	}
	virtual ~MotionInterpolationRotation() {}

	virtual bool SetupFromXml(const tinyxml2::XMLElement* pNode);
	virtual Motion* CreateFromTemplate()
	{
		MotionInterpolationRotation* pMotion = new MotionInterpolationRotation();
		if(pMotion)
		{
			pMotion->m_fStartRotation = GetStartRotation();
			pMotion->m_fTargetRotation = GetTargetRotation();
		}

		Motion::CopyFromTemplate(pMotion);

		return pMotion;
	}


	real GetTargetRotation() const
	{
		return m_fTargetRotation;
	}
	
	real GetStartRotation() const
	{
		return m_fStartRotation;
	}

	real m_fStartRotation;
	real m_fTargetRotation;
};

class MotionInterpolationScale : public Motion
{
public:
	ID_DECLARATION;
	MotionInterpolationScale()
		: Motion()
	{

	}
	virtual ~MotionInterpolationScale() {}

	virtual bool SetupFromXml(const tinyxml2::XMLElement* pNode);
	virtual Motion* CreateFromTemplate()
	{
		MotionInterpolationScale* pMotion = new MotionInterpolationScale();
		if(pMotion)
		{
			pMotion->m_oStartScale = GetStartScale();
			pMotion->m_oTargetScale = GetTargetScale();
		}
		Motion::CopyFromTemplate(pMotion);

		return pMotion;
	}

	const Vec2& GetTargetScale() const
	{
		return m_oTargetScale;
	}

	const Vec2& GetStartScale() const
	{
		return m_oStartScale;
	}

	Vec2 m_oTargetScale;
	Vec2 m_oStartScale;
};

class MotionAlphaBlend : public Motion
{
public:
	ID_DECLARATION;
	MotionAlphaBlend()
		: Motion()
	{

	}
	virtual ~MotionAlphaBlend() {}

	virtual bool SetupFromXml(const tinyxml2::XMLElement* pNode);
	virtual Motion* CreateFromTemplate()
	{
		MotionAlphaBlend* pMotion = new MotionAlphaBlend();
		if(pMotion)
		{
			pMotion->m_fTargetAlpha = GetTargetAlpha();
		}
		Motion::CopyFromTemplate(pMotion);

		return pMotion;
	}

	real GetTargetAlpha() const
	{
		return m_fTargetAlpha;
	}

	real m_fTargetAlpha;
};


