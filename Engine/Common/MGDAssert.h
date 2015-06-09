/*
*	Assert used in this engine
*/

#pragma once
//#include <windows.h>
#include <stdio.h>

bool ReportAssert(const char* i_filename, const int i_line, const char* i_function, const char* i_condition);

#define ASSERT_INFO(i_file, i_line, i_function, x) \
{	\
	bool failure = ((x)==0); \
	if (failure) \
	{ \
		if (ReportAssert(i_file, i_line, i_function, #x))	\
		{ DebugBreak(); } \
	} \
}

#define MGD_ASSERT(x) ASSERT_INFO(__FILE__, __LINE__, __FUNCTION__, x)
