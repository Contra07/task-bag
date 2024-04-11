echo on

copy "Misc\RTL\EmulWin\tbag.h" "..\tbag.h"
copy "Misc\RTL\EmulWin\tbag.cpp" "..\tbag.cpp"

del "..\pthread.h"
del "..\sched.h"
del "..\semaphore.h"

del "..\mpi.h"
del "..\mpicxx.h"
del "..\mpio.h"
 
rem dont take 'taskbag.cpp'
