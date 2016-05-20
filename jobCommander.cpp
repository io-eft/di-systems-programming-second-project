#include "jobCommanderFunctions.hpp"
#include <signal.h>

using namespace std;

void mysigHandler(int sig)
{
}

int main(int argc, char* argv[])
{
    if(argc == 1)   //if there are no arguments
        printInputError();  //print error
    else
    {
        struct sigaction sa;    //set a handler to receive the signal when the server is ready
        sa.sa_handler = mysigHandler;
        sigemptyset(&sa.sa_mask);
        sigaddset(&sa.sa_mask, SIGRTMIN+2);
        sigaction(SIGRTMIN+2, &sa, NULL);

        int pid = checkAndStartServer();    //check if the server is active, or start it
        int status = checkInput(argc, argv);    //check the validity of the command, and if it requires an answer
        if(status == 0) //if it's wrong
            printInputError();  //let the user know
        else    //if it's correct
        {
            sendCommand(argc, argv, pid);   //send the command
            if(status == 2) //if it requires a response
            {
                getResponse();  //get it
            }
        }
    }
}
