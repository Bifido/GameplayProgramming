#include "AudioSystem.h"
#include "..\LOG\LOGManager.h"
#include "..\Manager\SystemManager.h"

#include "AL\al.h"

ID_IMPLEMENTATION(AudioSystem);

AudioSystem::AudioSystem()
	:System()
{
	m_oRegisterEvent.Subscribe(this, &AudioSystem::CreateObjects, ObjectId("Registered/AudioSystem"));
	m_oUnregisterEvent.Subscribe(this, &AudioSystem::DeleteObjects, ObjectId("Test/Unregistered/AudioSystem"));

	MGD_LOG::LOGManager::GetSingleton().WriteLog(MGD_LOG::MGD_INFO, SYSTEMS_CONTEXT, "AudioSystem created!");
}


AudioSystem::~AudioSystem()
{
	alcMakeContextCurrent(NULL);				//Make no context current
	if (m_pContext)
		alcDestroyContext(m_pContext);			//Destroy the OpenAL Context
	if (m_pDevice)
		alcCloseDevice(m_pDevice);				//Close the OpenAL Device

	MGDMap<AudioID, AudioFile*>::iterator it = m_AudioFiles.begin();
	for (; it != m_AudioFiles.end(); ++it)
	{
		UnloadAudioFile(it->second->GetName());
	}

	m_AudioFiles.clear();

	MGD_LOG::LOGManager::GetSingleton().WriteLog(MGD_LOG::MGD_INFO, SYSTEMS_CONTEXT, "AudioSystem destroyed!");
}

void AudioSystem::Init()
{
	//Now OpenAL needs to be initialized 
	m_pDevice = alcOpenDevice(NULL);						//Open the device
	if (!m_pDevice)
	{
		//Error during device opening
		MGD_LOG::LOGManager::GetSingleton().WriteLog(MGD_LOG::MGD_ERROR, AUDIO_CONTEXT, "NO sound device");
	}
	else
	{
		m_pContext = alcCreateContext(m_pDevice, NULL);		//Give the device a context
		alcMakeContextCurrent(m_pContext);					//Make the context the current
		if (!m_pContext)
		{
			//Error during context handeling
			MGD_LOG::LOGManager::GetSingleton().WriteLog(MGD_LOG::MGD_ERROR, AUDIO_CONTEXT, "NO sound context");
		}
		else
		{
			MGD_LOG::LOGManager::GetSingleton().WriteLog(MGD_LOG::MGD_INFO, SYSTEMS_CONTEXT, "AudioSystem initialized!");
		}
	}
}

void AudioSystem::CreateObjects(const char* i_szName)
{

}

void AudioSystem::DeleteObjects(const char* i_szName)
{

}
	

void AudioSystem::Update(real i_fFrametime, real i_fTimestep)
{
}

void AudioSystem::AddTemplateAudioFile(const AudioID& i_audioID)
{
	MGDMap<AudioID, AudioFile*>::iterator it = m_AudioFiles.find(i_audioID);
	if (it == m_AudioFiles.end())
	{
		MGD_LOG::LOGManager::GetSingleton().WriteLog(MGD_LOG::MGD_ERROR, AUDIO_CONTEXT, "AudioSystem: (AddTemplateAudioFile) AudioFile not found, ID: %s", i_audioID.GetDebugName().c_str());
	}
	else
	{
		it->second->AddRefCounter();
	}
}

AudioID AudioSystem::AddAudioFile(const std::string& i_fileName, const char* i_type, bool i_directLoad)
{
	const AudioID id = AudioID(i_fileName.c_str());

	MGDMap<AudioID, AudioFile*>::iterator it = m_AudioFiles.find(id);
	if (it == m_AudioFiles.end())
	{
		if (strcmp(i_type, "wav") == 0)
		{
			AudioFile* audioFile = new WaveAudioFile(i_fileName);
			m_AudioFiles[id] = audioFile;

			if (i_directLoad)
				audioFile->Load();

			return id;
		}
		else
		{
			MGD_LOG::LOGManager::GetSingleton().WriteLog(MGD_LOG::MGD_ERROR, AUDIO_CONTEXT, "Format %s not supported", i_type);
			return AudioID(0);
		}
	}
	else
	{
		it->second->AddRefCounter();
		//This file is already added
		return it->second->GetName();
	}
}

bool AudioSystem::LoadAudioFile(const AudioID& i_audioID)
{
	AudioFile* audioFile = GetAudioFile(i_audioID);
	if (audioFile)
	{
		return audioFile->Load();
	}

	return false;
}

bool AudioSystem::UnloadAudioFile(const AudioID& i_audioID)
{
	MGDMap<AudioID, AudioFile*>::iterator it = m_AudioFiles.find(i_audioID);
	if (it != m_AudioFiles.end())
	{
		if (it->second->GetRefCounter() <= 1)
		{
			delete it->second;
			m_AudioFiles.erase(it);
		}
		else
		{
			it->second->RemoveRefCounter();
		}
		return true;
	}
	else
	{
		MGD_LOG::LOGManager::GetSingleton().WriteLog(MGD_LOG::MGD_ERROR, AUDIO_CONTEXT, "AudioSystem: (Unload) AudioFile not found, ID: %s", i_audioID.GetDebugName().c_str());
		return false;
	}
}

bool AudioSystem::isLoadedAudioFile(const AudioID& i_audioID)
{
	AudioFile* audioFile = GetAudioFile(i_audioID);
	if (audioFile)
	{
		return audioFile->IsLoaded();
	}

	return false;
}

AudioFile* AudioSystem::GetAudioFile(const AudioID& i_audioID)
{
	MGDMap<AudioID, AudioFile*>::iterator it = m_AudioFiles.find(i_audioID);
	if (it != m_AudioFiles.end())
	{
		return it->second;
	}
	else
	{
		MGD_LOG::LOGManager::GetSingleton().WriteLog(MGD_LOG::MGD_ERROR, AUDIO_CONTEXT, "AudioSystem: (Get) AudioFile not found, ID: %s", i_audioID.GetDebugName().c_str());
		return NULL;
	}
}