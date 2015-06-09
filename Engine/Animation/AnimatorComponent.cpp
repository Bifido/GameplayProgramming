#include "AnimatorComponent.h"
#include "Motion.h"
#include "..\Common\MGDAssert.h"
#include "..\Event\RegistryEventPublisher.h"
#include "..\Manager\LuaManager.h"

ID_IMPLEMENTATION(AnimatorComponent);


const char* AnimatorComponent::METATABLE_NAME = "AnimatorComponentMetatable";

AnimatorComponent::AnimatorComponent(std::string i_szOwnerID, bool i_bIsCreatedFromTemplate)
	: Component(i_szOwnerID, i_bIsCreatedFromTemplate)
	, m_oCurrentState("Init")
	, m_bDoTransition(true)
	, m_iLayerIndex(0)
{
	CreateLuaObject();
}

AnimatorComponent::~AnimatorComponent()
{
	char pEventName[128];
	sprintf_s(pEventName, "Test/Unregistered/%s", GetID().GetDebugName().c_str());
	RegistryEventPublisher<const char*> m_oEventPublisher;
	m_oEventPublisher.SetPath(ObjectId(pEventName));
	m_oEventPublisher.Raise(GetTextName().c_str());


	AnimatorComponentStateMapIt it = m_States.begin();
	for(; it != m_States.end(); ++it)
	{
		MGDVector<Motion*>::iterator motionIt = (*it).second.begin();
		while(motionIt != (*it).second.end())
		{
			delete((*motionIt));
			motionIt = (*it).second.erase(motionIt);
		}
		(*it).second.clear();
	}

	m_States.clear();
}

void AnimatorComponent::Init()
{
	AnimatorComponentStateMapIt it = m_States.begin();
	for(; it != m_States.end(); ++it)
	{
		for(uint32 uiIndex = 0; uiIndex < (*it).second.size(); ++uiIndex)
		{
			((*it).second.at(uiIndex))->Init();
		}
	}
}

bool AnimatorComponent::SetupFromXml( const tinyxml2::XMLElement* pNode )
{
	if(pNode)
	{
		const tinyxml2::XMLElement* pInitState = pNode->FirstChildElement("InitState");
		if(pInitState)
		{
			char szInitState[32];
			sprintf_s(szInitState,pInitState->Attribute("name"));
			m_oCurrentState = ObjectId(szInitState);
		}


		const tinyxml2::XMLElement* pLayerNum = pNode->FirstChildElement("LayerIndex");
		if (pLayerNum)
		{
			m_iLayerIndex = pLayerNum->IntAttribute("value");
		}

		for (const tinyxml2::XMLElement* pState = pNode->FirstChildElement("State"); pState; pState = pState->NextSiblingElement("State"))
		{
			char szName[64];
			sprintf_s(szName, pState->Attribute("name"));
			m_States[ObjectId(szName)] = MGDVector<Motion*>();

			for (const tinyxml2::XMLElement* pMotionNode = pState->FirstChildElement("Motion"); pMotionNode; pMotionNode = pMotionNode->NextSiblingElement("Motion"))
			{
				char szType[32];
				sprintf(szType, pMotionNode->Attribute("type"));
				Motion* pMotion = NULL;

				if (strcmp(szType, "SpriteAnimation") == 0)
				{
					pMotion = new MotionSpriteAnimation();
				}
				else if (strcmp(szType, "InterpolationPosition") == 0)
				{
					pMotion = new MotionInterpolationPosition();
				}
				else if (strcmp(szType, "InterpolationRotation") == 0)
				{
					pMotion = new MotionInterpolationRotation();
				}
				else if (strcmp(szType, "InterpolationScale") == 0)
				{
					pMotion = new MotionInterpolationScale();
				}
				else if (strcmp(szType, "AlphaBlend") == 0)
				{
					pMotion = new MotionAlphaBlend();
				}

				MGD_ASSERT(pMotion);
				if (pMotion)
				{
					pMotion->SetupFromXml(pMotionNode);
				}

				m_States[ObjectId(szName)].push_back(pMotion);
			}
		}
	}

	return Component::SetupFromXml(pNode);
}

void AnimatorComponent::SetState(const char* i_pState)
{
}


void AnimatorComponent::CreateLuaObject()
{
	m_oLuaObject.AssignNewTable(LuaManager::GetSingleton().GetLuaState());

	LuaPlus::LuaObject metaTable =	LuaManager::GetSingleton().GetGlobalVars().GetByName(METATABLE_NAME);

	m_oLuaObject.SetLightUserData("__object", const_cast<AnimatorComponent*>(this));
	m_oLuaObject.SetMetaTable(metaTable);
}



void AnimatorComponent::RegisterScriptFunction()
{
	LuaPlus::LuaObject metaTable = LuaManager::GetSingleton().GetGlobalVars().CreateTable(METATABLE_NAME);
	metaTable.SetObject("__index", metaTable); // it’s also its own metatable
	// register the SetPosition() function
	metaTable.RegisterObjectDirect("SetState", (AnimatorComponent*)0, &AnimatorComponent::SetState);
}
