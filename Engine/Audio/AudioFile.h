#pragma once
#include "../Common/DataTypes.h"
#include "AL/al.h"


class AudioFile
{

public:
	ID_DECLARATION;

	AudioFile(const std::string& i_szName);
	virtual ~AudioFile();

	virtual bool Load(){ return false; };

	const AudioID& GetName() const
	{
		return m_oId;
	}

	const bool IsLoaded() const
	{
		return m_bLoaded;
	}

	const ALuint& GetBuffer() const
	{
		return m_uBuffer;
	}

	void AddRefCounter()
	{
		++m_uRefCounter;
	}

	void RemoveRefCounter()
	{
		--m_uRefCounter;
	}

	uint32 GetRefCounter() const
	{
		return m_uRefCounter;
	}

protected:
	//virtual void Unload();
	bool m_bLoaded;
	std::string m_szName;
	AudioID		m_oId;

	ALuint m_uBuffer;					//Stores the sound data
	ALuint m_uFrequency;				//The Sample Rate of the WAVE file
	ALenum m_wFormat;					//The audio format (bits per sample, number of channels)

	ALenum GetFormat(short bitsPerSample, short channels);

	uint32 m_uRefCounter;
};

class WaveAudioFile : public AudioFile
{

public:
	ID_DECLARATION;

	WaveAudioFile(const std::string& i_szName) : AudioFile(i_szName) {};
	virtual ~WaveAudioFile(){};

	virtual bool Load();

protected:
	DWORD m_wSize, m_wChunkSize;
	short m_sFormatType, m_sChannels;
	DWORD m_wSampleRate, m_wAvgBytesPerSec;
	short m_sBytesPerSample, m_sBitsPerSample;
	DWORD m_wDataSize;

private:
	bool CheckWaveFormat(FILE *fp, const char* toCheck);
};

