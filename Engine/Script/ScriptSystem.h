#pragma once
#include "../Core/System.h"
#include "../Common/Utilities.h"
#include "LuaPlus.h"
#include "LuaPlusCD.h"

/*
*	This class manages the script components
*/

class ScriptSystem : public System
{
public:
	ID_DECLARATION;

	ScriptSystem();
	virtual ~ScriptSystem();

	virtual void	Init();
	virtual void	Update(real i_fFrametime, real i_fTimestep);

	static LuaPlus::LuaObject GetLuaObject( const char* i_pEntityName, const char* i_pComponentId );
	static LuaPlus::LuaObject GetLuaObjectByName( const char* i_pEntityName, const char* i_pComponentId, const char* i_pComponentName );

private:
	void RegisterCreationObject();
	void RegisterScriptFunction();
	void AttachScriptComponent(LuaPlus::LuaObject i_oScriptProcess);
};
