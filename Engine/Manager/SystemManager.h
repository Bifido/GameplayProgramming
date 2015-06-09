#pragma once
#include "../Core/Singleton.h"
#include "../Common/DataTypes.h"
#include "../Event/RegistryEventPublisher.h"

class Actor;
class Component;
class System;

typedef MGDMap<ObjectId, MGDVector<Component*> >			EntityComponentTable;	//entityId -> list of intances of components
typedef MGDMap<ObjectId, EntityComponentTable>				ComponentTable;			//componentId -> EntityComponentTable
typedef MGDMap<ObjectId, System*>							SystemTable;			//systemId -> system

//Sistema per la gestione dei sistemi - componenti ed entità
class SystemManager: public Singleton<SystemManager>
{
public:
	static void CreateInstance(uint16 i_uiMaxEntityCount, uint8 i_uiMaxPhaseCount);
	~SystemManager();
	//Game management
	void						SetPause(bool i_bPause);
	bool						IsInPause() const;

	//Components management
	void						AddComponent(Component* i_pComponent, const ObjectId& i_oOwnerId);

	//aggiunge un componente, al componente è associata un'entità
	void						RemoveComponent(Component* i_pComponent);
	void						RemoveComponent(const ObjectId& i_oComponentName, const ObjectId& i_oOwnerId);
	void						RemoveComponentsByOwnerId(const ObjectId& i_oOwnerId);
	void						RemoveTemplateComponents();

	//template
	void AddTemplate(const ObjectId& i_oTemplateId, Component* i_pComponent);
	void RemoveTemplate(const ObjectId& i_oTemplateId);
	void GetComponentsFromTemplate(const ObjectId& i_oTemplateId, MGDVector<Component*>& i_vComponents);

	//restituisce una tabella con le entità che possiedono il componente con id specificato
	EntityComponentTable*		EditComponentTable(const ObjectId& i_oComponentId);
	const EntityComponentTable*	GetComponentTable(const ObjectId& i_oComponentId) const;
	
	//restituisce, se esiste, il componente di una data entità
	Component*								EditComponent(const ObjectId& i_oComponentId, const ObjectId& i_oComponentName, const ObjectId& i_oOwnerId);
	MGDVector<Component*>*					EditComponent(const ObjectId& i_oComponentId, const ObjectId& i_oOwnerId);
	Component*								EditSingleComponent(const ObjectId& i_oComponentId, const ObjectId& i_oOwnerId);

	const Component*						GetComponent(const ObjectId& i_oComponentId, const ObjectId& i_oComponentName, const ObjectId& i_oOwnerId);
	const MGDVector<Component*>*			GetComponent(const ObjectId& i_oComponentId, const ObjectId& i_oOwnerId) const;
	const Component*						GetSingleComponent(const ObjectId& i_oComponentId, const ObjectId& i_oOwnerId) const;

	//Remove components
	void RemoveAllComponents();

	//Systems management
	//aggiunge un sistema 
	void				AddSystem(System* system);

	//restituisce un sistema tramite id
	System*				EditSystem(const ObjectId& i_oSystemId, uint8 i_uiPhase = uint8MAX);
	const System*		GetSystem(const ObjectId& i_oSystemId, uint8 i_uiPhase = uint8MAX) const;

	void				Update(real i_fFrametime, real i_fTimestep);	

private:
	SystemManager(uint16 i_uiMaxEntityCount, uint8 i_uiMaxPhaseCount);

	uint32						GetKeyForMap(const ObjectId& i_oId) const;
	void						CheckRemovableComponents();

	uint16						m_uiMaxEntityCount;
	uint8						m_uiMaxPhaseCount;
	ComponentTable				m_vComponentTable;
	EntityComponentTable		m_vTemplateTable;
	MGDVector<SystemTable>		m_vPerPhaseSystemArray;
	bool						m_bPause;

	RegistryEventPublisher<const char*> m_oEventPublisher;

};
