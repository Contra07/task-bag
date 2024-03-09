echo on

copy "Misc\RTL\Debug\tbag.h" "..\tbag.h"
copy "Misc\RTL\Debug\tbag.cpp" "..\tbag.cpp"

del "..\pthread.h"
del "..\sched.h"
del "..\semaphore.h"

del "..\mpi.h"
del "..\mpicxx.h"
del "..\mpio.h"
 
rem dont take 'taskbag.cpp'
