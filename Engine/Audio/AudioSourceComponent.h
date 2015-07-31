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

	void SetPitch(float pitch);
	void SetGain(float gain);
	void SetLooping(bool looping);
	void SetPlayAwake(bool playAwake);

private:

	void CreateLuaObject();

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

	AudioID m_oAudioFile;

	void Test();

};

