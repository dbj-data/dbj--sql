/*'
* transformed from:  https://www.freebasic.net/forum/viewtopic.php?select_statement=14804
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include "..\sqlite3\sqlite3.h"

static bool is_sqlite_err(int specimen_)
{
return ( 
    ((specimen_ == SQLITE_OK) || (specimen_ == SQLITE_ROW) || (specimen_ == SQLITE_DONE))
    ? false 
    : true 
    );
}

enum { TABLESIZE = 5000 } ;

// Shared err_ As Integer
static int err_ = 0;

//#macro sql(database, text)
//TRY(sqlite3_exec(database, text, NULL, NULL, NULL)
//#endmacro

#define sql(database, text) (TRY(sqlite3_exec(database, text, NULL, NULL, NULL), assert(! is_sqlite_err(err_)))

#define TRY(X) do { \
int errc = (X) ;\
assert(! is_sqlite_err(errc) ; \
} while(0)

// Sub main()
int main(void)
{

    //Dim As clock_t t1
    //Dim As clock_t t2
    clock_t t1, t2;

    //        Dim table(0 To TABLESIZE) As UInteger
    int table[TABLESIZE] = {0};
    // Dim As Integer success
    int success = 0;
    //    Dim As sqlite3 Ptr dbb
    sqlite3 *dbb = NULL;
    //Dim As ZString Ptr insert_statement = @"INSERT INTO hash_table VALUES(?,?)"
    //Dim As ZString Ptr select_statement = @"SELECT value FROM hash_table WHERE key = ?"
#define insert_statement "INSERT INTO hash_table VALUES(?,?)"
#define select_statement "SELECT value FROM hash_table WHERE key = ?"

    //Dim As sqlite3_stmt Ptr stmt
    //Dim As sqlite3_stmt Ptr stmt2
    sqlite3_stmt *stmt = NULL;
    sqlite3_stmt *stmt2 = NULL;

    srand(clock());
    t1 = clock();
    TRY(sqlite3_open(":memory:", &dbb));

    sql(dbb, "BEGIN TRANSACTION;");
    sql(dbb, "CREATE TABLE hash_table(key INTEGER,value INTEGER);");
    sql(dbb, "CREATE INDEX idx_hashtable ON hash_table(key);");
    sql(dbb, "COMMIT;");

    sql(dbb, "BEGIN TRANSACTION;");
    TRY(sqlite3_prepare(dbb, insert_statement, strlen(insert_statement), &stmt, NULL);

    for (int i = 0; i < TABLESIZE; ++i)
    {
        int key = rand() % 65536;
        TRY(sqlite3_bind_int(stmt, 1, key));
        TRY(sqlite3_bind_int(stmt, 2, key + 100));
        TRY(sqlite3_step(stmt));
        TRY(sqlite3_reset(stmt));
        table[i] = key;
    }
    TRY(sqlite3_finalize(stmt));
    sql(dbb, "COMMIT;");
    t2 = clock();

    printf("\nsqlite insert : %d ", (t2 - t1) / CLOCKS_PER_SEC);

    TRY(sqlite3_prepare(dbb, select_statement, strlen(select_statement), &stmt2, NULL);
    int value = 0;
    t1 = clock();
    for (int i = 0; i < TABLESIZE; ++i)
    {
        TRY(sqlite3_bind_int(stmt2, 1, table[i]));
        TRY(sqlite3_step(stmt2);
        value = sqlite3_column_int(stmt2, 0));
        TRY(sqlite3_reset(stmt2));
    }
    TRY(sqlite3_finalize(stmt2));
    TRY(sqlite3_close(dbb));
    t2 = clock();
    printf("\nsqlite select : %d ", (t2 - t1) / CLOCKS_PER_SEC);

} // main()
