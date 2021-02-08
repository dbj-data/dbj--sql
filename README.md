<h1> DBJ  SQLite Hash Table Research </h1>

 &copy; 2017 - 2021 by dbj@dbj.org &amp; [dbj.systems uk ltd](https://dbj.systems/).

 Licence -- https://dbj.org/license_dbj

---------------------------------------------------------------------  

## Why are you here?

The idea in here is to use the SQLite in memory database for Hash Tables implementations. If interested you are very welcome.

This folder is ISO C code only.


## Dive in

The environment

- Windows 10 Pro x64 
- Clone the [MACHINE_WIDE](https://github.com/dbj-data/machine_wide) to your D: drive.
  - Hint: Use `subst`  to "make" D: drive if you do not have one
  - SQLite 3 is part of that too
- For SQLite databases management we recommend [SQLite Studio](https://youtu.be/dugUk893gxQ) 
- Please install Visual Studio 2019 with Clang
- There are three VStudio projects in this folder
  -  each is best experienced by following it through the VStudio debugger

## What is what 


| File  | Description
|-------|--------------
| benchmarking.c | This is from where the actual original work has started. Study that one first.
| dbhash.c | Sampler. Hahshing the whole database. Taken from SQLite dev tree.
| sha1.c | SQLite "extension" sampler. `sha1` hash function. Taken from SQLite dev tree.


## On The Road

> aka "the Roadmap"

For full functionality one needs an "handle" (just a "number") to interface with hash tables feasibly. Unlike pointers, handles can be checked if they are valid. Handle is implemented as a result of the hash function

```cpp
// general unique values HT storing algorithm
auto handle = hash_function(value_to_store) ;
if ( ! already_stored(handle) )
      store(value_to_store) ;
//to the caller
return handle;
// there are other big issues arround that 
// we will talk later
```

User/caller is given that handle, not some pointer.

General aim is to develop SQLite extension that will aid creating hashtables, which basicaly means several hashing functions:

- [Paul Hsieh](https://gist.github.com/CedricGuillemet/4978020)
- [Jenkins](https://burtleburtle.net/bob/c/lookup3.c)
- [there are others](https://en.wikipedia.org/wiki/List_of_hash_functions#Non-cryptographic_hash_functions) although  it is a moot point why would they be used

### So far

So good. SQLite HT (in memory) seems rather fast. hash functions are not used, yet. Keep in mind performance is not the only sign of quality. There is also a question of space aka size.

Even without testing one can be reasonably sure SQLite supported HT implementation can store gigabytes of strings or general data. Persistent and in a very safe manner too. Put a "micro service" in front of that and "you can fly".

This is far from finished and documented. Stay tuned.

-------------------------------------

Contact : [info@dbj.systems](mailto:info@dbj.systems) | [Skype](skype:live:dbj.systems?chat)

[![dbj();](http://dbj.org/wp-content/uploads/2015/12/cropped-dbj-icon-e1486129719897.jpg)](http://www.dbj.org "dbj")  

