
# dbj++sql 

#### YAPI&trade; (Yet Another API)
C++17 SQLITE(3) for the rest of us

> Disclaimer: <br/>
> Immediatly usable in your projects. <br/>
> This is Proof Of Concept (POC). <br/>
> Use at your own risk.<br/>
> As we do.


What we thought of as minimal functionality is implemented. For more we hope you can use this lib as the foundation. 
Feel free to [contact us](#contact) for the possible help.

Also please stuff the issues tab with bugs; we love to debug.

Currently ( 2021 JAN ) this is MSVC code. 

#### Dependancies

As many other dbj projects this code depends on [MACHINE_WIDE](https://github.com/dbj-data/machine_wide) top level headers and sometimes libraries.
Please firt clone that repo into the folder `d:\machine_wide` and then 
add it to your include directories in Visual Studio. 
The solution in here has it all done for you.

In case you do not have a `D:` drive please do make it with a [SUBST](https://ss64.com/nt/subst.html) command.


All the code in here is the `dbj::sql` namespace. This is done, so that it differs from the `sqlite` namespace we have made in here.

This is a heaqder only library. It depends on SQLITE 3, one source file and one header file.
`sqlite3.c` is is linked in, by the virtue of Visual Studio projects we are using.  But it is in the separate namespace, as done in `sqlite++.h`
```cpp
#ifdef __cplusplus
namespace sqlite{
#endif
/*
here we use the relative path
and sqlite3 static lib

in case you hotly disagree and wish to use the dll
please download and install some sqlite3 built as dll, 
and include its header bellow
*/
#include "../sqlite3_lib/sqlite3.h"

#ifdef __cplusplus
}
#endif
/*
assumption is you know how to use pragma comment lib
in case you might preffer it.
*/
```

#### Testing
Is in the separate project called `dbjsqluser`, also part of this Visual Studio 2019 solution.
We use [our own simplified version](https://github.com/dbj-data/ubut) of UTEST and UBENCH. Yes, we include it through "MACHINE_WIDE".

### Support

Writing this kind of a code is perhaps [not trivial](https://stackoverflow.com/questions/1853243/do-you-really-write-exception-safe-code/1853769#1853769). 
We might help you if you need or want to write such a code. 
Please state your issue on the GitHub, under the "Issues". Or contact us directly.

More documentation is always in the works, if not all published. For immediate professional help please do mail us.
Please see the contact info on the bottom.

### Returns handling policy

We use MS STL verion of C++ STD lib code NOT throwing C++ exceptions. That means
`/D_HAS_EXCEPTIONS=0` on the compiler command line. Again already set for you in the project properties.

Code we have in here is not throwing exceptions. 
To signal the status to the callers
we use  metastate paradigm and its valstat implementation.
Please see the [sqlite_valstat.h](sqlite_valstat.h)

WG21 paper explaining it is [P2192](https://github.com/DBJDBJ/metastate).

In case of terminating error, we write it to the stderr and call the `exit()`. 

---

Built with Microsoft Visual Studio 2019 and C/C++ Optimizing Compiler. We update it regularly.


---

#### Copyright 2017,2018,2019,2020 by [dbj.systems uk ltd](https://dbj.systems/).

#### Licence -- [CC BY-SA 4.0](https://creativecommons.org/licenses/by-sa/4.0/)

---------------------------------------------------------------------  

<span id="contact"></span>

> Contact
> 
> [info@dbj.systems](mailto:info@dbj.systems)
>
> [skype:live:dbj.systems?chat](skype:live:dbj.systems?chat)



[![dbj();](http://dbj.org/wp-content/uploads/2015/12/cropped-dbj-icon-e1486129719897.jpg)](http://www.dbj.org "dbj")  


