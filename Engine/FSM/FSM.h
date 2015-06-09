/*
*	This is the FSM Finite state machine, we manage transitions between states
*/
#pragma once
#include "../Common/DataTypes.h"
#include "../Event/RegistryEventHandler.h"

class State;
class Transition;

class FSM
{
public:
	FSM(const std::string& i_szName);
	~FSM();

	void Update(real i_fFrametime, real i_fTimestep);

	void AddState(State* i_pState);
	void SetInitialState(const StateID& i_oInitialState)
	{
		m_oInitialState = i_oInitialState;
		m_pCurrentState = m_States[i_oInitialState];
	}

	const FSMID& GetName()
	{
		return m_oId;
	}
	
	void RegisterTransitions(const MGDMap<TransitionID,Transition*>& m_TransitionMap);
	void DoTransition(const char* i_szTransition);

	void Start();

private:
	MGDMap<StateID, State*>	m_States;
	StateID					m_oInitialState;
	State*					m_pCurrentState;

	RegistryEventHandler<FSM, const char*>	m_TransitionEvent;


	std::string				m_szName;
	FSMID					m_oId;
	bool					m_bIsStarted;
};

