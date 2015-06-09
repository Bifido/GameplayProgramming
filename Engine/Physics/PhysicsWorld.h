#pragma once

#include "../Common/Utilities.h"
#include "../Physics/PhysicsObject.h"

class PhysicsWorld;
class PhysicsComponent;

class PhysicsWorldFactory
{
public:

	virtual ~PhysicsWorldFactory() { }

	virtual PhysicsWorld* CreateWorld() const = 0;
}; 

class PhysicsWorld
{
public:
	PhysicsWorld()
		: m_bDebugDraw(false)
		, m_fGravity(9.8f)
	{

	}

	virtual ~PhysicsWorld() {}

	virtual void Configure()	= 0;
	virtual void Cleanup()		= 0;

	virtual void AddObject(const PhysicsObject* i_object)		= 0;
	virtual void RemoveObject(const PhysicsObject* i_object)	= 0;
	virtual void RemoveAllObjects()								= 0;

	virtual void Step(const real i_dt)							= 0;
	virtual void PostStep()										= 0;

protected:
	bool m_bDebugDraw;
	real m_fGravity;
}; 