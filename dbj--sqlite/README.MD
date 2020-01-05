
# dbj++sql 

#### YAPI (Yet Another API)
C++17 SQLITE(3) for the rest of us

What we think is minimal functionality is implemented. For more we hope you can use this lib as the foundation.

Currently ( 2020 JAN ) this is MSVC code. "Compiler agnostic", that is CLANG and GNU version, will come out next.

> Disclaimer: <br/>
> Immediatly usable in your projects. <br/>
> This is Proof Of Concept (POC). <br/>
> Use at your own risk.<br/>
> As we do.

#### Testing
Is in the separate project called `dbjsqluser`, also part of this Visual Studio 2019 solution.

#### Dependancies
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

Beside C++ std lib, and sqlite3 itself, no other external code is required.

### Support

Writing such a code is [not trivial](https://stackoverflow.com/questions/1853243/do-you-really-write-exception-safe-code/1853769#1853769). 
Perhaps we might help you if you need or want to write such a code. 
Please state your issue on the GitHub, under the "Issues".

More documentation is always in the works, if not all published. For immediate professional help please do mail us.
Please see the contact info on the bottom.

### Exception safety

C++ STD lib code is throwing exceptions. Code we have in here is never throwing exceptions. To signal the status to the callers
we use  `dbj_db_status_type`, please see the `dbj_sql_error_concept.h`

In case of terminating error, we write it to the stderr and call the `exit()`. 

---

Built with Microsoft Visual Studio 2019 and C/C++ Optimizing Compiler Version 19.15.26732.1 or better.


---

#### Copyright 2017,2018,2019,2020 by [dbj.systems uk ltd](https://dbj.systems/).

#### Licence -- [CC BY-SA 4.0](https://creativecommons.org/licenses/by-sa/4.0/)

---------------------------------------------------------------------  

> Contact
> 
> [info@dbj.systems](mailto:info@dbj.systems)
>
> [skype:live:dbj.systems?chat](skype:live:dbj.systems?chat)



[![dbj();](http://dbj.org/wp-content/uploads/2015/12/cropped-dbj-icon-e1486129719897.jpg)](http://www.dbj.org "dbj")  


