#include "../Common/MGDAssert.h"
#include "../LOG/LogManager.h"

bool ReportAssert(const char* i_filename, const int i_line, const char* i_function, const char* i_condition)
{
	if (i_filename  == NULL) i_filename  = "<Unknown file>";
	if (i_function  == NULL) i_function  = "<Unknown function>";
	if (i_condition == NULL) i_condition = "<Unknown condition>";

	MGD_LOG::LOGManager::GetSingleton().WriteLog(MGD_LOG::MGD_ERROR, ASSERT_CONTEXT, "ASSERT -> File: %s - Func: %s - Condition: %s", i_filename, i_function, i_condition);

	return true;
}