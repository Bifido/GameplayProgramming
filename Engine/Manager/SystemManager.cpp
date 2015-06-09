#include "SystemManager.h"
#include "../Core/Component.h"
#include "../Core/System.h"
#include "../Event/EventRegistry.h"
#include "../Common/MGDAssert.h"
#include "../LOG/LOGManager.h"
#include "../Core/Actor.h"

void SystemManager::CreateInstance(uint16 maxEntityCount, uint8 maxPhaseCount)
{
	if(GetSingletonPtr() == NULL)
		new SystemManager(maxEntityCount, maxPhaseCount);
}

SystemManager::SystemManager(uint16 maxEntityCount, uint8 maxPhaseCount)
	:	m_uiMaxEntityCount(maxEntityCount)
	,	m_uiMaxPhaseCount(maxPhaseCount)
	,	m_bPause(false)
{
	m_vPerPhaseSystemArray.resize(m_uiMaxPhaseCount);
}

uint32 SystemManager::GetKeyForMap(const ObjectId& id) const
{
	return id.GetId();
}

SystemManager::~SystemManager()
{
	RemoveAllComponents();
	CheckRemovableComponents();

	EntityComponentTable::iterator itTemplate = m_vTemplateTable.begin();
	for (; itTemplate != m_vTemplateTable.end(); ++itTemplate)
	{
		MGDVector<Component*>::iterator itComponent = (*itTemplate).second.begin();
		for (; itComponent != (*itTemplate).second.end(); ++itComponent)
		{
			delete((*itComponent));
		}
	}

	m_vTemplateTable.clear();

	MGDVector<SystemTable>::iterator it = m_vPerPhaseSystemArray.begin();
	for(; it != m_vPerPhaseSystemArray.end(); ++it)
	{
		SystemTable::iterator itSystem = (*it).begin();
		for(; itSystem != (*it).end(); ++itSystem)
		{
			delete((*itSystem).second);
		}
	}
}

void SystemManager::AddComponent(Component* i_pComponent, const ObjectId& i_oParentId)
{
	MGD_ASSERT(i_pComponent);
	if(i_pComponent)
	{
		if (i_pComponent->GetOwnerID() != i_oParentId)
		{
			MGD_ASSERT(0 && "TODO: remove this assert for template actor");
			i_pComponent->SetOwnerID(i_oParentId);
		}

		ComponentTable::iterator cIt = m_vComponentTable.find(i_pComponent->GetID());
		if(cIt != m_vComponentTable.end())
		{
			EntityComponentTable& entityComponentMap = cIt->second;
			entityComponentMap[i_oParentId].push_back(i_pComponent);

			MGD_LOG::LOGManager::GetSingleton().WriteLog(MGD_LOG::MGD_INFO, MANAGER_CONTEXT, "Component added: %s Actor: %s", i_pComponent->GetTextName().c_str(), i_pComponent->GetOwnerID().GetDebugName().c_str());
		}
		else
		{			
			EntityComponentTable entityComponentMap;			
			entityComponentMap[i_oParentId].push_back(i_pComponent);
			m_vComponentTable[i_pComponent->GetID()]= entityComponentMap;

			MGD_LOG::LOGManager::GetSingleton().WriteLog(MGD_LOG::MGD_INFO, MANAGER_CONTEXT, "Component added: %s Actor: %s", i_pComponent->GetTextName().c_str(), i_pComponent->GetOwnerID().GetDebugName().c_str());
		}
	}
}

void SystemManager::RemoveComponent(Component* i_pComponent)
{
	MGD_ASSERT(i_pComponent);
	if(i_pComponent)
	{
		ComponentTable::iterator cIt= m_vComponentTable.find(i_pComponent->GetID());
		if(cIt != m_vComponentTable.end())
		{
			EntityComponentTable& entityComponentMap= cIt->second;
			EntityComponentTable::iterator entityIt = entityComponentMap.find(i_pComponent->GetOwnerID());
			if(entityIt != entityComponentMap.end())
			{
				MGDVector<Component*>& oComponent = (*entityIt).second;
				for(uint32 uiIndex = 0; uiIndex < oComponent.size(); ++uiIndex)
				{
					if(!oComponent[uiIndex]->IsRemovable())
					{
						if(oComponent[uiIndex]->GetName() == i_pComponent->GetName())
						{
							char pEventName[128];
							sprintf_s(pEventName, "Unregistered/%s", i_pComponent->GetID().GetDebugName().c_str());
							m_oEventPublisher.SetPath(ObjectId(pEventName));
							m_oEventPublisher.Raise(i_pComponent->GetTextName().c_str());
							i_pComponent->Remove();
						}
					}
				}				
			}
		}
	}
}

void SystemManager::RemoveComponent(const ObjectId& i_oComponentName, const ObjectId& i_oOwnerId)
{
	ComponentTable::iterator cIt = m_vComponentTable.begin();
	while(cIt != m_vComponentTable.end())
	{
		EntityComponentTable& entityComponentMap= cIt->second;
		EntityComponentTable::iterator entityIt = entityComponentMap.begin();
		while(entityIt != entityComponentMap.end())
		{
			if(entityIt->first == i_oOwnerId)
			{
				MGDVector<Component*>& oComponent = (*entityIt).second;
				for(uint32 uiIndex = 0; uiIndex < oComponent.size(); ++uiIndex)
				{
					if(!oComponent[uiIndex]->IsRemovable())
					{
						if(oComponent[uiIndex]->GetName() == i_oComponentName)
						{
							char pEventName[128];
							sprintf_s(pEventName, "Unregistered/%s", oComponent[uiIndex]->GetID().GetDebugName().c_str());
							m_oEventPublisher.SetPath(ObjectId(pEventName));
							m_oEventPublisher.Raise(oComponent[uiIndex]->GetTextName().c_str());						
							oComponent[uiIndex]->Remove();
						}
					}
				}
			}

			++entityIt;
		}

		++cIt;
	}
}

void SystemManager::RemoveComponentsByOwnerId( const ObjectId& i_oOwnerId )
{
	ComponentTable::iterator cIt = m_vComponentTable.begin();
	while(cIt != m_vComponentTable.end())
	{
		EntityComponentTable& entityComponentMap= cIt->second;
		EntityComponentTable::iterator entityIt = entityComponentMap.begin();
		while(entityIt != entityComponentMap.end())
		{
			if(entityIt->first == i_oOwnerId)
			{
				MGDVector<Component*>& oComponent = (*entityIt).second;
				for(uint32 uiIndex = 0; uiIndex < oComponent.size(); ++uiIndex)
				{
					if(!oComponent[uiIndex]->IsRemovable())
					{
						char pEventName[128];
						sprintf_s(pEventName, "Unregistered/%s", oComponent[uiIndex]->GetID().GetDebugName().c_str());
						m_oEventPublisher.SetPath(ObjectId(pEventName));
						m_oEventPublisher.Raise(oComponent[uiIndex]->GetTextName().c_str());

						oComponent[uiIndex]->Remove();
					}
				}
			}

			++entityIt;
		}

		++cIt;
	}
}

void SystemManager::RemoveTemplateComponents()
{
	ComponentTable::iterator cIt = m_vComponentTable.begin();
	while(cIt != m_vComponentTable.end())
	{
		EntityComponentTable& entityComponentMap= cIt->second;
		EntityComponentTable::iterator entityIt = entityComponentMap.begin();
		while(entityIt != entityComponentMap.end())
		{
				MGDVector<Component*>& oComponent = (*entityIt).second;
				for(uint32 uiIndex = 0; uiIndex < oComponent.size(); ++uiIndex)
				{
					if(!oComponent[uiIndex]->IsRemovable() && oComponent[uiIndex]->IsCreatedFromTemplate())
					{
						char pEventName[128];
						sprintf_s(pEventName, "Unregistered/%s", oComponent[uiIndex]->GetID().GetDebugName().c_str());
						m_oEventPublisher.SetPath(ObjectId(pEventName));
						m_oEventPublisher.Raise(oComponent[uiIndex]->GetTextName().c_str());

						oComponent[uiIndex]->Remove();
					}
				}

			++entityIt;
		}

		++cIt;
	}
}


void SystemManager::AddTemplate(const ObjectId& i_oTemplateId, Component* i_pComponent)
{
	MGD_ASSERT(i_pComponent);
	if (i_pComponent)
	{
		m_vTemplateTable[i_oTemplateId].push_back(i_pComponent);
		MGD_LOG::LOGManager::GetSingleton().WriteLog(MGD_LOG::MGD_INFO, MANAGER_CONTEXT, "Template component added: %s Template actor name: %s", i_pComponent->GetTextName().c_str(), i_pComponent->GetOwnerID().GetDebugName().c_str());
	}
}

void SystemManager::RemoveTemplate(const ObjectId& i_oTemplateId)
{
	EntityComponentTable::iterator it = m_vTemplateTable.find(i_oTemplateId);
	if (it != m_vTemplateTable.end())
	{
		for (uint32 uiIndex = 0; uiIndex < (*it).second.size(); ++uiIndex)
		{
			(*it).second[uiIndex]->Remove();
		}
	}
}

void SystemManager::GetComponentsFromTemplate(const ObjectId& i_oTemplateId, MGDVector<Component*>& i_vComponents)
{
	EntityComponentTable::iterator it = m_vTemplateTable.find(i_oTemplateId);
	if (it != m_vTemplateTable.end())
	{
		i_vComponents.clear();
		for (uint32 uiIndex = 0; uiIndex < (*it).second.size(); ++uiIndex)
		{
			i_vComponents.push_back((*it).second[uiIndex]);
		}
	}
	
	MGD_ASSERT(i_vComponents.size() > 0 && "This template doesn't exist!");
}

EntityComponentTable* SystemManager::EditComponentTable(const ObjectId& i_oComponentId)
{
	ComponentTable::iterator cIt= m_vComponentTable.find(i_oComponentId);
	if(cIt != m_vComponentTable.end())
	{
		return &cIt->second;
	}
	return NULL;
}

const EntityComponentTable* SystemManager::GetComponentTable(const ObjectId& i_oComponentId) const
{
	ComponentTable::const_iterator cIt= m_vComponentTable.find(i_oComponentId);
	if(cIt != m_vComponentTable.end())
	{
		return &cIt->second;
	}
	return NULL;
}

Component* SystemManager::EditComponent(const ObjectId& i_oComponentId, const ObjectId& i_oComponentName, const ObjectId& i_oOwnerId)
{
	EntityComponentTable* ect= EditComponentTable(i_oComponentId);
	if(ect!=NULL)
	{
		EntityComponentTable::iterator ectIt= ect->begin();
		for(; ectIt != ect->end(); ++ectIt)
		{
			if(ectIt->first == i_oOwnerId)
			{
				MGDVector<Component*>& vComponents = ectIt->second;
				MGDVector<Component*>::iterator itComponent = vComponents.begin();
				for(; itComponent != vComponents.end(); ++itComponent)
				{
					if((*itComponent)->GetName() == i_oComponentName)
					{
						return (*itComponent);
					}
				}
			}			
		}
	}
	return NULL;
}

const Component* SystemManager::GetComponent( const ObjectId& i_oComponentId, const ObjectId& i_oComponentName, const ObjectId& i_oOwnerId )
{
	const EntityComponentTable* ect= GetComponentTable(i_oComponentId);
	if(ect!=NULL)
	{
		EntityComponentTable::const_iterator ectIt= ect->begin();
		for(; ectIt != ect->end(); ++ectIt)
		{
			if(ectIt->first == i_oOwnerId)
			{
				const MGDVector<Component*>& vComponents = ectIt->second;
				MGDVector<Component*>::const_iterator itComponent = vComponents.begin();
				for(; itComponent != vComponents.end(); ++itComponent)
				{
					if((*itComponent)->GetName() == i_oComponentName)
					{
						return (*itComponent);
					}
				}
			}			
		}
	}
	return NULL;
}

MGDVector<Component*>* SystemManager::EditComponent(const ObjectId& i_oComponentId, const ObjectId& i_oOwnerId)
{
	EntityComponentTable* ect= EditComponentTable(i_oComponentId);
	if(ect!=NULL)
	{
		EntityComponentTable::iterator ectIt= ect->begin();
		for(; ectIt != ect->end(); ++ectIt)
		{
			if(ectIt->first == i_oOwnerId)
			{
				return &(ectIt->second);
			}			
		}
	}
	return NULL;
}

const MGDVector<Component*>* SystemManager::GetComponent(const ObjectId& i_oComponentId, const ObjectId& i_oOwnerId) const
{
	const EntityComponentTable* ect = GetComponentTable(i_oComponentId);
	if(ect != NULL)
	{
		EntityComponentTable::const_iterator ectIt= ect->begin();
		for(; ectIt != ect->end(); ++ectIt)
		{
			if(ectIt->first == i_oOwnerId)
			{
				return &(ectIt->second);			
			}			
		}
	}

	return NULL;
}

Component* SystemManager::EditSingleComponent(const ObjectId& i_oComponentId, const ObjectId& i_oOwnerId)
{
	EntityComponentTable* ect= EditComponentTable(i_oComponentId);
	if(ect!=NULL)
	{
		EntityComponentTable::iterator ectIt= ect->begin();
		for(; ectIt != ect->end(); ++ectIt)
		{
			if(ectIt->first == i_oOwnerId)
			{
				MGD_ASSERT(ectIt->second.size() == 1 && "More than one component of this type in the actor!");
				if(ectIt->second.size() > 0)
				{
					return ectIt->second[0];
				}
			}			
		}
	}
	return NULL;
}

const Component* SystemManager::GetSingleComponent(const ObjectId& i_oComponentId, const ObjectId& i_oOwnerId) const
{
	const EntityComponentTable* ect = GetComponentTable(i_oComponentId);
	if(ect != NULL)
	{
		EntityComponentTable::const_iterator ectIt= ect->begin();
		for(; ectIt != ect->end(); ++ectIt)
		{
			if(ectIt->first == i_oOwnerId)
			{
				MGD_ASSERT(ectIt->second.size() == 1);
				if(ectIt->second.size() > 0)
				{
					return ectIt->second[0];			
				}
			}			
		}
	}

	return NULL;
}

void SystemManager::CheckRemovableComponents()
{
	EntityComponentTable::iterator it = m_vTemplateTable.begin();
	while(it != m_vTemplateTable.end())
	{
		MGDVector<Component*>::iterator itComp = (*it).second.begin();
		while(itComp != (*it).second.end())
		{
			if((*itComp)->IsRemovable())
			{
				delete((*itComp));
				itComp = (*it).second.erase(itComp);
			}
			else
			{
				++itComp;
			}
		}
		if((*it).second.empty())
		{
			it = m_vTemplateTable.erase(it);
		}
		else
		{
			++it;
		}
	}

	ComponentTable::iterator itComponentTable = m_vComponentTable.begin();
	while(itComponentTable != m_vComponentTable.end())
	{
		EntityComponentTable& vEntityTable = (*itComponentTable).second;
		EntityComponentTable::iterator itEntity = vEntityTable.begin();
		while(itEntity != vEntityTable.end())
		{
			MGDVector<Component*>& vComponentList = (*itEntity).second;
			MGDVector<Component*>::iterator itComponent = vComponentList.begin();
			while(itComponent != vComponentList.end())
			{
				if((*itComponent)->IsRemovable())
				{
					MGD_LOG::LOGManager::GetSingleton().WriteLog(MGD_LOG::MGD_INFO, MANAGER_CONTEXT, "Component removed: %s Actor: %s", (*itComponent)->GetTextName().c_str(), (*itComponent)->GetOwnerID().GetDebugName().c_str());

					// ********************** LEAK *************************
					delete((*itComponent));
					// ****************************************************
					//delete((*it2).first);
					itComponent = vComponentList.erase(itComponent);
				}
				else
				{
					++itComponent;
				}
			}


			//If we don't have other components for this entity we remove it from the list
			if(vComponentList.empty())
			{
				itEntity = vEntityTable.erase(itEntity);
			}
			else
			{
				++itEntity;
			}
		}

		++itComponentTable;
	}
}

void SystemManager::AddSystem(System* i_pSystem)
{
	MGD_ASSERT(i_pSystem);
	if(i_pSystem)
	{
		i_pSystem->Init();

		MGD_ASSERT(i_pSystem->GetUpdatePhase() < m_uiMaxPhaseCount);
		SystemTable& systemMap= m_vPerPhaseSystemArray[i_pSystem->GetUpdatePhase()];
		systemMap[i_pSystem->GetID()] = i_pSystem;
	}
}

System* SystemManager::EditSystem(const ObjectId& i_oSystemId, uint8 i_uiPhase)
{
	uint8 startIndex= 0;
	uint8 endIndex= m_uiMaxPhaseCount;
	if(i_uiPhase < m_uiMaxPhaseCount)
	{
		startIndex= i_uiPhase;
		endIndex= startIndex + 1; 
	}

	for(uint8 i= startIndex; i < endIndex; ++i)
	{
		SystemTable& systemMap= m_vPerPhaseSystemArray[i];
		SystemTable::iterator smIt= systemMap.find(i_oSystemId);
		if(smIt != systemMap.end())
			return smIt->second;
	}
	return NULL;
}

const System* SystemManager::GetSystem(const ObjectId& i_oSystemId, uint8 i_uiPhase) const
{
	uint8 startIndex= 0;
	uint8 endIndex= m_uiMaxPhaseCount;
	if(i_uiPhase < m_uiMaxPhaseCount)
	{
		startIndex= i_uiPhase;
		endIndex= startIndex + 1; 
	}

	for(uint8 i= startIndex; i < endIndex; ++i)
	{
		const SystemTable& systemMap= m_vPerPhaseSystemArray[i];
		SystemTable::const_iterator smIt= systemMap.find(i_oSystemId);
		if(smIt != systemMap.end())
			return smIt->second;
	}
	return NULL;
}

void SystemManager::Update(real i_fFrametime, real i_fTimestep)
{
	for(uint8 i= 0; i< m_vPerPhaseSystemArray.size(); ++i)
	{
		SystemTable& systemTable= m_vPerPhaseSystemArray[i];
		for(SystemTable::iterator smIt= systemTable.begin(); smIt != systemTable.end(); ++smIt)
		{
			smIt->second->Update(i_fFrametime, i_fTimestep);
		}
	}

	CheckRemovableComponents();
}

void SystemManager::SetPause(bool i_bPause)
{
	m_bPause= i_bPause;
}

bool SystemManager::IsInPause() const
{
	return m_bPause;
}

void SystemManager::RemoveAllComponents()
{
	ComponentTable::iterator cIt = m_vComponentTable.begin();
	while(cIt != m_vComponentTable.end())
	{
		EntityComponentTable& entityComponentMap= cIt->second;
		EntityComponentTable::iterator entityIt = entityComponentMap.begin();
		while(entityIt != entityComponentMap.end())
		{
			MGDVector<Component*>& oComponent = (*entityIt).second;
			for(uint32 uiIndex = 0; uiIndex < oComponent.size(); ++uiIndex)
			{
				if(!oComponent[uiIndex]->IsRemovable())
				{
					char pEventName[128];
					sprintf_s(pEventName, "Unregistered/%s", oComponent[uiIndex]->GetID().GetDebugName().c_str());
					m_oEventPublisher.SetPath(ObjectId(pEventName));
					m_oEventPublisher.Raise(oComponent[uiIndex]->GetTextName().c_str());
					
					oComponent[uiIndex]->Remove();
				}
			}

			++entityIt;
		}

		++cIt;
	}
}
