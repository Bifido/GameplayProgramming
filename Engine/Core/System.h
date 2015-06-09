/*
*	This is the base class for systems
*	System is the manager for a specific component type.
*	This is used to do the update of that type odf component.
*/


#pragma once
#include "../Common/DataTypes.h"
//Un sistema esegue determinate azioni su un componente di riferimento
class System
{
public:
	ID_DECLARATION;

	System(uint8 i_uiUpdatePhase = 0);
	virtual ~System();

	virtual void	Init() = 0;
	virtual void	Update(real i_fFrametime, real i_fTimestep) = 0;

	//restituisce la fase in cui verrà invocata la update
	uint8			GetUpdatePhase() const;
	
private:
	uint8	    m_uiUpdatePhase;
	ObjectId	m_oSystemId;
};