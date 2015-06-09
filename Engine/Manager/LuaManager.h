#pragma once
#include "Interface/IScriptManager.h"
#include "../Core/Singleton.h"
#include "../Common/DataTypes.h"
#include "LuaPlus.h"

/*
	This class creates the Lua state and exposes a few useful methods to managed it.
*/

class LuaManager: public Singleton<LuaManager>, public IScriptManager
{
public:
	static void CreateInstance();
	~LuaManager();

	virtual void Init();

	/*
		It's used to run a lua file
	*/
	virtual void ExecuteFile(const char* i_pFilename);

	/*
		It's used to run a coscript directly
	*/
	virtual void ExecuteString(const char* i_pString);

	/*
		Get the global evironment variables
	*/
	LuaPlus::LuaObject GetGlobalVars(void);

	/*
		Get the LuaState
	*/
	LuaPlus::LuaState* GetLuaState(void) const;
	

private:
	/*
		Encript the error and prints a string that explain which is the mistake
	*/
	void PrintError(int errorNum);

	/*
		Clear the LuaState
	*/
	void ClearStack(void);

	static void Print(const char* i_szString);
	static void PrintReal(real i_fval);

	void RegisterGlobalLuaFunction();

	LuaManager();	

	LuaPlus::LuaState* m_pLuaState;
};