#include <iostream>
#include <fstream>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sstream>
#include <stdlib.h>
#include "myjob.hpp"
#include "jobserver.hpp"
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

using namespace std;

int flag = -1;  //set the flag to figure if it's a command or a child terminated
int* jobsDone;

void sigHandler(int sig)
{
    if(sig == SIGRTMIN+1)   //if you received a signal from the commander
        flag = 0;
    if(sig == SIGCHLD)  //if some child terminated
    {
        int i = 0;
        int j = 0;
        while ((j = waitpid((pid_t)(-1), NULL, WNOHANG)) > 0)   //while you are still receiving signals from the children
        {
            if(jobsDone[i] == -2 && j != jobsDone[i])
            {
                jobsDone[i] = j;    //set the in the array
                i++;
            }
        }
        flag = i + 1;   //set the flag
    }
}

int main(int argc, char* argv[])
{
    sigset_t newSet, oldSet;
    sigemptyset(&newSet);   //empty the new set
    sigaddset(&newSet, SIGRTMIN+1); //add the signals we're going to use
    sigaddset(&newSet, SIGCHLD);
    sigprocmask(SIG_BLOCK, &newSet, &oldSet);   //block them
    struct sigaction sa;
    sa.sa_handler = sigHandler;     //set the handler
    sigaddset(&sa.sa_mask, SIGRTMIN+1); //the signals which it is going to block while handling
    sigaddset(&sa.sa_mask, SIGCHLD);
    sigaction(SIGRTMIN+1, &sa, NULL);   //and set the handler to handle those signals
    sigaction(SIGCHLD, &sa, NULL);

    int commanderPid = atoi(argv[1]);   //get the pid of the initial commander

    JobServer js;       //start the server
    js.initialize();    //initialize it
    kill(commanderPid, SIGRTMIN+2); //let the parent know you are done

    while(1)
    {
        jobsDone = new int[js.getNumberOfJobs()];   //set an array to store the pid
        for(int i = 0; i < js.getNumberOfJobs(); i++)   //initialize them all in -2
            jobsDone[i] = -2;
        sigsuspend(&oldSet);    //suspend and wait for a singal
        if(flag == 0)   //if you receiced a command
        {
            delete[] jobsDone;  //free the array in case you received an exit
            js.run(flag, NULL); //run the command
        }
        else    //if some children terminated
        {
            js.run(flag, jobsDone); //run first
            delete[] jobsDone;  //then clear the memory
        }
        flag = -1;  //reset the flag
    }
}
