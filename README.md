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
- Please install VS Code with the C/C++ Extension
- Open VStudio 64 bit command prompt, then open this folder in VS Code
  - But why VS Code, why?
    - It is better for single file based, code study.
    - it makes build properties very easy to change compared to Visual Studio GUI
    - Warning: VS Code is not made for C/C++ projects **NOT** using CMake
    - We do not use CMake and it has taken us days to make this work just by using VSCode
      - We will document that part separately
- There are three C files
  -  each is best experienced by following it through the VS Code debugger

## What is what 


| File  | Description
|-------|--------------
| hash_db.c | Benchmarking. This is from where the actual original work has started. 
| dbhash.c | Hahshing the whole database. Taken from SQLite dev tree.
| sha1.c | SQLite "extension". `sha1` hash function. Taken from SQLite dev tree.


## On The Road

This is far from finished and documented. Stay tuned.

-------------------------------------

Contact : [info@dbj.systems](mailto:info@dbj.systems) | [Skype](skype:live:dbj.systems?chat)

[![dbj();](http://dbj.org/wp-content/uploads/2015/12/cropped-dbj-icon-e1486129719897.jpg)](http://www.dbj.org "dbj")  

