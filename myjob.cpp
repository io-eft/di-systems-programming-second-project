#include "myjob.hpp"

MyJob::MyJob(int p_id, int jid, std::string n, char** arg, int ac): pid(p_id), id(jid), status(false), name(n), argc(ac)
{
    arguments = arg;
}

MyJob::~MyJob()
{
    if(arguments != NULL)
    {
        for(int i = 0; i < argc; i++)
            delete[] arguments[i];
        delete[] arguments;
    }
}

void MyJob::setPid(int a)
{
    pid = a;
}

void MyJob::setId(int a)
{
    id = a;
}

void MyJob::setActiveStatus()
{
    status = true;
}

int MyJob::getPid()
{
    return pid;
}

int MyJob::getId()
{
    return id;
}

bool MyJob::getStatus()
{
    return status;
}

std::string MyJob::getName()
{
    return name;
}

char** MyJob::getArguments()
{
    return arguments;
}

void MyJob::print()
{
    std::cout << "Name: " << name << std::endl;
    std::cout << "JobId: " << id << std::endl;
    std::cout << "Pid: " << pid << std::endl;
    if(status)
        std::cout << "Active!" << std::endl;
    else
        std::cout << "Not Active!" << std::endl;
    for(int i = 0; i < argc; i++)
        std::cout << arguments[i] << " ";
    std::cout << std::endl;
}

void MyJob::printToFile(FILE* fp)
{
    if(status)
        fprintf(fp, "%d %s - Active!\n", id, name.c_str());
    else
        fprintf(fp, "%d %s - Not Active!\n", id, name.c_str());
}

void MyJob::printPollToFile(FILE* fp)
{
    fprintf(fp, "%d %s\n", id, name.c_str());
}
