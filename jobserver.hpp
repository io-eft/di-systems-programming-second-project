#ifndef __JOBSERVER__HPP
#define __JOBSERVER__HPP
#include "customlist.hpp"
#include "myfunctions.hpp"
#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sstream>
#include <string.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>

class JobServer{
    int concurrency;
    int jobCount;
    CustomList queuedJobs;
    CustomList activeJobs;

    public:
	JobServer();
	
	void setConcurrency(int a);
	int getConcurrency();
	void initialize();
	void terminate();
	std::string getCommand();
	void executeCommand(std::string s);
	void addNewJob();
	MyJob* getJob(std::string s);
	void sendResponse(MyJob* mj);
	void sendResponse(int i);
	void run(int flag, int* jobsDone);
	int getNumberOfJobs();
};


#endif
