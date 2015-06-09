/*
*	This class is used to contains all the components that define an object in the game.
*/

#pragma once
#include "../Core/Component.h"
#include "../Manager/SystemManager.h"
#include "../Common/DataTypes.h"

#if defined(_DEBUG)
	static uint32 NumActors = 0;
	static uint32 NumComponents = 0;
#endif

class Actor
{
public:
	Actor(ObjectId i_oName);

	virtual ~Actor();

	const MGDVector<Component*>& GetComponents() const;

	void AddComponent(Component* i_pComponent);

	Component* GetComponent( const ObjectId& i_oComponentID ) const;

	void Init();

	const ObjectId&		GetName() const;
	std::string			GetTextName() const;

private:
	ObjectId				m_oName;
	MGDVector<Component*>	m_Components;
};

inline const MGDVector<Component*>& Actor::GetComponents() const
{
	return m_Components;
}

inline const ObjectId& Actor::GetName() const
{
	return m_oName;
}

inline std::string Actor::GetTextName() const
{
	return m_oName.GetDebugName();
}