#pragma once

#include "../Core/System.h"
#include "../Common/DataTypes.h"
#include "../Event\RegistryEventHandler.h"

#include "AudioFile.h"

#include "AL/alc.h"

//class ALCdevice;
//class ALCcontext;

class AudioSystem : public System
{
public:
	ID_DECLARATION;

	AudioSystem();
	virtual ~AudioSystem();

	virtual void	Init();
	virtual void	Update(real i_fFrametime, real i_fTimestep);

	AudioID AddAudioFile(const std::string& i_fileName, const char* i_type, bool i_directLoad);
	void AddTemplateAudioFile(const AudioID& i_audioID);
	//void DeleteAudioFile(const AudioID& i_audioID);
	bool LoadAudioFile(const AudioID& i_audioID);
	bool UnloadAudioFile(const AudioID& i_audioID);
	bool isLoadedAudioFile(const AudioID& i_audioID);
	AudioFile* GetAudioFile(const AudioID& i_audioID);

private:
	void AddAudioComponent();

	void CreateObjects(const char* i_szName);
	void DeleteObjects(const char* i_szName);

	RegistryEventHandler<AudioSystem, const char*> m_oRegisterEvent;
	RegistryEventHandler<AudioSystem, const char*> m_oUnregisterEvent;

	ALCdevice* m_pDevice;
	ALCcontext* m_pContext;

	MGDMap<AudioID, AudioFile*> m_AudioFiles;
	MGDMap<AudioID, AudioFile*> m_StreamingAudioFiles;
};

