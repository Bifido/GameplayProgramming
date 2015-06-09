/*
*	This class contains the logic of transition with the name of 
*	the state target and the name of the action that should occurs to fire this transition
*/

#pragma once
#include "../Common/DataTypes.h"

class Transition
{
public:
	ID_DECLARATION;
	
	Transition(const std::string& i_szName, const StateID& i_oStateTargetID);
	~Transition();	

	const TransitionID& GetName() const 
	{
		return m_oId;
	}

	const StateID& GetStateTargetID() const
	{
		return m_oStateTargetID;
	}

private:
	StateID					m_oStateTargetID;
	std::string				m_szName;
	TransitionID			m_oId;
};

