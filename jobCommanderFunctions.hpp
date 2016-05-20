#ifndef __JOBCOMMANDERFUNCTIONS__HPP
#define __JOBCOMMANDERFUNCTIONS__HPP
#include <iostream>
#include <string.h>
#include <sstream>
#include <fstream>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>

void printInputError();

int checkInput(int argc, char** argv);

int checkAndStartServer();

void sendCommand(int argc, char* argv[]);
void sendCommand(int argc, char* argv[], int i);
void getResponse();

#endif
