all: jobCommander jobExecutorServer

jobCommander: jobCommander.o myfunctions.o jobCommanderFunctions.o
	g++ -g -Wall jobCommander.o myfunctions.o jobCommanderFunctions.o -o jobCommander

jobExecutorServer: jobExecutorServer.o jobserver.o customlist.o myjob.o myfunctions.o
	g++ -g -Wall jobExecutorServer.o jobserver.o customlist.o myjob.o myfunctions.o -o jobExecutorServer

jobCommander.o: jobCommander.cpp
	g++ -c -g3 jobCommander.cpp

jobExecutorServer.o: jobExecutorServer.cpp
	g++ -c -g3 jobExecutorServer.cpp

jobCommanderFunctions.o: jobCommanderFunctions.cpp
	g++ -c -g3 jobCommanderFunctions.cpp

myjob.o: myjob.cpp
	g++ -c -g3 myjob.cpp

myfunctions.o: myfunctions.cpp
	g++ -c -g3 myfunctions.cpp

customlist.o: customlist.cpp
	g++ -c -g3 customlist.cpp

jobserver.o: jobserver.cpp myfunctions.cpp
	g++ -c -g3 jobserver.cpp myfunctions.cpp

clean:
	rm -rf *.o jobCommander jobExecutorServer serverPid ServerToCommander CommanderToServer
