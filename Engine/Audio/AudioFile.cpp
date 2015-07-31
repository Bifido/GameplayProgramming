#include "AudioFile.h"
#include "AudioUtils.h"
#include "../LOG/LOGManager.h"

ID_IMPLEMENTATION(AudioFile);
ID_IMPLEMENTATION(WaveAudioFile);

AudioFile::AudioFile(const std::string& i_szName)
	:m_szName(i_szName),
	m_oId(m_szName.c_str()),
	m_bLoaded(false),
	m_uRefCounter(1)
{
}

AudioFile::~AudioFile()
{
	//Delete the OpenAL Buffer
	if (m_uBuffer)
		alDeleteBuffers(1, &m_uBuffer);
}

ALenum AudioFile::GetFormat(short bitsPerSample, short channels)
{
	//Figure out the format of the WAVE file
	if (bitsPerSample == 8)
	{
		if (channels == 1)
			return AL_FORMAT_MONO8;
		else if (channels == 2)
			return AL_FORMAT_STEREO8;
	}
	else if (bitsPerSample == 16)
	{
		if (channels == 1)
			return AL_FORMAT_MONO16;
		else if (channels == 2)
			return AL_FORMAT_STEREO16;
	}

	//Not valid format
	return 0;
}

bool WaveAudioFile::Load()
{
	//Loading of the WAVE file
	if(!m_bLoaded)
	{
		FILE *fp = NULL;                //Create FILE pointer for the WAVE file
		fp = fopen(m_szName.c_str(), "rb");		//Open the WAVE file
		if (!fp)
		{
			//Could not open file
			m_bLoaded = false;
			MGD_LOG::LOGManager::GetSingleton().WriteLog(MGD_LOG::MGD_ERROR, AUDIO_CONTEXT, "Failed to open audio file: %s", m_szName.c_str());
		}
		else
		{
			if (!CheckWaveFormat(fp, "RIFF"))
			{
				m_bLoaded = false;
				MGD_LOG::LOGManager::GetSingleton().WriteLog(MGD_LOG::MGD_ERROR, AUDIO_CONTEXT, "Not RIFF, audio file: %s", m_szName.c_str());
			}
			else
			{
				fread(&m_wSize, sizeof(DWORD), 1, fp);

				if (!CheckWaveFormat(fp, "WAVE"))
				{
					m_bLoaded = false;
					MGD_LOG::LOGManager::GetSingleton().WriteLog(MGD_LOG::MGD_ERROR, AUDIO_CONTEXT, "Not WAVE, audio file: %s", m_szName.c_str());
				}
				else if (!CheckWaveFormat(fp, "fmt "))
				{
					m_bLoaded = false;
					MGD_LOG::LOGManager::GetSingleton().WriteLog(MGD_LOG::MGD_ERROR, AUDIO_CONTEXT, "Not ftm, audio file: %s", m_szName.c_str());
				}
				else
				{
					//Now we know that the file is a acceptable WAVE file
					//Info about the WAVE data is now read and stored
					fread(&m_wChunkSize, sizeof(DWORD), 1, fp);
					fread(&m_sFormatType, sizeof(short), 1, fp);
					fread(&m_sChannels, sizeof(short), 1, fp);
					fread(&m_wSampleRate, sizeof(DWORD), 1, fp);
					fread(&m_wAvgBytesPerSec, sizeof(DWORD), 1, fp);
					fread(&m_sBytesPerSample, sizeof(short), 1, fp);
					fread(&m_sBitsPerSample, sizeof(short), 1, fp);

					if (!CheckWaveFormat(fp, "data"))
					{
						m_bLoaded = false;
						MGD_LOG::LOGManager::GetSingleton().WriteLog(MGD_LOG::MGD_ERROR, AUDIO_CONTEXT, "Missing DATA, audio file: %s", m_szName.c_str());
					}
					else
					{
						//The size of the sound data is read
						fread(&m_wDataSize, sizeof(DWORD), 1, fp);

						MGD_LOG::LOGManager::GetSingleton().WriteLog(MGD_LOG::MGD_INFO, AUDIO_CONTEXT,
							"Audio file: %s - Chunck size: %d - Format Type: %d - Channels: %d - Sample Rate: %d - Average Bytes Per Second : %d - Bytes Per Sample: %d - Bits Per Sample: %d - Data Size: %d",
							m_szName.c_str(), m_wChunkSize, m_sFormatType, m_sChannels, m_wSampleRate,
							m_wAvgBytesPerSec, m_sBytesPerSample, m_sBitsPerSample, m_wDataSize);

						//Allocate memory for the sound data
						unsigned char* buf = new unsigned char[m_wDataSize];
						//Load audio data in buf
						fread(buf, sizeof(BYTE), m_wDataSize, fp);

						m_wFormat = GetFormat(m_sBitsPerSample, m_sChannels);
						if (!m_wFormat)
						{
							m_bLoaded = false;
							MGD_LOG::LOGManager::GetSingleton().WriteLog(MGD_LOG::MGD_ERROR, AUDIO_CONTEXT, "Wrong BitPerSample, audio file: %s", m_szName.c_str());
						}
						else
						{
							m_uFrequency = m_wSampleRate;
							//Generate one OpenAL Buffer and link to "m_uBuffer"
							alGenBuffers(1, &m_uBuffer);
							//Store the sound data in the OpenAL Buffer
							alBufferData(m_uBuffer, m_wFormat, buf, m_wDataSize, m_uFrequency);

							ALenum error = alGetError();
							if (error != AL_NO_ERROR)
							{
								//Error during buffer loading
								MGD_LOG::LOGManager::GetSingleton().WriteLog(MGD_LOG::MGD_ERROR, AUDIO_CONTEXT, "Error loading ALBuffer: %s, audio file: %s", AudioUtils::GetALErrorString(error).c_str(), m_szName.c_str());
							}
							else
							{
								//File load OK
								m_bLoaded = true;
							}
						}
						delete[] buf; //Delete the sound data buffer
					}
				}
			}
			//Close the WAVE file
			fclose(fp);
		}
	}
	return m_bLoaded;
}

bool WaveAudioFile::CheckWaveFormat(FILE *fp, const char* toCheck)
{
	char type[4];
	//Check that the WAVE file is OK
	fread(type, sizeof(char), 4, fp);
	if (type[0] != toCheck[0] || type[1] != toCheck[1] || type[2] != toCheck[2] || type[3] != toCheck[3])
	{
		return false;
	}
	return true;
}

