echo on

copy "Misc\RTL\Windows\tbag.h" "..\tbag.h"
copy "Misc\RTL\Windows\tbag.cpp" "..\tbag.cpp"

del "..\pthread.h"
del "..\sched.h"
del "..\semaphore.h"

del "..\mpi.h"
del "..\mpicxx.h"
del "..\mpio.h"
 
rem dont take 'taskbag.cpp'
