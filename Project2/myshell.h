
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/signal.h>
#include <sys/types.h>
#include <errno.h>
#include <pwd.h>
#include <dirent.h>
#include <sys/types.h>
/*
 * Name:	Junxiang Wen
 * Last Edited: 10/13/2019
 * Topic:     Project 2: Pseudocode for Implementing a Linux Shell
 *
 * Requir: implement a command line interpreter (CLI) or,
 * as it is more commonly known, a shell. The shell should operate in this basic
 * way: when you type in a command (in response to its prompt), the shell
 * creates a child process that executes the command you entered and then
 * prompts for more user input when it has finished.
 */

#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>

#define MAX_BUFFER 1024  // The max length of a command in one line is 1024 char
#define MAX_ARGS 64  // The max length of a command in one line is 64 commands
#define  EOF  (-1)
#define SYMBOLNUM  10 // double time of symbly number ('>','<','>>','&','|')

enum command {cd, clr, dir, environ1, echo, help, pause2, quit }; // for internal command type
extern char **environ;	//  the environment strings.

void runOneCommand(char* command[],char* inputD,int checkCom );	// Run a command
int call_cd(char* command[]);	//execute command 'cd'
int check(char*command[]);	// Check the command type
void Simulation(int , char* argv[]); // initialize data and program
void call_clr();// execute command 'clr'
void call_dir();// execute command 'dir'
void call_echo(char * );// execute command 'echo'
void call_environ();// execute command 'environ'
void call_pause();// execute command 'pause'
void call_run(char *command[]);// execute external command
void splitInput(char*command[],char inputD[], int checkRedir[]); //split input for execute
void ioRedir(char*command[],char inputD[]); // run with I/O redirection
void call_help();	// execute command 'help'


















