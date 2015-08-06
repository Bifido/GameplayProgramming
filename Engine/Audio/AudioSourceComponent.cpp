#include "AudioSourceComponent.h"
#include "AudioFile.h"
#include "AudioSystem.h"
#include "AudioUtils.h"

#include "..\Manager\LuaManager.h"
#include "..\LOG\LOGManager.h"
#include "../OtherComponents/TransformationComponent.h"
#include "../Physics/PhysicsComponent.h"


ID_IMPLEMENTATION(AudioSourceComponent);

const char* AudioSourceComponent::METATABLE_NAME = "AudioSourceComponentMetatable";

AudioSourceComponent::AudioSourceComponent(std::string i_szOwnerID, bool i_bIsCreatedFromTemplate)
	:Component(i_szOwnerID, i_bIsCreatedFromTemplate), m_uSource(AL_INVALID)
{
	CreateLuaObject();
	AudioUtils::FillOpenALfloat3(0.0f, m_pVelocity);
	AudioUtils::FillOpenALfloat3(0.0f, m_pPosition);

	m_bLooping = false;
	m_bPlayAwake = false;
	m_bStraming = false;
}


AudioSourceComponent::~AudioSourceComponent()
{
	if (m_uSource != AL_INVALID)
		alDeleteSources(1, &m_uSource); //Delete the OpenAL Source

	AudioSystem* audioSystem = dynamic_cast<AudioSystem*>(SystemManager::GetSingleton().EditSystem(AudioSystem::ID));
	audioSystem->UnloadAudioFile(m_oAudioFile);

	MGD_LOG::LOGManager::GetSingleton().WriteLog(MGD_LOG::MGD_INFO, AUDIO_CONTEXT, "AudioSourceComponent deleted!; %s", GetOwnerID().GetDebugName().c_str());
}

void AudioSourceComponent::Init()
{
	std::string szEventName;
	szEventName.append(GetOwnerID().GetDebugName().c_str());
	szEventName.append("/POSITION_CHANGED");
	m_oEvent.Subscribe(this, &AudioSourceComponent::SyncronizePosition, ObjectId(szEventName.c_str()));

	//Generate one OpenAL Source and link to "source"
	alGenSources(1, &m_uSource);  
	ALenum errEnum = alGetError();
	if (errEnum != AL_NO_ERROR)
	{
		//Error during buffer/source generation
		MGD_LOG::LOGManager::GetSingleton().WriteLog(MGD_LOG::MGD_ERROR, AUDIO_CONTEXT, "AudioSourceComponent source generation error: (OpenAL err: %s)", AudioUtils::GetALErrorString(errEnum).c_str());
	}
	else
	{
		AudioSystem* audioSystem = dynamic_cast<AudioSystem*>(SystemManager::GetSingleton().EditSystem(AudioSystem::ID));
		AudioFile* audioFile = audioSystem->GetAudioFile(m_oAudioFile);
		if (audioFile && audioFile->IsLoaded())
		{
			alSourcei(m_uSource, AL_BUFFER, audioFile->GetBuffer());
			alSourcei(m_uSource, AL_SOURCE_RELATIVE, AL_TRUE);

			SetPitch(m_fPitch);
			SetGain(m_fGain);
			SetLooping(m_bLooping);

			SyncronizePosition();

			if (m_bPlayAwake)
				Play();

			MGD_LOG::LOGManager::GetSingleton().WriteLog(MGD_LOG::MGD_INFO, AUDIO_CONTEXT, "AudioSourceComponent initialized!; %s", GetOwnerID().GetDebugName().c_str());
		}
	}
}

void AudioSourceComponent::CreateLuaObject()
{
	m_oLuaObject.AssignNewTable(LuaManager::GetSingleton().GetLuaState());

	LuaPlus::LuaObject metaTable = LuaManager::GetSingleton().GetGlobalVars().GetByName(METATABLE_NAME);

	m_oLuaObject.SetLightUserData("__object", const_cast<AudioSourceComponent*>(this));
	m_oLuaObject.SetMetaTable(metaTable);
}



void AudioSourceComponent::RegisterScriptFunction()
{
	LuaPlus::LuaObject metaTable = LuaManager::GetSingleton().GetGlobalVars().CreateTable(METATABLE_NAME);
	metaTable.SetObject("__index", metaTable); // it’s also its own metatable
	
	metaTable.RegisterObjectDirect("Play", (AudioSourceComponent*)0, &AudioSourceComponent::Play);
	metaTable.RegisterObjectDirect("Stop", (AudioSourceComponent*)0, &AudioSourceComponent::Stop);
	metaTable.RegisterObjectDirect("Pause", (AudioSourceComponent*)0, &AudioSourceComponent::Pause);
	metaTable.RegisterObjectDirect("SetLooping", (AudioSourceComponent*)0, &AudioSourceComponent::SetLoopingFromScript);
	metaTable.RegisterObjectDirect("SetPitch", (AudioSourceComponent*)0, &AudioSourceComponent::SetPitchFromScript);
	metaTable.RegisterObjectDirect("SetGain", (AudioSourceComponent*)0, &AudioSourceComponent::SetGainFromScript);
	metaTable.RegisterObjectDirect("SetPlayAwake", (AudioSourceComponent*)0, &AudioSourceComponent::SetPlayAwakeFromScript);

}

void AudioSourceComponent::CreateFromTemplate(Component* i_pComponent, const ObjectId& i_oOwner)
{
	if (i_pComponent)
	{
		AudioSourceComponent* pAudioSourceComponent = static_cast<AudioSourceComponent*>(i_pComponent);
		if (pAudioSourceComponent)
		{
			//m_uSource = pAudioSourceComponent->m_uSource;
			m_fPitch = pAudioSourceComponent->m_fPitch;
			m_fGain = pAudioSourceComponent->m_fGain;
			m_bLooping = pAudioSourceComponent->m_bLooping;
			m_bPlayAwake = pAudioSourceComponent->m_bPlayAwake;
			//SyncronizePosition();
			m_oAudioFile = pAudioSourceComponent->m_oAudioFile;

			AudioSystem* audioSystem = dynamic_cast<AudioSystem*>(SystemManager::GetSingleton().EditSystem(AudioSystem::ID));
			audioSystem->AddTemplateAudioFile(m_oAudioFile);

			Component::CreateFromTemplate(i_oOwner);
		}
	}
}

bool AudioSourceComponent::SetupFromXml(const tinyxml2::XMLElement* pNode)
{
	if (pNode)
	{
		const tinyxml2::XMLElement* pAudioFile = pNode->FirstChildElement("AudioFile");
		if (pAudioFile)
		{
			const char* type = pAudioFile->Attribute("type");
			std::string filename(pAudioFile->Attribute("filename"));
			bool directLoad = pAudioFile->BoolAttribute("directLoad");

			AudioSystem* audioSystem = dynamic_cast<AudioSystem*>(SystemManager::GetSingleton().EditSystem(AudioSystem::ID));
			m_oAudioFile = audioSystem->AddAudioFile(filename, type, directLoad);
		}

		const tinyxml2::XMLElement* pPitch = pNode->FirstChildElement("Pitch");
		if (pPitch)
		{
			m_fPitch = pPitch->FloatAttribute("value");
		}
		const tinyxml2::XMLElement* pGain = pNode->FirstChildElement("Gain");
		if (pGain)
		{
			m_fGain = pGain->FloatAttribute("value");
		}
		const tinyxml2::XMLElement* pLooping = pNode->FirstChildElement("Looping");
		if (pLooping)
		{
			m_bLooping = pLooping->BoolAttribute("value");
		}
		const tinyxml2::XMLElement* pPlayAwake = pNode->FirstChildElement("PlayAwake");
		if (pPlayAwake)
		{
			m_bPlayAwake = pPlayAwake->BoolAttribute("value");
		}
	}
	return Component::SetupFromXml(pNode);
}

void AudioSourceComponent::SetPitch(float pitch)
{
	if (m_uSource != AL_INVALID)
	{
		m_fPitch = pitch;
		alSourcef(m_uSource, AL_PITCH, m_fPitch);                                 //Set the pitch of the source
		ALenum errEnum = alGetError();
		if (errEnum != AL_NO_ERROR)
		{
			//Error during buffer/source generation
			MGD_LOG::LOGManager::GetSingleton().WriteLog(MGD_LOG::MGD_ERROR, AUDIO_CONTEXT, "AudioSourceComponent SetPitch error: (OpenAL err: %s)", AudioUtils::GetALErrorString(errEnum).c_str());
		}
	}
}

void AudioSourceComponent::SetGain(float gain)
{
	if (m_uSource != AL_INVALID)
	{
		m_fGain = gain;
		alSourcef(m_uSource, AL_GAIN, m_fGain);                                 //Set the gain of the source
		ALenum errEnum = alGetError();
		if (errEnum != AL_NO_ERROR)
		{
			//Error during buffer/source generation
			MGD_LOG::LOGManager::GetSingleton().WriteLog(MGD_LOG::MGD_ERROR, AUDIO_CONTEXT, "AudioSourceComponent SetGain error: (OpenAL err: %s)", AudioUtils::GetALErrorString(errEnum).c_str());
		}
	}
}

void AudioSourceComponent::SetLooping(bool looping)
{
	if (m_uSource != AL_INVALID)
	{
		m_bLooping = looping;
		alSourcei(m_uSource, AL_LOOPING, m_bLooping);                                 //Set if source is looping sound
		ALenum errEnum = alGetError();
		if (errEnum != AL_NO_ERROR)
		{
			//Error during buffer/source generation
			MGD_LOG::LOGManager::GetSingleton().WriteLog(MGD_LOG::MGD_ERROR, AUDIO_CONTEXT, "AudioSourceComponent SetLooping error: (OpenAL err: %s)", AudioUtils::GetALErrorString(errEnum).c_str());
		}
	}
}

void AudioSourceComponent::SetPlayAwake(bool playAwake)
{
	m_bPlayAwake = playAwake;
}

void AudioSourceComponent::SyncronizePosition()
{
	if (m_uSource != AL_INVALID)
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
				//MGD_LOG::LOGManager::GetSingleton().WriteLog(MGD_LOG::MGD_INFO, AUDIO_CONTEXT, "AudioSource position %s, x:%f y:%f", GetOwnerID().GetDebugName().c_str(), oPos.x,oPos.y);
				alSourcefv(m_uSource, AL_POSITION, m_pPosition);                                  //Set position of the listener
			}
			else
			{
				alSourcefv(m_uSource, AL_POSITION, AudioUtils::ZeroALfloatVec3);
			}
		}
		else
		{
			alSourcefv(m_uSource, AL_POSITION, AudioUtils::ZeroALfloatVec3);
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

				alSourcefv(m_uSource, AL_VELOCITY, m_pVelocity);                                  //Set velocity of the listener
			}
			else
			{
				alSourcefv(m_uSource, AL_VELOCITY, AudioUtils::ZeroALfloatVec3);
			}
		}
		else
		{
			alSourcefv(m_uSource, AL_VELOCITY, AudioUtils::ZeroALfloatVec3);
		}
	}
}

void AudioSourceComponent::Play()
{
	alSourcePlay(m_uSource);
}

void AudioSourceComponent::Stop()
{
	alSourceStop(m_uSource);
}

void AudioSourceComponent::Pause()
{
	alSourcePause(m_uSource);
}

void AudioSourceComponent::SetPitchFromScript(float i_fPitch)
{
	SetPitch(i_fPitch);
}
void AudioSourceComponent::SetGainFromScript(float i_fGain)
{
	SetGain(i_fGain);
}
void AudioSourceComponent::SetLoopingFromScript(bool i_bLooping)
{
	SetLooping(i_bLooping);
}
void AudioSourceComponent::SetPlayAwakeFromScript(bool i_bPlayAwake)
{
	SetPlayAwake(i_bPlayAwake);
}
//
//enum { Left = 0, Right = 1 };
//ALuint Source[2];
//
//alGenSources(2, Source);
//...
//alSourcei(Source[Left], AL_SOURCE_RELATIVE, AL_TRUE);
//alSource3f(Source[Left], AL_POSITION, -1.0f, 0.0f, 0.0f);
//alSourcei(Source[Right], AL_SOURCE_RELATIVE, AL_TRUE);
//alSource3f(Source[Right], AL_POSITION, +1.0f, 0.0f, 0.0f);
//
//// Both sources can use the same buffer, no problem
//alSourcei(Source[Left], AL_BUFFER, buffer);
//alSourcei(Source[Right], AL_BUFFER, buffer);
//...
//// Use alSourcePlayv to make sure they start synchronized
//alSourcePlayv(2, Sources);
//
//// Update panning
//alSourcef(Source[Left], AL_GAIN, clamp(1.0f - pan, 0.0f, 1.0f));
//alSourcef(Source[Right], AL_GAIN, clamp(1.0f + pan, 0.0f, 1.0f));
