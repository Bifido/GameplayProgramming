#include "FSMManager.h"
#include "FSM.h"
#include "../Manager/LuaManager.h"
#include "../LOG/LogManager.h"

void FSMManager::CreateInstance()
{
	if(GetSingletonPtr() == NULL)
		new FSMManager();
}

FSMManager::FSMManager()
{
	RegisterLuaFunctions();
}

FSMManager::~FSMManager()
{
	//TODO -> Studente MGD
	MGDMap<FSMID, FSM*>::iterator it = m_vFSMMap.begin();
	for (; it != m_vFSMMap.end(); ++it)
	{
		delete it->second;
	}

	m_vFSMMap.clear();
}

void FSMManager::RegisterLuaFunctions()
{
	LuaManager::GetSingleton().GetGlobalVars().RegisterDirect("StartFSM", &FSMManager::StartFSM);
}

void FSMManager::StartFSM(const char* i_szFSMName)
{
	//TODO -> Studente MGD
	MGDMap<FSMID, FSM*>::iterator it = FSMManager::GetSingleton().m_vFSMMap.find(FSMID(i_szFSMName));
	if (it != FSMManager::GetSingleton().m_vFSMMap.end())
	{
		(it->second)->Start();
	}
}

void FSMManager::AddFSM( FSM* i_pFSM )
{
	//TODO -> Studente MGD
	MGDMap<FSMID, FSM*>::iterator it = m_vFSMMap.find(i_pFSM->GetName());
	if (it == m_vFSMMap.end())
	{
		m_vFSMMap[i_pFSM->GetName()] = i_pFSM;
	}
}

void FSMManager::RemoveFSM( const ObjectId& i_oName )
{
	//TODO -> Studente MGD
	MGDMap<FSMID, FSM*>::iterator it = m_vFSMMap.find(i_oName);
	if (it != m_vFSMMap.end())
	{
		m_vFSMMap.erase(it);
	}
}

void FSMManager::Update(real i_fFrametime, real i_fTimestep)
{
	//TODO -> Studente MGD
	MGDMap<FSMID, FSM*>::iterator it = m_vFSMMap.begin();
	for (; it != m_vFSMMap.end(); ++it)
	{
		(it->second)->Update(i_fFrametime, i_fTimestep);
	}
}