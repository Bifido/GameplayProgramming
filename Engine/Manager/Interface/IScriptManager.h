#pragma once

/*
	This is the interface for the script managers
*/

class IScriptManager
{
public:
	virtual ~IScriptManager() {}
	virtual void Init() = 0;
	virtual void ExecuteFile(const char* i_pFilename) = 0;
	virtual void ExecuteString(const char* i_pString) = 0;
};