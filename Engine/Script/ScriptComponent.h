#pragma once
#include "../Core/Component.h"
#include "../Physics/PhysicsComponent.h"

class ScriptComponent : public Component
{
public:
	ID_DECLARATION;
	enum ScriptState
	{
		UNINITILIZE = 0,
		RUNNING,
		SUCCEED,
		FAIL,

		PAUSE,
		NONE
	};

	ScriptComponent(std::string i_szOwnerID, bool i_bIsCreatedFromTemplate = false);
	virtual ~ScriptComponent();

	virtual void Init();
	virtual bool SetupFromXml( const tinyxml2::XMLElement* pNode );
	virtual void CreateFromTemplate(Component* i_pComponent, const ObjectId& i_oOwner)
	{
		if (i_pComponent)
		{
			ScriptComponent* pScriptComponent = static_cast<ScriptComponent*>(i_pComponent);
			if (pScriptComponent)
			{
				m_szFileName = pScriptComponent->GetFilename();
				Component::CreateFromTemplate(i_oOwner);
			}
		}
	}


	static void RegisterScriptFuncitons();

	void Succeed();
	void Fail();
	void Pause();
	void UnPause();
	bool IsRunning();
	bool IsPaused();
	bool IsEnded();

	ScriptState	GetState();

	void AttachScriptProcess();

	void OnEnter();
	void OnUpdate(real /*i_fFrametime*/, real i_fTimestep);
	void OnExit();

	void OnCollision( const PhysicsComponent::CollisionData& i_oCollisionData );

	const std::string& GetFilename() const;

private:	
	bool BuildCppDataFromScript(LuaPlus::LuaObject i_oConstructionData);

	LuaPlus::LuaObject GetLuaObjectForScript()
	{
		return m_oLuaObject;
	}

	void SetupFromScript( LuaPlus::LuaObject i_oConstructionData);
	void CreateLuaObject();
	
	std::string m_szFileName;

	real				m_fFrequency;
	real				m_fTime;
	ScriptState			m_eState;
	ScriptState			m_ePreviousState;
	//LuaPlus::LuaObject	m_oScriptOnEnterFunction;
	//LuaPlus::LuaObject	m_oScriptOnUpdateFunction;
	//LuaPlus::LuaObject	m_oScriptOnExitFunction;
	//LuaPlus::LuaObject	m_oScriptOnCollisionFunction;

	static const std::string SCRIPT_COMPONENT_TABLE;
};

inline const std::string& ScriptComponent::GetFilename() const
{
	return m_szFileName;
}

inline void ScriptComponent::Succeed()
{
	m_eState = SUCCEED;
}

inline void ScriptComponent::Fail()
{
	m_eState = FAIL;
}

inline void ScriptComponent::Pause()
{
	m_ePreviousState = m_eState;
	m_eState = PAUSE;
}

inline void ScriptComponent::UnPause()
{
	MGD_ASSERT(m_ePreviousState != NONE);
	m_eState = m_ePreviousState;
	m_ePreviousState = NONE;
}

inline bool ScriptComponent::IsRunning()
{
	return m_eState == RUNNING;
}

inline bool ScriptComponent::IsPaused()
{
	return m_eState == PAUSE;
}

inline bool ScriptComponent::IsEnded()
{
	return m_eState == SUCCEED || m_eState == FAIL;
}

inline ScriptComponent::ScriptState ScriptComponent::GetState()
{
	return m_eState;
}