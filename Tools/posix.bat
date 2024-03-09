echo on

copy "Misc\RTL\Posix\tbag.h" "..\tbag.h"
copy "Misc\RTL\Posix\tbag.cpp" "..\tbag.cpp"

copy "Misc\RTL\Posix\pthread.h" "..\pthread.h"
copy "Misc\RTL\Posix\sched.h" "..\sched.h"
copy "Misc\RTL\Posix\semaphore.h" "..\semaphore.h"

del "..\mpi.h"
del "..\mpicxx.h"
del "..\mpio.h"
 
rem dont take 'taskbag.cpp'
