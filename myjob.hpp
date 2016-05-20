#ifndef __MYJOBS__HPP
#define __MYJOBS__HPP
#include <iostream>
#include <stdio.h>

class MyJob{
    int pid;
    int id;
    bool status;
    std::string name;
    char** arguments;
    int argc;
    
    public:
	MyJob(int p_id, int jid, std::string n, char** arg, int ac);
	~MyJob();
	
	void setPid(int a);
	void setId(int a);
	void setActiveStatus();

	int getPid();
	int getId();
	bool getStatus();

	std::string getName();
	char** getArguments();
	void print();
	void printToFile(FILE* fp);
	void printPollToFile(FILE* fp);
};

#endif
