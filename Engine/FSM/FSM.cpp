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
}

void FSM::AddState( State* i_pState )
{
	//TODO -> Studente MGD
}

void FSM::DoTransition(const char* i_szTransition)
{
	//TODO -> Studente MGD
}

void FSM::Update(real /*i_fFrametime*/, real /*i_fTimestep*/)
{
	//TODO -> Studente MGD
}

void FSM::Start()
{
	//TODO -> Studente MGD
}
