#include "jobserver.hpp"
#include "myfunctions.hpp"

JobServer::JobServer(): concurrency(1), jobCount(1) {}

void JobServer::setConcurrency(int a)
{
    concurrency = a;
}

int JobServer::getConcurrency()
{
    return concurrency;
}

void JobServer::initialize()
{
    std::ofstream data;
    data.open("serverPid"); //create the file with your pid
    if(data.is_open())
    {
        data << getpid() << std::endl;  //put your pid in it
        data.close();   //close the file
    }
    else
    {
        perror("Failed to create serverPid");
        exit(-1);
    }
    if(mkfifo("CommanderToServer", 0777) < 0 || mkfifo("ServerToCommander", 0777) < 0)
    {   //create the 2 fifos
        perror("Failed to create fifos");
        exit(-1);
    }
}

int JobServer::getNumberOfJobs()
{   //get the maximum number of jobs that can end in a single signal
    if(concurrency == activeJobs.length())
    {   //if concurrency and active jobs are equal
        if(concurrency == 0)    //if they're 0
            return 1;   //return 1
        else    //else
            return concurrency; //return the number
    }
    else    //else return the higher
    {
        if(concurrency >= 0 && activeJobs.length() >= 0)
        {
            if(concurrency > activeJobs.length())
                return concurrency;
            else
                return activeJobs.length();
        }
        else
            return 1;   //or 1 if they are both negative
    }
}

void JobServer::terminate()
{
    while(!activeJobs.isEmpty())    //send signal to all current active jobs
    {
        MyJob* mj = activeJobs.getTop();
        kill(mj->getPid(), SIGTERM);
        activeJobs.removeTopEntry();
    }
    unlink("CommanderToServer");    //close the fifos
    unlink("ServerToCommander");
    remove("serverPid");    //and remove the file with the server pid
}

std::string JobServer::getCommand()
{
    std::string s = "";
    FILE* inp;
    inp = fopen("CommanderToServer", "r");  //open the fifo
    if(inp == NULL)
    {
        perror("Failed to open the fifo");
        exit(-1);
    }
    char buff[1024];
    memset(buff, '\0', 1024);
    while(fgets(buff, 1024, inp) > 0)   //while there's still stuff in it
    {
        s += buff;  //add it to the string
    }
    fclose(inp);    //close the fifo
    return s;   //return the string
}

void JobServer::executeCommand(std::string s)
{
    std::stringstream ss;
    std::string line;
    ss << s;
    getline(ss, line, ' '); //get the first argument from the string
    if(line == "issuejob")  //if it's issuejob
    {
        MyJob* mj = getJob(s);  //create the new job
        queuedJobs.insertEntry(mj); //add it in the queued jobs
        while(!queuedJobs.isEmpty() && concurrency > activeJobs.length())
                addNewJob();    //then try to activate it
        sendResponse(mj);   //send the response
        jobCount++; //increase the number of total jobs
        return; //and return
    }
    if(line == "setConcurrency")    //if it's set concurrency
    {
        setConcurrency(getSecondToLastNumber(s));   //change the concurrency
        while(!queuedJobs.isEmpty() && concurrency > activeJobs.length())
            addNewJob();    //try to add more jobs
        return; //return
    }
    if(line == "stop")  //if it's stop
    {   //get the number you want stopped
        int jobid = getSecondToLastNumber(s);
        if(queuedJobs.jobIdExists(jobid))   //check if it's in queued jobs
        {
            queuedJobs.removeEntryByJobId(jobid);   //and remove it
        }
        else    //else
        {
            int pid = activeJobs.getPidByJobId(jobid);  //get it's pid
            if(pid != -1)
            {
                kill(pid, SIGTERM);//send a signal for it to be terminated
                activeJobs.removeEntryByJobId(jobid);   //and remove it from the list
            }
            while(!queuedJobs.isEmpty() && concurrency > activeJobs.length())
                addNewJob();    //then add new jobs to replace that
        }
        return; //and return
    }
    if(line == "poll")
    {   //if it's poll
        getline(ss, line, ' ');
        if(line == "queued")    //find out if it's queued or active
            sendResponse(1);
        else    //and send the appropriate response
            sendResponse(0);
        return; //then return
    }
    if(line == "exit")
    {   //if it's exit
        terminate();    //close up everything
        exit(0);    //and exit
    }
}

void JobServer::addNewJob()
{
    MyJob* mj = queuedJobs.popTop();    //get the top queued job
    int pid;
    {
        if((pid = fork()) == 0)
        {   //try first exece, in case it's an executable in the same path
            if(execve(mj->getName().c_str(), mj->getArguments(), environ) < 0)
            {   //then execvp in case it's another command
                if(execvp(mj->getName().c_str(), mj->getArguments()) < 0)
                {   //if they both fail
                    perror("Execvp failed!");
                    delete mj;  //release the memmory
                    exit(pid);  //and exit with pid so it can be removed from the list
                }
            }
        }
        activeJobs.insertEntry(mj); //add the job in the active list
        mj->setPid(pid);    //set it's pid
        mj->setActiveStatus();  //set it as active
    }
}

MyJob* JobServer::getJob(std::string s)
{
    std::stringstream ss;
    std::string line;
    ss << s;
    getline(ss, line, ' '); //skip the issuejob
    getline(ss, line, ' '); //get the actual name
    std::string name = line;    //set it in a string
    int argc = getLastNumber(s);    //get the number of arguments
    char** c = new char*[argc+1];   //create an array for them
    c[argc] = NULL; //set the last entry of it as null
    c[0] = new char(name.length() + 1); //and the first one as the name given
    strcpy(c[0], line.c_str());
    for(int i = 1; i < argc; i++)   //for each other argument
    {
        getline(ss, line, ' '); //get it
        c[i] = new char[line.length() + 1]; //and put it in the array
        strcpy(c[i], line.c_str());
    }   //create the new job (pid, job number, name, arguments, number of arguments)
    MyJob* mj = new MyJob(0, jobCount, name, c, argc);
    return mj;  //return it
}

void JobServer::sendResponse(MyJob* mj)
{   //response for issuejob
    FILE* fp = fopen("ServerToCommander", "w"); //open the fifo
    if(fp != NULL)
    {   //print the response
        mj->printToFile(fp);
        fclose(fp);
    }
    else
        perror("Failed to open the fifo");
}

void JobServer::sendResponse(int i)
{   //response for poll
    FILE* fp = fopen("ServerToCommander", "w"); //open the fifo
    if(fp != NULL)
    {
        if(i == 0)  //if it's the running
            activeJobs.printToFifo(fp); //print the response to fifo
        else    //if it's queued
            queuedJobs.printToFifo(fp);
        fclose(fp);//print the response to fifo
    }
    else
        perror("Failed to open the fifo");
}

void JobServer::run(int flag, int* jobsDone)
{
    if(flag == 0)   //if you received a command
    {
        std::string line = getCommand();    //find out what you received
        executeCommand(line);   //and execute it
    }
    else    //if a child terminated
    {
        for(int i = 0; i < flag; i++)   //for each child that terminated
        {
            activeJobs.removeEntryByPid(jobsDone[i]);   //remove it from the list
        }
        while(!queuedJobs.isEmpty() && concurrency > activeJobs.length())   //then, while there are still jobs to be entered, and there's room for them
        {
            addNewJob();    //insert them
        }
    }
}
