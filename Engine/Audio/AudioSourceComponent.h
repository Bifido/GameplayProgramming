#pragma once

#include "../Core/Component.h"
#include "../Common/DataTypes.h"
#include "../Event\RegistryEventHandler.h"

#include "AL/al.h"

class AudioFile;

class AudioSourceComponent : public Component
{
public:
	ID_DECLARATION;


	AudioSourceComponent(std::string i_szOwnerID, bool i_bIsCreatedFromTemplate = false);
	virtual ~AudioSourceComponent();

	virtual void Init();
	virtual bool SetupFromXml(const tinyxml2::XMLElement* pNode);
	virtual void CreateFromTemplate(Component* i_pComponent, const ObjectId& i_oOwner);

	static void RegisterScriptFunction();

	void Play();
	void Stop();
	void Pause();

	void SetPitch(float i_fPitch);
	void SetGain(float i_fGain);
	void SetLooping(bool i_bLooping);
	void SetPlayAwake(bool i_bPlayAwake);

private:

	void CreateLuaObject();

	void SetPitchFromScript(float i_fPitch);
	void SetGainFromScript(float i_fGain);
	void SetLoopingFromScript(bool i_bLooping);
	void SetPlayAwakeFromScript(bool i_bPlayAwake);

	static const char* METATABLE_NAME;

	RegistryEventHandler<AudioSourceComponent> m_oEvent;
	void SyncronizePosition();

	//from TransformationComponent
	ALfloat m_pPosition[3];
	//from PhysicsComponent
	ALfloat m_pVelocity[3];

	ALuint m_uSource;

	ALfloat m_fPitch;
	ALfloat m_fGain;
	bool m_bLooping;
	bool m_bPlayAwake;

	bool m_bStraming;

	AudioID m_oAudioFile;

	//enum { Left = 0, Right = 1 };
};

