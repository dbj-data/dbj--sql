'
' https://www.freebasic.net/forum/viewtopic.php?t=14804
'
#include "crt/stdlib.bi"
#include "crt/time.bi"
#include "sqlite3.bi"

#define TABLESIZE 5000

Dim Shared err_ As Integer

#macro sql(database,text)
  err_ = sqlite3_exec(database,text,NULL,NULL,NULL)
#endmacro

Sub main()
 
  Dim As clock_t t1
  Dim As clock_t t2
 
 
  Dim table(0 To TABLESIZE) As UInteger
  Dim As Integer success
  Dim As sqlite3 Ptr dbb
  Dim As ZString Ptr s = @"INSERT INTO hash_table VALUES(?,?)"
  Dim As ZString Ptr t = @"SELECT value FROM hash_table WHERE key = ?"
  Dim As sqlite3_stmt Ptr stmt
  Dim As sqlite3_stmt Ptr stmt2
 
  srand(clock())
  t1 = clock()
  err_ = sqlite3_open("hash.db",@dbb)
  sql(dbb,"BEGIN TRANSACTION;") 
  sql(dbb,"CREATE TABLE hash_table(key INTEGER,value INTEGER);")
  sql(dbb,"CREATE INDEX idx_hashtable ON hash_table(key);")
  sql(dbb,"COMMIT;")
  sql(dbb,"BEGIN TRANSACTION;")
  err_ = sqlite3_prepare(dbb,s,Len(*s),@stmt,NULL)
  err_ = sqlite3_prepare(dbb,t,Len(*t),@stmt2,NULL)
  For i As Integer = 0 To TABLESIZE
     Var key = rand() Mod 65536
     err_ = sqlite3_bind_int(stmt,1,key)
     err_ = sqlite3_bind_int(stmt,2,key+100)
     err_ = sqlite3_step(stmt)
     err_ = sqlite3_reset(stmt)      
     table(i) = key
  Next i 
  err_ = sqlite3_finalize(stmt)
  sql(dbb,"COMMIT;")   
  t2 = clock()   
  Print "sqlite insert start : ";t1;" sqlite insert end : ";t2   
 
 
Dim As UInteger value   
  t1 = clock()
  For i As UInteger = 0 To TABLESIZE
    err_ = sqlite3_bind_int(stmt2,1,table(i))
    err_ = sqlite3_step(stmt2)
    value = sqlite3_column_int(stmt2,0) 
    err_ = sqlite3_reset(stmt2)
  Next i
  err_ = sqlite3_finalize(stmt2)
  err_ = sqlite3_close(dbb)
  t2 = clock()   
  Print "sqlite query start: ";t1;" sqlite query end: ";t2   
End Sub

main()

