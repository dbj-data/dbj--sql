
## Using SQLite for Hash Table implementation

Specificaly for implementing the storage of unique strings.

### Encrypt the whole database or schema 
dbhash.c is from the SQLite dev tree : https://www.sqlite.org/src/file?name=tool/dbhash.c

## SHA1 is SQLite User Defined Functions (UDF)

This is also an SQLite extension.

File ext/misc/sha1.c is from the [latest check-in](https://www.sqlite.org/src/file/ext/misc/sha1.c)


### Building

I find it most feasile to build one file C/C++ projects like these in here, using VSCode.
Thus this is not part of the VS solution.

- To build make sure sqlite3.lib is on the path.
- To run make sure sqlite3.dll is on the path.
- on our builds DBJ_SQLITE3 env var points to where they both are
- it is also added to the path


| Contact | &nbsp;
|---------|----------
|[Email](mailto:info@dbj.systems) | [![dbj();](https://dbj.org/wp-content/uploads/2015/12/cropped-dbj-icon-e1486129719897.jpg)](http://www.dbj.org "dbj")
|[Skype](skype:live:dbj.systems?chat) |  

