#include "Motion.h"
#include "tinyxml2.h"

ID_IMPLEMENTATION(Motion);
ID_IMPLEMENTATION(MotionInterpolationPosition);
ID_IMPLEMENTATION(MotionInterpolationRotation);
ID_IMPLEMENTATION(MotionInterpolationScale);
ID_IMPLEMENTATION(MotionSpriteAnimation);
ID_IMPLEMENTATION(MotionAlphaBlend);

bool Motion::SetupFromXml( const tinyxml2::XMLElement* pNode )
{
	if(pNode)
	{
		m_fDuration = pNode->FloatAttribute("duration");
		m_bLoop = pNode->BoolAttribute("loop");

		return true;
	}

	return false;
}

bool MotionSpriteAnimation::SetupFromXml( const tinyxml2::XMLElement* pNode )
{
	if(pNode)
	{
		const tinyxml2::XMLElement* pImage = pNode->FirstChildElement("Image");
		if (pImage)
		{
			sprintf(m_szPath, pImage->Attribute("filename"));
		}

		const tinyxml2::XMLElement* pNumImage = pNode->FirstChildElement("NumImages");
		if (pNumImage)
		{
			m_uiNumImages = pNumImage->IntAttribute("value");
		}

		const tinyxml2::XMLElement* pFlip = pNode->FirstChildElement("Flip");
		if (pFlip)
		{
			m_bIsFlipped = pFlip->BoolAttribute("value");
		}

		const tinyxml2::XMLElement* pSize = pNode->FirstChildElement("Size");
		if (pSize)
		{
			m_Size[0] = pSize->FloatAttribute("width");
			m_Size[1] = pSize->FloatAttribute("height");
		}
	}
	return Motion::SetupFromXml(pNode);
}

bool MotionInterpolationPosition::SetupFromXml( const tinyxml2::XMLElement* pNode )
{
	if(pNode)
	{
		const tinyxml2::XMLElement* pTargetPosition = pNode->FirstChildElement("TargetPosition");
		if(pTargetPosition)
		{
			m_oTargetPosition.x = pTargetPosition->FloatAttribute("x");
			m_oTargetPosition.y = pTargetPosition->FloatAttribute("y");
		}

		const tinyxml2::XMLElement* pStartPosition = pNode->FirstChildElement("StartPosition");
		if(pStartPosition)
		{
			m_oStartPosition.x = pStartPosition->FloatAttribute("x");
			m_oStartPosition.y = pStartPosition->FloatAttribute("y");
		}
	}

	return Motion::SetupFromXml(pNode);
}

bool MotionInterpolationRotation::SetupFromXml( const tinyxml2::XMLElement* pNode )
{
	if(pNode)
	{
		const tinyxml2::XMLElement* pTargetRotation = pNode->FirstChildElement("TargetRotation");
		if(pTargetRotation)
		{
			m_fTargetRotation = pTargetRotation->FloatAttribute("degree");
		}
		const tinyxml2::XMLElement* pStartRotation = pNode->FirstChildElement("StartRotation");
		if(pStartRotation)
		{
			m_fStartRotation = pStartRotation->FloatAttribute("degree");
		}

	}

	return Motion::SetupFromXml(pNode);
}

bool MotionInterpolationScale::SetupFromXml( const tinyxml2::XMLElement* pNode )
{
	if(pNode)
	{
		const tinyxml2::XMLElement* pTargetScale = pNode->FirstChildElement("TargetScale");
		if(pTargetScale)
		{
			m_oTargetScale.x = pTargetScale->FloatAttribute("x");
			m_oTargetScale.y = pTargetScale->FloatAttribute("y");
		}
		const tinyxml2::XMLElement* pStartScale = pNode->FirstChildElement("StartScale");
		if(pStartScale)
		{
			m_oStartScale.x = pStartScale->FloatAttribute("x");
			m_oStartScale.y = pStartScale->FloatAttribute("y");
		}
	}

	return Motion::SetupFromXml(pNode);
}

bool MotionAlphaBlend::SetupFromXml( const tinyxml2::XMLElement* pNode )
{
	if(pNode)
	{
		const tinyxml2::XMLElement* pTargetScale = pNode->FirstChildElement("TargetAlpha");
		if(pTargetScale)
		{
			m_fTargetAlpha = pTargetScale->FloatAttribute("value");
		}
	}

	return Motion::SetupFromXml(pNode);
}
