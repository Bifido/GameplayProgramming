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
}

void FSMManager::RegisterLuaFunctions()
{
	LuaManager::GetSingleton().GetGlobalVars().RegisterDirect("StartFSM", &FSMManager::StartFSM);
}

void FSMManager::StartFSM(const char* i_szFSMName)
{
	//TODO -> Studente MGD
}

void FSMManager::AddFSM( FSM* i_pFSM )
{
	//TODO -> Studente MGD
}

void FSMManager::RemoveFSM( const ObjectId& i_oName )
{
	//TODO -> Studente MGD
}

void FSMManager::Update(real i_fFrametime, real i_fTimestep)
{
	//TODO -> Studente MGD
}