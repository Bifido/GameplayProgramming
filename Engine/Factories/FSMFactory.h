/*
*	This class creates and destroies state, transition and the whole FSMs from XML files
*/
#pragma once
#include "../Core/Component.h"
#include "../Core/Actor.h"
#include "../Common/DataTypes.h"
#include "../Factories/ActorFactory.h"
#include "../FSM\State.h"
#include "../FSM\Transition.h"

class FSMFactory : public Singleton<FSMFactory>
{
public:
	static void CreateInstance();

	~FSMFactory();

private:
	FSMFactory();

	typedef ObjectId StateID;
	GenericObjectFactory<State,StateID> m_Factory;

	static void LoadFile(const char* i_szFilename);
	static State* CreateState(const tinyxml2::XMLElement* i_pState);
	static Transition* CreateTransition(const tinyxml2::XMLElement* i_pNode);


	static void RemoveFile(const char* i_szFilename);


	void RegisterComponents();
	void RegisterGlobalLuaFunction();
};