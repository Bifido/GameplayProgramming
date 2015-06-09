/*
*	This component contains the properties of the camera in game
*
*	Lua Functions:
*	
*	void SetEnable(bool)
*	bool IsEnable()
*	void SetFixedAxis(bool)
*	
*	
*		
*/

#pragma once
#include "../Core/Component.h"


class CameraComponent : public Component
{
public:
	ID_DECLARATION;

	CameraComponent(std::string i_szOwnerID, bool i_bIsCreatedFromTemplate = false);
	virtual ~CameraComponent();

	virtual void Init();
	virtual bool SetupFromXml(const tinyxml2::XMLElement* pNode);
	virtual void CreateFromTemplate(Component* i_pComponent, const ObjectId& i_oOwner)
	{
		if (i_pComponent)
		{
			CameraComponent* pCameraComponent = static_cast<CameraComponent*>(i_pComponent);
			if (pCameraComponent)
			{
				m_bIsFixedAxis = pCameraComponent->IsFixedAxis();
				m_uiNearClipDistance = pCameraComponent->GetNearClipDistance();
				m_bIsEnable = pCameraComponent->IsEnable();
				m_fAspectRatio = pCameraComponent->GetAspectRatio();
				m_bIsOrthographic = pCameraComponent->IsOrthographic();
				Component::CreateFromTemplate(i_oOwner);
			}
		}
	}

	void SetAspectRatio( real i_fWidth, real i_fHeigth)
	{
		m_fAspectRatio = (i_fWidth / i_fHeigth);
	}

	real GetAspectRatio() const
	{
		return m_fAspectRatio;
	}

	void SetOrthographic(bool i_bIsOrthographic)
	{
		m_bIsOrthographic = i_bIsOrthographic;
	}

	bool IsOrthographic() const
	{
		return m_bIsOrthographic;
	}

	void SetEnable(bool i_bEnable)
	{
		m_bIsEnable = i_bEnable;
	}

	bool IsEnable() const
	{
		return m_bIsEnable;
	}

	void SetFixedAxis(const bool& i_bIsFixedAxis)
	{
		m_bIsFixedAxis = i_bIsFixedAxis;
	}
	bool IsFixedAxis() const
	{
		return m_bIsFixedAxis;
	}
	real GetNearClipDistance() const
	{
		return m_uiNearClipDistance;
	}
	void SetNearClipDistance(real i_uiNearClipDistance)
	{
		m_uiNearClipDistance = i_uiNearClipDistance;
	}

	static void RegisterScriptFunction();
private:
	/* LUA FUNCTIONS */	
	void CreateLuaObject();
	void SetFixedAxisFromScript(LuaPlus::LuaObject i_oObject);
	/* ************ */
	bool m_bIsFixedAxis;
	real m_uiNearClipDistance;
	bool m_bIsEnable;
	real m_fAspectRatio;
	bool m_bIsOrthographic;

	static const std::string CAMERA_COMPONENT_METATABLE;
};

