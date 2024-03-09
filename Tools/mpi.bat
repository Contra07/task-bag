echo on

copy "Misc\RTL\MPI\tbag.h" "..\tbag.h"
copy "Misc\RTL\MPI\tbag.cpp" "..\tbag.cpp"

del "..\thread.h"
del "..\sched.h"
del "..\semaphore.h"

copy "Misc\RTL\MPI\mpi.h" "..\mpi.h"
copy "Misc\RTL\MPI\mpicxx.h" "..\mpicxx.h"
copy "Misc\RTL\MPI\mpio.h" "..\mpio.h"
 
rem dont take 'taskbag.cpp'
