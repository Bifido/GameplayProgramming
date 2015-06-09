#include "LuaManager.h"
#include "../LOG/LogManager.h"

void LuaManager::CreateInstance()
{
	if(GetSingletonPtr() == NULL)
		new LuaManager();
}

LuaManager::LuaManager()
{
	Init();
}

LuaManager::~LuaManager()
{
	if (m_pLuaState)
	{
		LuaPlus::LuaState::Destroy(m_pLuaState);
		m_pLuaState = NULL;
	}
}

void LuaManager::Init()
{
	//Create LuaState
	m_pLuaState = LuaPlus::LuaState::Create(true);
	if (m_pLuaState == NULL)
	{
		MGD_LOG::LOGManager::GetSingleton().WriteLog(MGD_LOG::MGD_ERROR, MANAGER_CONTEXT, "Error creating LuaState!");
		return;
	}

	RegisterGlobalLuaFunction();
}

void LuaManager::RegisterGlobalLuaFunction()
{
	GetGlobalVars().RegisterDirect("Print", &LuaManager::Print);
	GetGlobalVars().RegisterDirect("PrintReal", &LuaManager::PrintReal);
}

void LuaManager::ExecuteFile(const char* i_pFilename)
{
	int result = m_pLuaState->DoFile(i_pFilename);
	if (result != 0)
	{
		PrintError(result);
		
		MGD_ASSERT(0 && "Script error!");
	}
}

void LuaManager::ExecuteString( const char* i_pString )
{
	if (strlen(i_pString) > 0)
	{
		int result = m_pLuaState->DoString(i_pString);
		if (result != 0)
		{
			PrintError(result);
		}
	}
}

LuaPlus::LuaObject LuaManager::GetGlobalVars(void)
{
	return m_pLuaState->GetGlobals();
}

LuaPlus::LuaState* LuaManager::GetLuaState(void) const
{
	return m_pLuaState;
}

void LuaManager::PrintError(int errorNum)
{
	LuaPlus::LuaStackObject stackObj(m_pLuaState,-1);
	const char* errStr = stackObj.GetString();
	if (errStr)
	{
		ClearStack();
	}
	else
	{
		errStr = "Unknown Lua parse error";
	}

	MGD_LOG::LOGManager::GetSingleton().WriteLog(MGD_LOG::MGD_ERROR, MANAGER_CONTEXT, "%s", errStr);
}

void LuaManager::ClearStack(void)
{
	m_pLuaState->SetTop(0);
}

void LuaManager::Print(const char* i_szString)
{
	printf(i_szString);
	MGD_LOG::LOGManager::GetSingleton().WriteLog(MGD_LOG::MGD_INFO, MANAGER_CONTEXT, "LuaPrint: %s", i_szString);
}

void LuaManager::PrintReal(real i_fval)
{
	MGD_LOG::LOGManager::GetSingleton().WriteLog(MGD_LOG::MGD_INFO, MANAGER_CONTEXT, "LuaPrint: %2.2f", i_fval);
}
