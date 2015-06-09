/*
*	This class contains the logic of the state. We have a list of transition and the functions OnEnter, OnExit and Upsate
*/

#pragma once
#include "../Common/DataTypes.h"
#include "Transition.h"

class State
{
private:
public:
	ID_DECLARATION;

	State(const std::string& i_szName);

	~State();

	void OnEnter();
	void OnUpdate();
	void OnExit();

	const StateID& DoTransition(const char* i_szTransition);

	void AddTransition(Transition* i_pTransition);
	void SetScriptOnEnter(const char* i_szFilename);
	void SetScriptOnUpdate(const char* i_szFilename);
	void SetScriptOnExit(const char* i_szFilename);

	const StateID& GetName()
	{
		return m_oId;
	}

private:
	MGDMap<TransitionID,Transition*>	m_TransitionMap;
	std::string							m_szName;
	StateID								m_oId;	

	std::string							m_szScriptOnEnter;
	std::string							m_szScriptOnUpdate;
	std::string							m_szScriptOnExit;
};

