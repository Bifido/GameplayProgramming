#include "System.h"

ID_IMPLEMENTATION(System);

System::System(uint8 i_uiUpdatePhase)
	:m_uiUpdatePhase(i_uiUpdatePhase)
{

}

System::~System() 
{

}

uint8 System::GetUpdatePhase() const
{
	return m_uiUpdatePhase;
}
