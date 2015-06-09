/*
*	This is the manager for the FSMs, we add, remove and start FSMs from this class
*/

#pragma once
#include "../Common/DataTypes.h"
#include "../Core/Singleton.h"

class FSM;

class FSMManager : public Singleton<FSMManager>
{
public:
	static void CreateInstance();

	~FSMManager();

	void AddFSM(FSM* i_pFSM);
	void RemoveFSM(const ObjectId& i_oName);

	void Update(real i_fFrametime, real i_fTimestep);

	static void StartFSM(const char* i_szFSMName);

private:
	FSMManager();
	
	void RegisterLuaFunctions();

	typedef ObjectId FSMID;
	MGDMap<FSMID, FSM*>	m_vFSMMap;
};

