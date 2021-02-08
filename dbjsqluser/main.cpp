/*
(c) 2019 by dbj@dbj.org -- CC BY-SA 4.0 -- https://creativecommons.org/licenses/by-sa/4.0/
*/

/*'
/link /LIBAPTH must be properly set
*/

#pragma comment(lib,"sqlite3.lib") 

#include "common.h"

#ifdef DBJ_UBUT_SIMPLE_LOG
// dbj_simple_log requires this in one place
static int dbj_simple_log_setup_ = (DBJ_LOG_DEFAULT_WITH_CONSOLE);
#endif


#include "build_time_stamp.inc"

#include "tests/basic_tests.h"
#include "tests/easy_udf_sample.h"
#include "tests/naked_udf.h"

#pragma warning( push )
#pragma warning( disable: 4100 )
// https://msdn.microsoft.com/en-us/library/26kb9fy0.aspx 

UTEST_STATE;

// struct utest_state_s utest_state = { NULL , 0, 0 };

int main(int argc, const char* const argv[]) {
	return utest_main(argc, argv);
}

#pragma warning( pop ) // 4100