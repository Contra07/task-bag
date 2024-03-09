mkdir ./target
chmod a+x run.sh
if [ -e mpi.h ]
then
	mpicxx -Wall -o target/taskbag-`uname -m` taskbag.cpp tbag.cpp
else
	g++ -Wall -o target/taskbag-`uname -m` taskbag.cpp tbag.cpp -lpthread -pthread
fi