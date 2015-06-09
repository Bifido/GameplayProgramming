#include "../Core/Actor.h"
#include "../Event/EventRegistry.h"

Actor::Actor(ObjectId i_oName)
	: m_oName(i_oName)
{
}

Actor::~Actor()
{
	MGDVector<Component*>::iterator it = m_Components.begin();
	for(; it != m_Components.end(); ++it)
	{
		delete(*it);
	}

	m_Components.clear();

#if defined(_DEBUG)
	--NumActors;
#endif
}

void Actor::Init()
{
	MGDVector<Component*>::iterator it = m_Components.begin();
	for(; it != m_Components.end(); ++it)
	{
		(*it)->Init();

		char pEventName[128];
		sprintf_s(pEventName, "Registered/%s", (*it)->GetID().GetDebugName().c_str());
		EventRegistry::GetSingleton().Raise(ObjectId(pEventName),(*it)->GetOwnerID().GetDebugName().c_str());
	}
}

void Actor::AddComponent(Component* i_pComponent)
{
	MGD_ASSERT(i_pComponent);
	if(i_pComponent)
	{
		m_Components.push_back(i_pComponent);
	}
}

Component* Actor::GetComponent( const ObjectId& i_oComponentID ) const
{
	for(uint32 uiIndex = 0; uiIndex < m_Components.size(); ++uiIndex)
	{
		if(m_Components[uiIndex]->GetID() == i_oComponentID)
		{
			return m_Components[uiIndex];
		}
	}
}
