#include "jobCommanderFunctions.hpp"

void printInputError()  //error message
{
    std::cout << "Please run the program with the correct parameters" << std::endl;
    std::cout << "jobCommander issuejob <job>" << std::endl;
    std::cout << "jobCommander setConcurrency <N>" << std::endl;
    std::cout << "jobCommander stop <jobID>" << std::endl;
    std::cout << "jobCommander poll [running, queued]" << std::endl;
    std::cout << "jobCommander exit" << std::endl;
}

int checkInput(int argc, char** argv)
{
   // if(argc != 1)   //if there are arguments (has already been checked, so commented it
    //{
        if((strcmp(argv[1], "setConcurrency") == 0 || strcmp(argv[1], "stop") == 0) && argc == 3)
        {   //if it's a command that required 2 arguments, 2nd of which is a number, and do not require a response
            std::stringstream ss;
            int n;
            ss << argv[2];
            if(ss >> n) //if the 2nd argument is a number
                return 1;   //send a positive answer
            else
                return 0;   //else a negative
        }
        if(strcmp(argv[1], "poll") == 0 && argc == 3)
        {   //if it's poll
            if(strcmp(argv[2], "queued") == 0 || strcmp(argv[2], "running") == 0)   //followed by queued or running
                return 2;   //send a positive - wait for response answer
            else
                return 0;   //else a negative
        }
        if(strcmp(argv[1], "exit") == 0 && argc == 2)
            return 1;   //if it's exit, send a positive
        if(strcmp(argv[1], "issuejob") == 0 && argc > 2)
            return 2;   //if it's issuejob and has more arguments, send a positive - wait for response answer
        return 0;   //else a negative
 //   }
   // return 0;
}

int checkAndStartServer()
{
    int pid = 0;
    std::ifstream server;
    server.open("serverPid");   //check if the server exists already
    if(!server.is_open())   //if it doesn't
    {
        int mypid = getpid();   //get your pid
        char mypidstr[100]; //turn it to string
        memset(mypidstr, '\0', 100);
        sprintf(mypidstr, "%d", mypid); //using sprintf
        if((pid = fork()) == 0) //create it
        {
            if(execl("./jobExecutorServer", "jobExecutorServer", mypidstr, NULL) < 0)
            {
                perror("jobExecutorServer exec failed");
                return -1;
            }
        }
        pause();    //wait for the server to tell you he's ready
    }
    else    //if it does exist
    {
        std::string line;
        getline(server, line);
        pid = atoi(line.c_str());   //get the pid of it from the file
        server.close();
    }
    return pid; //return in
}

void sendCommand(int argc, char* argv[], int i)
{
    FILE* command;
    kill(i, SIGRTMIN+1);    //let the user know you're sending a command
    command = fopen("CommanderToServer", "w");  //open the fifo
    if(command == NULL) //if it didn't open
    {   //let the user know
        perror("Failed to open fifo");
        exit(-1);   //and exit
    }
    for(int i = 1; i < argc; i++)   //for every argument in the command line
    {
        fprintf(command, "%s ", argv[i]);   //put them in the fifo, with spaces between them
    }
    fprintf(command, "%d\n", argc - 2); //then put the number of the arguments in the end, and end the line
    fclose(command);    //close the file
}

void getResponse()
{
    FILE* inp;
    inp = fopen("ServerToCommander", "r");  //open the fifo
    if(inp == NULL) //if it didn't open, let the user know
    {
        perror("Failed to open the fifo");
        exit(-1);   //amd exit
    }
    char buff[1024];
    memset(buff, '\0', 1024);
    int i = 0;  //else, read the answer, line by line
    while(fgets(buff, 1024, inp) > 0)
    {
        std::cout << buff;
    }
    fclose(inp);    //close the file
}
