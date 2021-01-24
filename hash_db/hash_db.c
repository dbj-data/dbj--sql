/*'
* transformed from:  https://www.freebasic.net/forum/viewtopic.php?select_statement=14804
*/
#ifndef _DEBUG
#ifndef NDEBUG
#define NDEBUG
#endif
#endif // _DEBUG

#ifdef _MSC_VER
#define MICROSOFT_WINDOWS_WINBASE_H_DEFINE_INTERLOCKED_CPLUSPLUS_OVERLOADS 0
#endif

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <ubut/ubench.h>

#undef _CRT_STRINGIZE_
#define _CRT_STRINGIZE_(x) #x
#undef _CRT_STRINGIZE
#define _CRT_STRINGIZE(x) _CRT_STRINGIZE_(x)

// using the lib and dll from the ./bin folder
#include "..\sqlite3\sqlite3.h"

static bool is_sqlite_err(const int specimen_)
{
	return (
		(specimen_ == SQLITE_OK) || (specimen_ == SQLITE_ROW) || (specimen_ == SQLITE_DONE)
			? false
			: true);
}

typedef struct GLOBALS_STRUCT
{
	sqlite3_stmt *insert_stmt;
	sqlite3_stmt *select_stmt;
	sqlite3_stmt *delete_stmt;
	int last_err_;
	/* it depends on the UBENCH how many times some BENCH will be called */
	size_t inserts_counter;
	size_t deletions_counter;
} GLOBALS_STRUCT;

static GLOBALS_STRUCT globals_ =
	{.delete_stmt = NULL, .insert_stmt = NULL, .select_stmt = NULL, .last_err_ = SQLITE_OK, 
	.inserts_counter = 0, .deletions_counter = 0
	};

#define TRY(EXP_)                                            \
	do                                                       \
	{                                                        \
		globals_.last_err_ = (EXP_);                             \
		if (is_sqlite_err(globals_.last_err_))                   \
		{                                                    \
			puts("\nline: " _CRT_STRINGIZE(__LINE__));       \
			puts("sqlite call: " #EXP_ "\nreturned error:"); \
			puts(sqlite3_errstr(globals_.last_err_));            \
			perror("system ");                               \
			exit(0);                                         \
		}                                                    \
	} while (0)

#define sql(database, text) TRY(sqlite3_exec(database, text, NULL, NULL, NULL))

static sqlite3 *init_setup()
{
	static sqlite3 *dbb_ = NULL;

	if (dbb_ != NULL)
		return dbb_;

	// on Windows + using cl or clang-cl
	// be sure /utf-8 switch is in use!
	sqlite3_open(":memory:", &dbb_);

	sql(dbb_, "BEGIN TRANSACTION;");
	sql(dbb_, "CREATE TABLE hash_table(key INTEGER PRIMARY KEY AUTOINCREMENT, value TEXT);");
	sql(dbb_, "CREATE INDEX idx_hashtable ON hash_table(key);");
	sql(dbb_, "COMMIT;");

// key == NULL because key is INTEGER PRIMARY KEY AUTOINCREMENT
// must read: https://www.sqlite.org/autoinc.html
#define insert_statement "INSERT INTO hash_table VALUES(NULL,?)"
#define select_statement "SELECT value FROM hash_table WHERE key = ?"
#define delete_statement "DELETE FROM hash_table WHERE key = ?"

	TRY(sqlite3_prepare(dbb_, insert_statement, strlen(insert_statement), &globals_.insert_stmt, NULL));
	TRY(sqlite3_prepare(dbb_, select_statement, strlen(select_statement), &globals_.select_stmt, NULL));
	TRY(sqlite3_prepare(dbb_, delete_statement, strlen(select_statement), &globals_.delete_stmt, NULL));

#undef insert_statement
#undef select_statement
#undef delete_statement

	srand((unsigned int)time((time_t*)0)); // will need rand()

	return dbb_;
}

static void close_and_clean(void)
{
	TRY(sqlite3_finalize(globals_.insert_stmt));
	TRY(sqlite3_finalize(globals_.select_stmt));

	(void)sqlite3_close(init_setup());

	printf("\nThere were %zu, inserts", (globals_.inserts_counter + globals_.deletions_counter));
	printf("\nThere were %zu, deletions\n", globals_.deletions_counter);
}

static void hash_db_populate(sqlite3 *dbb)
{
// this works with ot without BEGIN/COMMIT
// without is only slightly faster
#ifdef INSERTION_IS_TRANSACTION
	sql(dbb, "BEGIN TRANSACTION;");
#endif
	char buffy_[0xF] = {'\0'};
	#ifndef _MSC_VER
	int snprintf_rez_ = snprintf(buffy_, 0xF, "%zu", globals_.inserts_counter + 1000);
	#else
	int snprintf_rez_ = _snprintf_s(buffy_, 0xF, 0xF, "%zu", globals_.inserts_counter + 1000);
	#endif
	// must read: https://www.sqlite.org/autoinc.html
	TRY(sqlite3_bind_text(
		globals_.insert_stmt, 1, buffy_,
		snprintf_rez_ + 1 /*strnlen_s(buffy_, 0xF)*/,
		SQLITE_STATIC));
	TRY(sqlite3_step(globals_.insert_stmt));
	TRY(sqlite3_reset(globals_.insert_stmt));

#ifdef INSERTION_IS_TRANSACTION
	sql(dbb, "COMMIT;");
#endif
	/*
    we could do it this way:

	globals_.keys_table[ globals_.inserts_counter % keys_table_max ] = sqlite3_last_insert_rowid( dbb ) ;
	
	but that is a genuine non inlined call and that is expensive.

	obviously that will be required for hash table interfaces, as that **is** the unique ID
	managed by sqlite3 for us
*/
	globals_.inserts_counter += 1;
}

// notice we do not need pointer to the sqlite3 here
static void hash_db_select(void)
{
	// generate the key randomly
	int key = rand() % globals_.inserts_counter;
	// NOTE: ROWID is int but that key above might be not in the table
	// as sqlite manages the rowid increment upon insertion
	// it is sometimes not simple +1 increment
	// hint: that is because of deleted rows
	TRY(sqlite3_bind_int(globals_.select_stmt, 1, key));
	TRY(sqlite3_step(globals_.select_stmt));
	// if we have generated the "wrong" key the value_ will be NULL, that's all
	char *value_ = (char *)sqlite3_column_text(globals_.select_stmt, 0);
	(void)value_ ;
	TRY(sqlite3_reset(globals_.select_stmt));
}

//
static void hash_db_delete(sqlite3 *dbb)
{

	// start from the end
	int key = globals_.inserts_counter, rows_modified = 0;

	rows_modified = 0;
	char buf[0xFF] = {0};
	(void)snprintf(buf, 0xFF, "DELETE FROM hash_table WHERE key = %d;", key);
	sql(dbb, buf);
	rows_modified = sqlite3_changes(dbb);

	if (rows_modified > 0)
	{
		globals_.inserts_counter -= 1;
		globals_.deletions_counter += 1;
	}
}

UBENCH(dbj_sqlite_hash, populate)
{
	hash_db_populate(init_setup());
}

UBENCH(dbj_sqlite_hash, select)
{
	hash_db_select();
}

UBENCH(dbj_sqlite_hash, delete)
{
	hash_db_delete(init_setup());
}

// using the sqlite3.exec is approx 7 times slower
// vs the  hash_db_create_populate();
#if 0
UBENCH(dbj_sqlite_hash, slow_populate)
{
	sqlite3* dbb = init_setup();

	char sql_insert[0xFF] = { 0 };

	int key = rand() % TABLESIZE;
	snprintf(sql_insert, 0xFF, "INSERT INTO hash_table VALUES(%d,%d)", key, key + 100);
	sql(dbb, sql_insert);
	globals_.keys_table[i] = key;
}
#endif // 0

UBENCH_STATE ; // note there is no "()" !

int main(int argc, const char *const argv[])
{
	(void)init_setup();
	int retval = ubench_main(argc, argv);
	close_and_clean();
	return retval;
}