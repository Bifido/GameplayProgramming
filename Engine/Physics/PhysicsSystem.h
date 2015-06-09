#pragma once
#include "../Core/System.h"
#include "../Event\RegistryEventHandler.h"

/*
*	This class manages the info in the physic components
*/

class PhysicsWorldFactory;
class PhysicsWorld;

class PhysicsSystem : public System
{
public:
	ID_DECLARATION;

	PhysicsSystem(PhysicsWorldFactory* i_pFactory);
	virtual ~PhysicsSystem();

	virtual void	Init();
	virtual void	Update(real i_fFrametime, real i_fTimestep);

private:
	void NotifyCollisions();

	void CreateObjects(const char* i_szName);
	void DeleteObjects(const char* i_szName);

	PhysicsWorld* m_pPhysicWorld;

	RegistryEventHandler<PhysicsSystem, const char*> m_oRegisterEvent;
	RegistryEventHandler<PhysicsSystem, const char*> m_oUnregisterEvent;

	static const ObjectId PHYSICS_SYSTEM_ID;
};

