/*'
* transformed from:  https://www.freebasic.net/forum/viewtopic.php?select_statement=14804
*/
#ifndef _DEBUG
#ifndef NDEBUG
#define NDEBUG
#endif
#endif // _DEBUG

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <ubut/ubench.h>
#include "..\sqlite3\sqlite3.h"

static bool is_sqlite_err(const int specimen_)
{
    return (
        (specimen_ == SQLITE_OK) || (specimen_ == SQLITE_ROW) || (specimen_ == SQLITE_DONE)
            ? false
            : true);
}

enum
{
    TABLESIZE = 5000
};

#define insert_statement "INSERT INTO hash_table VALUES(?,?)"
#define select_statement "SELECT value FROM hash_table WHERE key = ?"

typedef struct STAT
{
    int keys_table[TABLESIZE];
    sqlite3_stmt *insert_stmt;
    sqlite3_stmt *select_stmt;
    int last_err_;
} STAT;

static STAT stat = {.keys_table = {0}, .insert_stmt = NULL, .select_stmt = NULL, .last_err_ = SQLITE_OK};

// #define sql(database, text) ( stat.last_err_ = sqlite3_exec(database, text, NULL, NULL, NULL), assert(! is_sqlite_err(stat.last_err_)))

#define TRY(EXP_)                                 \
    do                                            \
    {                                             \
        stat.last_err_ = (EXP_);                  \
        if (is_sqlite_err(stat.last_err_))        \
        {                                         \
            puts(_CRT_STRINGIZE(__LINE__));       \
            puts("sqlite error from: " #EXP_);    \
            puts(sqlite3_errstr(stat.last_err_)); \
            perror("system ");                    \
            exit(0);                              \
        }                                         \
    } while (0)

#define sql(database, text) TRY(sqlite3_exec(database, text, NULL, NULL, NULL))

static sqlite3 *init_setup()
{
    static sqlite3 *dbb_ = NULL;

    if (dbb_ != NULL)
        return dbb_;

    sqlite3_open(":memory:", &dbb_);

    sql(dbb_, "BEGIN TRANSACTION;");
    sql(dbb_, "CREATE TABLE hash_table(key INTEGER,value INTEGER);");
    sql(dbb_, "CREATE INDEX idx_hashtable ON hash_table(key);");
    sql(dbb_, "COMMIT;");

    TRY(sqlite3_prepare(dbb_, insert_statement, strlen(insert_statement), &stat.insert_stmt, NULL));
    TRY(sqlite3_prepare(dbb_, select_statement, strlen(select_statement), &stat.select_stmt, NULL));

    return dbb_;
}

static void close_and_clean(void)
{
    sqlite3 *dbb_ = init_setup();
    TRY(sqlite3_finalize(stat.insert_stmt));
    TRY(sqlite3_finalize(stat.select_stmt));
    TRY(sqlite3_close(dbb_));
}

static void hash_db_create_populate(void)
{
    sqlite3 *dbb = init_setup();

    sql(dbb, "BEGIN TRANSACTION;");

    for (int i = 0; i < TABLESIZE; ++i)
    {
        int key = rand() % TABLESIZE;
        TRY(sqlite3_bind_int(stat.insert_stmt, 1, key));
        TRY(sqlite3_bind_int(stat.insert_stmt, 2, key + 100));
        TRY(sqlite3_step(stat.insert_stmt));
        TRY(sqlite3_reset(stat.insert_stmt));
        stat.keys_table[i] = key;
    }
    sql(dbb, "COMMIT;");
}

static void hash_db_select(void)
{
    sqlite3 *dbb = init_setup();
    int value = 0;
    for (int i = 0; i < TABLESIZE; ++i)
    {
        TRY(sqlite3_bind_int(stat.select_stmt, 1, stat.keys_table[i]));
        TRY(sqlite3_step(stat.select_stmt));
        value = sqlite3_column_int(stat.select_stmt, 0);
        TRY(sqlite3_reset(stat.select_stmt));
    }
}

UBENCH(dbj_sqlite_hash, populate) { hash_db_create_populate(); }

UBENCH(dbj_sqlite_hash, select) { hash_db_select(); }

// approx 7 times slower 
// vs the  hash_db_create_populate();
#if 0
UBENCH(dbj_sqlite_hash, slow_populate)
{
    sqlite3 *dbb = init_setup();

    char sql_insert[0xFF] = {0};

    for (int i = 0; i < TABLESIZE; ++i)
    {
        int key = rand() % TABLESIZE;
        snprintf(sql_insert, 0xFF, "INSERT INTO hash_table VALUES(%d,%d)" , key, key + 100);
        sql(dbb,sql_insert);
        stat.keys_table[i] = key;
    }
}
#endif // 0

UBENCH(dbj_sqlite_hash, fast_select) { hash_db_select(); }

UBENCH_STATE; // note there is no ()!

int main(int argc, const char *const argv[])
{
    int retval = ubench_main(argc, argv);
    close_and_clean();
    return retval;
}