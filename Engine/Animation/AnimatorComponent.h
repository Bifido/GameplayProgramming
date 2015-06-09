#pragma once
#include "../Core/Component.h"
#include "../Common/DataTypes.h"
#include "..\Common\DataTypes.h"
#include "Motion.h"

class Motion;

class AnimatorComponent : public Component
{
public:
	ID_DECLARATION;
	AnimatorComponent(std::string i_szOwnerID, bool i_bIsCreatedFromTemplate = false);
	virtual ~AnimatorComponent();

	typedef MGDMap<ObjectId, MGDVector<Motion*>>			AnimatorComponentStateMap;
	typedef AnimatorComponentStateMap::iterator				AnimatorComponentStateMapIt;
	typedef AnimatorComponentStateMap::const_iterator		AnimatorComponentStateMapCIt;

	virtual void Init();
	virtual bool SetupFromXml(const tinyxml2::XMLElement* pNode);

	virtual void CreateFromTemplate(Component* i_pComponent, const ObjectId& i_oOwner)
	{
		if (i_pComponent)
		{
			AnimatorComponent* pAnimatorComponent = static_cast<AnimatorComponent*>(i_pComponent);
			if (pAnimatorComponent)
			{
				Component::CreateFromTemplate(i_oOwner);

				m_bDoTransition = true;
				m_oCurrentState = pAnimatorComponent->GetCurrentState();
				pAnimatorComponent->CopyStates(m_States);
				m_iLayerIndex = pAnimatorComponent->GetLayerIndex();
			}
		}
	}
	void TransitionComplete()
	{
		m_bDoTransition = false;
	}
	bool DoingTransition() const
	{
		return m_bDoTransition;
	}

	MGDVector<Motion*>& GetMotions()
	{
		AnimatorComponentStateMapIt it = m_States.find(m_oCurrentState);
		if(it != m_States.end())
		{
			return it->second;
		}

		return MGDVector<Motion*>();
	}

	const AnimatorComponentStateMap& GetStates() const
	{
		return m_States;
	}

	void CopyStates(AnimatorComponentStateMap& o_oStates)
	{
		o_oStates = m_States;

		AnimatorComponentStateMapIt it = o_oStates.begin();
		AnimatorComponentStateMapCIt itOriginal = m_States.begin();
		for(; it != o_oStates.end(); ++it, ++itOriginal)
		{
			for(Uint32 uiIndex = 0; uiIndex < (*it).second.size(); ++uiIndex)
			{

				(*it).second[uiIndex] = itOriginal->second[uiIndex]->CreateFromTemplate();
				//(*it).second[uiIndex]->CreateFromTemplate(itOriginal->second[uiIndex]);
			}
		}
	}

	const ObjectId& GetCurrentState() const
	{
		return m_oCurrentState;
	}

	int32 GetLayerIndex() const
	{
		return m_iLayerIndex;
	}

	static void RegisterScriptFunction();
private:
	void SetState(const char* i_pState);
	void CreateLuaObject();

	AnimatorComponentStateMap m_States;
	bool		m_bDoTransition;
	ObjectId	m_oCurrentState;
	int32 m_iLayerIndex;

	static const char* METATABLE_NAME;
};

