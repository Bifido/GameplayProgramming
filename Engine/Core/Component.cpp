#include "Component.h"
#include "../Manager/LuaManager.h"
#include "../LOG/LogManager.h"
#include "../Core/Actor.h"

ID_IMPLEMENTATION(Component);

int8 Component::DEFAULT_COMPONENT_COUNT = 0;

Component::Component(std::string i_szOwnerID, bool i_bIsCreatedFromTemplate)
	: m_bInit(false)
	, m_bIsRemovable(false)
	, m_bIsCreatedFromTemplate(i_bIsCreatedFromTemplate)
	, m_oOwnerId(i_szOwnerID.c_str())
	, m_bIsEnable(true)
{
	m_oLuaObject.AssignNil(LuaManager::GetSingleton().GetLuaState());
}

Component::~Component() 
{
#if defined(_DEBUG)
	--NumComponents;
#endif
}

void Component::CreateLuaObject()
{
	m_oLuaObject.AssignNil(LuaManager::GetSingleton().GetLuaState());
}


bool Component::SetupFromXml( const tinyxml2::XMLElement* pNode )
{
	if(pNode)
	{
		const char* pTextName = pNode->Attribute("Name");		
		if (pTextName && strlen(pTextName) > 0)
		{
			m_szTextName = pTextName;
			m_oName = ObjectId(pTextName);
			return true;
		}
		else
		{
			CreateName();
		}

		return true;
	}

	return false;
}

void Component::CreateName()
{
	char szTextName[15];
	sprintf_s(szTextName, "Component_%d", DEFAULT_COMPONENT_COUNT);
	++DEFAULT_COMPONENT_COUNT;
	m_szTextName = szTextName;
	m_oName = ObjectId(szTextName);
}