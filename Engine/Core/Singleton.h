#pragma once
#include "../Common/MGDAssert.h"

/*
	http://scottbilas.com/publications/gem-singleton/
*/

template <typename T> class Singleton
{
	static T* ms_singleton;

public:
	Singleton()
	{
		MGD_ASSERT( !ms_singleton);
		ms_singleton = static_cast <T*> (this);
	}

	~Singleton()
	{ 
		MGD_ASSERT(ms_singleton); 
		ms_singleton = 0; 
	}

	static T& GetSingleton()
	{ 
		MGD_ASSERT(ms_singleton); 
		return *ms_singleton; 
	}

	static T* GetSingletonPtr()
	{ 
		return ms_singleton; 
	}

	static void Release()
	{
		MGD_ASSERT(ms_singleton);
		delete(ms_singleton);
		ms_singleton= NULL;
	}

};

template <typename T> T* Singleton <T>::ms_singleton = 0;
