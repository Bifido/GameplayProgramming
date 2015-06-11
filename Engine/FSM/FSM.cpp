#include "FSM.h"
#include "State.h"
#include "../LOG/LogManager.h"


FSM::FSM(const std::string& i_szName)
	: m_szName(i_szName)
	, m_oId(i_szName.c_str())
	, m_pCurrentState(NULL)
	, m_bIsStarted(FALSE)
{
	m_TransitionEvent.Subscribe(this, &FSM::DoTransition, ObjectId("DoTransition"));
}

FSM::~FSM()
{
	//TODO -> Studente MGD
	m_bIsStarted = FALSE;
	MGDMap<StateID, State*>::iterator it = m_States.begin();
	for (; it != m_States.end(); ++it)
	{
		delete it->second;
	}
	m_States.clear();
}

void FSM::AddState( State* i_pState )
{
	//TODO -> Studente MGD
	m_States[i_pState->GetName()] = i_pState;
}

void FSM::DoTransition(const char* i_szTransition)
{
	if (m_pCurrentState) 
	{
		const StateID& oStateTarget(m_pCurrentState->DoTransition(i_szTransition));
		MGDMap<StateID, State*>::iterator it = m_States.find(oStateTarget);
		if (it != m_States.end()) 
		{
			State* pNewState = (*it).second;
			if (pNewState) 
			{
				m_pCurrentState->OnExit();
				m_pCurrentState = pNewState;
				m_pCurrentState->OnEnter();
			}
		}
	}
	else 
	{
		//MGDMap<StateID, State*>::iterator it = m_States.find(m_oInitialState);
		//if (it != m_States.end()) 
		//{
		//	m_pCurrentState = (*it).second;
		//	if (m_pCurrentState) 
		//	{
		//		m_pCurrentState->OnEnter();
		//	}
		//}
		Start();
	}
}

void FSM::Update(real /*i_fFrametime*/, real /*i_fTimestep*/)
{
	//TODO -> Studente MGD
	if (m_bIsStarted)
	{
		m_pCurrentState->OnUpdate();
	}
}

void FSM::Start()
{
	//TODO -> Studente MGD
	if (!m_bIsStarted)
	{
		MGDMap<StateID, State*>::iterator it = m_States.find(m_oInitialState);
		if (it != m_States.end())
		{
			m_pCurrentState = (*it).second;
			if (m_pCurrentState)
			{
				m_pCurrentState->OnEnter();
				m_bIsStarted = TRUE;
			}
		}
	}
}
