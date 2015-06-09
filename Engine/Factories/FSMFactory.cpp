#include "FSMFactory.h"
#include "tinyxml2.h"
#include "../Manager\LuaManager.h"
#include "../FSM\State.h"
#include "../FSM\FSMManager.h"
#include "../FSM\FSM.h"
#include "../LOG/LOGManager.h"

void FSMFactory::CreateInstance()
{
	if(GetSingletonPtr() == NULL)
		new FSMFactory();
}

FSMFactory::FSMFactory()
{
	RegisterGlobalLuaFunction();
	RegisterComponents();
}

FSMFactory::~FSMFactory()
{
}

void FSMFactory::RegisterGlobalLuaFunction()
{
	LuaManager::GetSingleton().GetGlobalVars().RegisterDirect("LoadFSMFile", &FSMFactory::LoadFile);
	LuaManager::GetSingleton().GetGlobalVars().RegisterDirect("RemoveFSMFile", &FSMFactory::RemoveFile);
}


void FSMFactory::RegisterComponents()
{
	//m_Factory.Register<State>(State::ID);
}

void FSMFactory::LoadFile(const char* i_szFilename)
{
	if(!i_szFilename)
	{
		MGD_LOG::LOGManager::GetSingleton().WriteLog(MGD_LOG::MGD_ERROR, FSM_FACTORY_CONTEXT, "ERROR BUILDING FSM");
		return;
	}

	tinyxml2::XMLDocument oDocument;
	tinyxml2::XMLError oErr = oDocument.LoadFile(i_szFilename);
	if(oErr != tinyxml2::XML_NO_ERROR)
	{
		MGD_LOG::LOGManager::GetSingleton().WriteLog(MGD_LOG::MGD_ERROR, FSM_FACTORY_CONTEXT, "ERROR DOC");
		return;
	}
	const tinyxml2::XMLElement* pRoot = oDocument.RootElement();
	if(pRoot)
	{

		const char* pName = pRoot->Attribute("Name");

		FSM* pFSM = new FSM(std::string(pName));
		if(pFSM)
		{
			for (const tinyxml2::XMLElement* pState = pRoot->FirstChildElement("State"); pState; pState = pState->NextSiblingElement("State"))
			{
				State* pNewState = CreateState(pState);			
				if(pState)
				{
					pFSM->AddState(pNewState);
				}
			}

			const tinyxml2::XMLElement* pInitialState = pRoot->FirstChildElement("InitialState");
			if(pInitialState)
			{
				const char* pName = pInitialState->Attribute("Name");
				if(strlen(pName) > 0)
				{
					pFSM->SetInitialState(ObjectId(pName));
				}
			}

			FSMManager::GetSingleton().AddFSM(pFSM);
			return;
		}
	}

	MGD_LOG::LOGManager::GetSingleton().WriteLog(MGD_LOG::MGD_ERROR, FSM_FACTORY_CONTEXT, "Create FSM error!");

	return;
}

State* FSMFactory::CreateState(const tinyxml2::XMLElement* i_pState)
{
	if(i_pState)
	{
		const char* pName = i_pState->Attribute("Name");

		State* pNewState = new State(std::string(pName));
		if(pNewState)
		{
			for (const tinyxml2::XMLElement* pNode = i_pState->FirstChildElement("Transition"); pNode; pNode = pNode->NextSiblingElement("Transition"))
			{
				Transition* pTransition = CreateTransition(pNode);
				if (pTransition)
				{
					pNewState->AddTransition(pTransition);
				}
			}

			const tinyxml2::XMLElement* pScriptOnEnterNode = i_pState->FirstChildElement("ScriptOnEnter");
			if(pScriptOnEnterNode)
			{
				pNewState->SetScriptOnEnter(pScriptOnEnterNode->Attribute("Filename"));
			}

			const tinyxml2::XMLElement* pScriptOnUpdateNode = i_pState->FirstChildElement("ScriptOnUpdate");
			if(pScriptOnUpdateNode)
			{
				pNewState->SetScriptOnUpdate(pScriptOnUpdateNode->Attribute("Filename"));
			}

			const tinyxml2::XMLElement* pScriptOnExitNode = i_pState->FirstChildElement("ScriptOnExit");
			if(pScriptOnExitNode)
			{
				pNewState->SetScriptOnExit(pScriptOnExitNode->Attribute("Filename"));
			}

		}
		return pNewState;
	}

	return NULL;
}

Transition* FSMFactory::CreateTransition(const tinyxml2::XMLElement* i_pNode)
{
	if(i_pNode)
	{
		const char* pName = i_pNode->Attribute("Name");
		const char* pStateTarget = i_pNode->Attribute("Target");

		if(strlen(pName) > 0 && strlen(pStateTarget) > 0)
		{
			Transition* pTransition = new Transition(std::string(pName), ObjectId(pStateTarget));
			if(pTransition)
			{
				return pTransition;
			}
		}
	}

	MGD_LOG::LOGManager::GetSingleton().WriteLog(MGD_LOG::MGD_ERROR, FSM_FACTORY_CONTEXT, "Create Transition error!");
	return NULL;
}

void FSMFactory::RemoveFile(const char* i_szFilename)
{
	if(!i_szFilename)
	{
		MGD_LOG::LOGManager::GetSingleton().WriteLog(MGD_LOG::MGD_ERROR, FSM_FACTORY_CONTEXT, "ERROR BUILDING FSM");
		return;
	}

	tinyxml2::XMLDocument oDocument;
	tinyxml2::XMLError oErr = oDocument.LoadFile(i_szFilename);
	if(oErr != tinyxml2::XML_NO_ERROR)
	{
		MGD_LOG::LOGManager::GetSingleton().WriteLog(MGD_LOG::MGD_ERROR, FSM_FACTORY_CONTEXT, "ERROR DOC");
		return;
	}
	const tinyxml2::XMLElement* pRoot = oDocument.RootElement();
	if(pRoot)
	{
		const char* pName = pRoot->Attribute("Name");
		FSMManager::GetSingleton().RemoveFSM(ObjectId(pName));
	}
}
