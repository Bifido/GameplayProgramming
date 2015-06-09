#include "ScriptComponent.h"
#include "../Manager/LuaManager.h"
#include "LuaPlus.h"

const std::string ScriptComponent::SCRIPT_COMPONENT_TABLE("ScriptComponent");

ID_IMPLEMENTATION(ScriptComponent);

ScriptComponent::ScriptComponent(std::string i_szOwnerID, bool i_bIsCreatedFromTemplate)
	: Component(i_szOwnerID, i_bIsCreatedFromTemplate)
	, m_eState(UNINITILIZE)
{
	m_fFrequency = 0.0f;
	m_fTime = 0.0f;

	CreateLuaObject();
}

ScriptComponent::~ScriptComponent()
{
	OnExit();
}

void ScriptComponent::Init()
{
	LuaManager::GetSingleton().ExecuteFile(m_szFileName.c_str());
}

void ScriptComponent::CreateLuaObject()
{
	m_oLuaObject.AssignNewTable(LuaManager::GetSingleton().GetLuaState());

	LuaPlus::LuaObject metaTable =	LuaManager::GetSingleton().GetGlobalVars().GetByName(SCRIPT_COMPONENT_TABLE.c_str());

	m_oLuaObject.SetLightUserData("__object", const_cast<ScriptComponent*>(this));
	m_oLuaObject.SetMetaTable(metaTable);
}

void ScriptComponent::OnEnter()
{
	if(m_oLuaObject.GetByName("OnEnter").IsFunction())
	{
		LuaPlus::LuaFunction<void> oOnEnterFunction(m_oLuaObject.GetByName("OnEnter"));
		oOnEnterFunction(m_oLuaObject);
	}

	m_eState = RUNNING;
}

void ScriptComponent::OnUpdate(real /*i_fFrametime*/, real i_fTimestep)
{
	m_fTime += i_fTimestep;
	if (m_fTime >= m_fFrequency)
	{
		if(m_oLuaObject.GetByName("OnUpdate").IsFunction())
		{
			try
			{
				LuaPlus::LuaFunction<void> oOnUpdateFunc(m_oLuaObject.GetByName("OnUpdate"));
				//m_fTime is passed to script in milliseconds
				oOnUpdateFunc(m_oLuaObject, static_cast<int32>(m_fTime*1000.0f));
				m_fTime = 0.0f;
			}
			catch (LuaPlus::LuaException& /*e*/)
			{
			}
		}
	}
}

void ScriptComponent::OnExit()
{
	if(m_oLuaObject.GetByName("OnExit").IsFunction())
	{
		LuaPlus::LuaFunction<void> oOnExitFunction(m_oLuaObject.GetByName("OnExit"));
		oOnExitFunction(m_oLuaObject);
	}
	m_eState = NONE;
}

void ScriptComponent::OnCollision(const PhysicsComponent::CollisionData& i_oCollisionData)
{
	if(m_oLuaObject.GetByName("OnCollision").IsFunction())
	{
		LuaPlus::LuaFunction<void> oOnCollisionFunction(m_oLuaObject.GetByName("OnCollision"));
		oOnCollisionFunction(m_oLuaObject, const_cast<PhysicsComponent::CollisionData&>(i_oCollisionData).GetLuaCollisionObj());
	}
}

bool ScriptComponent::SetupFromXml( const tinyxml2::XMLElement* pNode )
{
	if(pNode)
	{
		const tinyxml2::XMLElement* pFile = pNode->FirstChildElement("File");
		if (pFile)
		{
			m_szFileName = pFile->Attribute("value");
		}
	}

	return Component::SetupFromXml(pNode);
}

void ScriptComponent::RegisterScriptFuncitons()
{
	//Create new metatable for a generic script component
	LuaPlus::LuaObject oScriptComponentMetatable = LuaManager::GetSingleton().GetGlobalVars().CreateTable(SCRIPT_COMPONENT_TABLE.c_str());

	//if we will not find the object inside the instance we try to search inside the base metatable
	oScriptComponentMetatable.SetObject("__index", oScriptComponentMetatable); 

	//We set as key base the metatable of the base script component
	oScriptComponentMetatable.SetObject("base", oScriptComponentMetatable);

	//We register the functions proper for each instance
	oScriptComponentMetatable.RegisterObjectDirect("Succeed",    (ScriptComponent*)0, &ScriptComponent::Succeed);
	oScriptComponentMetatable.RegisterObjectDirect("Fail",       (ScriptComponent*)0, &ScriptComponent::Fail);
	oScriptComponentMetatable.RegisterObjectDirect("Pause", (ScriptComponent*)0, &ScriptComponent::Pause);
	oScriptComponentMetatable.RegisterObjectDirect("UnPause",(ScriptComponent*)0, &ScriptComponent::UnPause);
	oScriptComponentMetatable.RegisterObjectDirect("IsRunning", (ScriptComponent*)0, &ScriptComponent::IsRunning);
	oScriptComponentMetatable.RegisterObjectDirect("IsEnded", (ScriptComponent*)0, &ScriptComponent::IsEnded);
	oScriptComponentMetatable.RegisterObjectDirect("IsPaused", (ScriptComponent*)0, &ScriptComponent::IsPaused);
	//oScriptComponentMetatable.RegisterObjectDirect("AttachChild",(ScriptComponent*)0, &ScriptComponent::AttachChild);

	//Expose the c++ function to create a object from script
	oScriptComponentMetatable.RegisterObjectDirect("Setup",  (ScriptComponent*)0, &ScriptComponent::SetupFromScript);
	oScriptComponentMetatable.RegisterObjectDirect("GetLuaObject",  (ScriptComponent*)0, &ScriptComponent::GetLuaObjectForScript);
}

//Assign script function to cpp function
bool ScriptComponent::BuildCppDataFromScript( LuaPlus::LuaObject i_oConstructionData )
{
	//Fill member variable with contrsuction data from script
	if (i_oConstructionData.IsTable())
	{
		for (LuaPlus::LuaTableIterator oConstructionDataIt = i_oConstructionData;
			oConstructionDataIt; oConstructionDataIt.Next())
		{
			const char* szKey = oConstructionDataIt.GetKey().GetString();
			LuaPlus::LuaObject oVal = oConstructionDataIt.GetValue();

			if (strcmp(szKey, "Frequency") == 0 && oVal.IsInteger())
			{
				m_fFrequency = static_cast<real>(oVal.GetInteger());
			}
			else
			{
				m_oLuaObject.SetObject(szKey, oVal);
			}
		}
	}

	return true;
}

void ScriptComponent::SetupFromScript( LuaPlus::LuaObject i_oConstructionData)
{
	//Assign script function to cpp function
	if (!BuildCppDataFromScript(i_oConstructionData))
	{
		//Delete object
		m_oLuaObject.AssignNil(LuaManager::GetSingleton().GetLuaState());
	}
}