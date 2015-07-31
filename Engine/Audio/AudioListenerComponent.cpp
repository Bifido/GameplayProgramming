#include "AudioListenerComponent.h"
#include "..\Manager\LuaManager.h"
#include "../OtherComponents/TransformationComponent.h"
#include "../Physics/PhysicsComponent.h"
#include "../Manager/SystemManager.h"
#include "../LOG/LOGManager.h"
#include "AudioUtils.h"

ID_IMPLEMENTATION(AudioListenerComponent);

const char* AudioListenerComponent::METATABLE_NAME = "AudioListenerComponentMetatable";

AudioListenerComponent::AudioListenerComponent(std::string i_szOwnerID, bool i_bIsCreatedFromTemplate)
	:Component(i_szOwnerID, i_bIsCreatedFromTemplate)
{
	CreateLuaObject();
	AudioUtils::FillOpenALfloat3(0.0f, m_pVelocity);
	AudioUtils::FillOpenALfloat3(0.0f, m_pPosition);
	AudioUtils::FillOpenALfloat6(0.0f, m_pOrientation);
}


AudioListenerComponent::~AudioListenerComponent()
{
}

void AudioListenerComponent::Init()
{
	std::string szEventName;
	szEventName.append(GetOwnerID().GetDebugName().c_str());
	szEventName.append("/POSITION_CHANGED");
	m_oEvent.Subscribe(this, &AudioListenerComponent::SyncronizePosition, ObjectId(szEventName.c_str()));

	alListenerfv(AL_POSITION, m_pPosition);                                  //Set position of the listener
	alListenerfv(AL_ORIENTATION, m_pOrientation);                            //Set orientation of the listener
	alListenerfv(AL_VELOCITY, m_pVelocity);                                  //Set velocity of the listener

	MGD_LOG::LOGManager::GetSingleton().WriteLog(MGD_LOG::MGD_INFO, AUDIO_CONTEXT, "AudioListenerComponent initialized!");
}

void AudioListenerComponent::CreateLuaObject()
{
	m_oLuaObject.AssignNewTable(LuaManager::GetSingleton().GetLuaState());

	LuaPlus::LuaObject metaTable = LuaManager::GetSingleton().GetGlobalVars().GetByName(METATABLE_NAME);

	m_oLuaObject.SetLightUserData("__object", const_cast<AudioListenerComponent*>(this));
	m_oLuaObject.SetMetaTable(metaTable);
}



void AudioListenerComponent::RegisterScriptFunction()
{
	//LuaPlus::LuaObject metaTable = LuaManager::GetSingleton().GetGlobalVars().CreateTable(METATABLE_NAME);
	//metaTable.SetObject("__index", metaTable); // it’s also its own metatable
	// register the ??() function
	//metaTable.RegisterObjectDirect("SetState", (AudioListenerComponent*)0, &AudioListenerComponent::??);
}

//bool AudioListenerComponent::SetupFromXml(const tinyxml2::XMLElement* pNode)
//{
//	return false;
//}

void AudioListenerComponent::SyncronizePosition()
{
	TransformationComponent* pTransformationComponent = NULL;
	Component* component = SystemManager::GetSingleton().EditSingleComponent(TransformationComponent::ID, GetOwnerID());
	if (component != NULL)
	{
		pTransformationComponent = static_cast<TransformationComponent*>(component);

		if (pTransformationComponent)
		{
			Vec3 oPos = pTransformationComponent->GetPosition();
			Quaternion oRotation = pTransformationComponent->GetRotation();

			AudioUtils::ConvertVec3ToOpenALobj(oPos, m_pPosition);
			AudioUtils::ConvertQuatToOpenALobj(oRotation, m_pOrientation);
                                                                          
			alListenerfv(AL_POSITION, m_pPosition);                                  //Set position of the listener
			alListenerfv(AL_ORIENTATION, m_pOrientation);                            //Set orientation of the listener
		}
		else
		{
			alListenerfv(AL_POSITION, AudioUtils::ZeroALfloatVec3);                                  
			alListenerfv(AL_ORIENTATION, AudioUtils::ZeroALfloatVec6);
		}
	}
	else
	{
		alListenerfv(AL_POSITION, AudioUtils::ZeroALfloatVec3);
		alListenerfv(AL_ORIENTATION, AudioUtils::ZeroALfloatVec6);
	}

	PhysicsComponent* pPhysicsComponent = NULL;
	component = SystemManager::GetSingleton().EditSingleComponent(PhysicsComponent::ID, GetOwnerID());
	if (component != NULL)
	{
		pPhysicsComponent = static_cast<PhysicsComponent*>(component);
		if (pPhysicsComponent)
		{
			Vec3 oVel = pPhysicsComponent->GetVec3Velocity();
			AudioUtils::ConvertVec3ToOpenALobj(oVel, m_pVelocity);

			alListenerfv(AL_VELOCITY, m_pVelocity);                                  //Set velocity of the listener
		}
		else
		{
			alListenerfv(AL_VELOCITY, AudioUtils::ZeroALfloatVec3);
		}
	}
	else
	{
		alListenerfv(AL_VELOCITY, AudioUtils::ZeroALfloatVec3);
	}
}
