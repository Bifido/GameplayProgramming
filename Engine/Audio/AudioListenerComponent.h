#pragma once

#include "../Core/Component.h"
#include "../Common/DataTypes.h"
#include "../Event\RegistryEventHandler.h"

#include "AL\al.h"

class AudioListenerComponent : public Component
{
public:
	ID_DECLARATION;

	AudioListenerComponent(std::string i_szOwnerID, bool i_bIsCreatedFromTemplate = false);
	virtual ~AudioListenerComponent();

	virtual void Init();
	//virtual bool SetupFromXml(const tinyxml2::XMLElement* pNode);
	virtual void CreateFromTemplate(Component* i_pComponent, const ObjectId& i_oOwner)
	{
		if (i_pComponent)
		{
			AudioListenerComponent* pAudioListenerComponent = static_cast<AudioListenerComponent*>(i_pComponent);
			if (pAudioListenerComponent)
			{
				SyncronizePosition();
				Component::CreateFromTemplate(i_oOwner);
			}
		}
	}

	static void RegisterScriptFunction();

private:

	void CreateLuaObject();
	static const char* METATABLE_NAME;

	RegistryEventHandler<AudioListenerComponent> m_oEvent;
	void SyncronizePosition();

	//from TransformationComponent
	ALfloat m_pPosition[3];
	ALfloat m_pOrientation[6];
	//from PhysicsComponent
	ALfloat m_pVelocity[3];
};

