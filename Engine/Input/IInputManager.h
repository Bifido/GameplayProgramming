#pragma once
#include "..\Common\DataTypes.h"

class IInputManager
{
public:
	IInputManager()
		: m_bShoutdown(false)
	{

	}
	virtual ~IInputManager() {}

	virtual void Update(real i_fFrametime, real i_fTimestep) = 0;

	bool IsShutdown() const
	{
		return m_bShoutdown;
	}

	void OnExit()
	{
		m_bShoutdown = true;
	}

	MGDVector<int32>				m_KeyButtonDown;

private:
	bool m_bShoutdown;
};

