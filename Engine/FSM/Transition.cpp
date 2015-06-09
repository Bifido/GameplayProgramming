#include "Transition.h"

ID_IMPLEMENTATION(Transition);

Transition::Transition( const std::string& i_szName, const ObjectId& i_oStateTargetID )
	: m_szName(i_szName)
	, m_oId(i_szName.c_str())
	, m_oStateTargetID(i_oStateTargetID)
{

}


Transition::~Transition(void)
{
}
