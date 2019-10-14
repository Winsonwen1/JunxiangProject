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


#include "myshell.h"



/*  Function "Simulation"  :  used to initialize data and program  */
void Simulation(int argc, char* argv[]) {
	char *command[MAX_ARGS]= {};	// store cammdnds
	char inputD[MAX_BUFFER]= {};	// input data from user
	char pwd[MAX_BUFFER];		// print working directory
	char shellPath[MAX_ARGS]="shell="; //store for the path of shell
	char readmePath[MAX_ARGS]="README=";	// ftore for the path of readma
//	char currPath[MAX_ARGS];	// store for current path
	int batFileBool = 0;	// boolean: is user use the  batchfile?
	char c; // getchar for batch file
	int k=0; // index for store data from batch file
	int checkRedir[SYMBOLNUM]= {0}; // check symbol <,>,|,&,>> form command
	int fd;   // open batch file
	int pid;  // pid for using pipe
	char *inPipe;	// command for output of pipe
	char *outPipe;	// command for input of pipe
	int pfds[2];	// pipe

	call_clr();
	getcwd(pwd,  sizeof(pwd));// get pwd

	// set up shellPath and put into environment
	strcat(shellPath,pwd);
	strcat(shellPath,"/myshell");
	putenv( shellPath); //add the working directory  of myshell in the environment variables
	// set up the path readme file and put into environment
	strcat(readmePath,pwd);
	strcat(readmePath,"/readme");
	putenv( readmePath);

	//Open batch file and set up input inputfile
	if(strcmp(argv[0], "./myshell")==0&&argc==2) {
		printf("enter");
		fd = open(argv[1], O_RDONLY);
		dup2(fd, 0);
		batFileBool = 1;

	}//end if


	while(1) {

		// reset all command
		for(int i1=0; i1<MAX_ARGS; i1++) {
			command[i1]=NULL;
		}

		// set up curr Path and put into environment
//		strcpy(currPath,"PWD=");
//		getcwd(pwd,  sizeof(pwd));
//		strcat(currPath,pwd);
//		putenv( currPath);


		if(batFileBool == 0) {
			// get command from user
			fprintf(stderr,"[%s%s]%s",getenv("NAME"),getenv("PWD"),"myshell>");
			fgets(inputD, sizeof(char)*MAX_BUFFER, stdin);
		} else {
			// get each line of command from batch file
			while(1) {

				c = getchar();
				if( c==10 || c==EOF) {
					if(c==EOF) {
						inputD[k] ='\0';
						k=0;
						break;
					}
					inputD[k-1] ='\0';
					k=0;
					break;
				} else {
					inputD[k] =c;
					k++;
					continue;
				}
			}
		}

		// flush stin
		fflush(stdin);

		////split input for execute
		splitInput(command,inputD,checkRedir);

		//if command have pipr symbol '|'
		if(checkRedir[8]) {

			inPipe = strtok(inputD, "|");
			outPipe = strtok(NULL, "");

			//	child program run for the pipe
			switch (pid=fork( )) {
				case -1:
					fprintf(stderr,"fork error1\n");

				case  0:
					// set up pipe.
					if ( pipe(pfds) == 0 ) {
						switch (pid=fork( )) {
							case -1:
								fprintf(stderr,"fork error2\n");
								//	child program run for input pipe
							case  0:
								close(1);
								dup2( pfds[1], 1 );
								close( pfds[0] );
								ioRedir(command,inPipe);
								exit(1);
								//parent parogam run for output pipe
							default:
								close(0);
								dup2( pfds[0], 0 );
								close( pfds[1] );
								wait(NULL);
								ioRedir( command,outPipe);
								exit(1);
						}// end second fork
					}// end pipe
					//origin program
				default:

					waitpid(pid,NULL,WUNTRACED );
					for(int i=0; i<100000; i++) {
					}
			}//end fitst fork


		} else {	// if No '|' in Command
			ioRedir(command,inputD);
		}

		//if using batch file. if all command in file is compeleted. close  file and exit program
		if(c==EOF) {
			close( fd);
			exit(1);

		}


		//reset checkRedir
		for(int i=0; i<SYMBOLNUM; i++) {
			checkRedir[i]=0;

		}


	}//end while



}//end Simulation



/*  Function "ioRedir"  :  used to exeute the command  run with I/O redirection  */
void ioRedir(char*command[],char inputD[]) {
	int checkRedir[10]= {0}; // verify: is comands have symbol'<','|','>','>>','&' and its index
	int checkCom; //store the command type
//	int temp[SYMBOLNUM];	//temporary array
	int outPid,inPid;// pid for I/O redirection
	int pid;
	int i1, i2;
	char *originCommand[MAX_ARGS]= {};// command befor '<','>','>>'
	char infile[MAX_BUFFER]= {}; // file for input Redirection
	char outfile[MAX_BUFFER] = {};// file for output redirection
	int openin=0,openout=0; //verify: is using I or O redirection
	int infd=0,outfd=0;// file descriptor for input/output file
	//verify for child progarm and parent progarm
	int inout=0;
	int both =0;
	int inout1=0;
	int inout2=0;

//	// reset  command[]
	for(int i1=0; i1<MAX_ARGS; i1++) {
		command[i1]=NULL;
	}
	//split command into commands
	splitInput(command,inputD,checkRedir);
	//store the command type
	checkCom = check(command);

	// if  command have '<'
	if(checkRedir[0]) {
		// if command have '>' or '>>' and get arg from args before '<' or '>' or '>>'
		if(checkRedir[2]>0) {

			for(int i=0; command[i]!=NULL; i++) {
				if(i<checkRedir[3]&&i<checkRedir[1]) {
					originCommand[i]=command[i];
				}
			}
		} else {
			for(int i=0; command[i]!=NULL; i++) {
				if(i<checkRedir[1]) {
					originCommand[i]=command[i];
				}
			}

		}

		// get a child program for input redirection
		inPid = fork();
		inout=1;


		if(inPid<0) {
			fprintf(stderr,"fork error4\n");
		} else if(inPid==0) {
			//get  input file name and original command
			strcat(infile,command[checkRedir[1]]);

			if(command[checkRedir[2]]>0)
				strcat(outfile,command[checkRedir[3]]);

			infd = open(infile, O_RDONLY);
			dup2(infd, 0);
			for( i2=0; command[i2]!=NULL; i2++) {
				command[i2]=originCommand[i2];
			}

			command[i2]=NULL;
			openin=1;
			both =1;

		} else {
			//parent program output Pid
			waitpid(inPid,NULL,WUNTRACED );
//			fprintf(stderr,"pidsad=%d\n",inPid);
			inout2=1;
		}

	}//end if(checkRedir[0])



	// if it's child progarm from input redirection
	if(inout==1&&both == 1) {
		// if have symbol  '>','>>'
		if(checkRedir[2]>0) {
			// if have symbol  '>'
			if(checkRedir[0]==1) {
				//get arg from args before '<' or '>' or '>>'
				for(int i=0; command[i]!=NULL; i++) {
					if(i<checkRedir[3]&&i<checkRedir[1]) {

						originCommand[i]=command[i];

					}
				}
				// if have symbol  '>>'
			} else {
				for(int i=0; command[i]!=NULL; i++) {
					if(i<checkRedir[3]) {
						originCommand[i]=command[i];
					}
				}
			}
			//open file
			if(checkRedir[2]==1) {
				outfd = open(outfile, O_CREAT|O_WRONLY|O_TRUNC);
			} else if(checkRedir[2]==2) {
				outfd = open(outfile, O_CREAT|O_WRONLY|O_APPEND);
			}
			//setup output redirection
			dup2(outfd, 1);
			//setup origin command
			for( i1=0; command[i1]!=NULL; i1++) {
				command[i1]=originCommand[i1];
			}
			command[i1]=NULL;
			openout=1;

		}
		// if No input rediretion '<'
	} else if(inout==0) {
		// if have symbol '>' or '>>'
		if(checkRedir[2]>0) {
			// if have symbol  '>'
			if(checkRedir[0]==1) {
				//get arg from args before  '>' or '>>'
				for(int i=0; command[i]!=NULL; i++) {
					if(i<checkRedir[3]&&i<checkRedir[1]) {
						originCommand[i]=command[i];
					}
				}
				// if have symbol  '>>'
			} else {
				for(int i=0; command[i]!=NULL; i++) {
					if(i<checkRedir[3]) {

						originCommand[i]=command[i];
					}
				}
			}

			//creat child program
			outPid=fork();
			if(outPid<0) {
				fprintf(stderr,"fork error5\n");
			} else if(outPid==0) {
				strcat(outfile,command[checkRedir[3]]);
				// open file
				if(checkRedir[2]==1) {
					outfd = open(outfile, O_CREAT|O_WRONLY|O_TRUNC,S_IRWXU);
				} else if(checkRedir[2]==2) {
					outfd = open(outfile, O_CREAT|O_WRONLY|O_APPEND,S_IRWXU);
				}

				//setup output redirection
				dup2(outfd, 1);
				inout=1;
				for( i1=0; command[i1]!=NULL; i1++) {

					command[i1]=originCommand[i1];

				}
				command[i1]=NULL;
				openout=1;
			} else {
				waitpid(outPid,NULL,WUNTRACED );
				inout1=1;
				if(inout==1)
					exit(1);

			}
		}

	} // end for output rediretion


	//reset vaild
	if((inout1==0&&inout2==1)||(inout1==1&&inout2==0) ) {
		for(int i=0; originCommand[i]!=NULL; i++) {
			originCommand[i]=NULL;
		}
		for(int i=0; i<10; i++) {
			checkRedir[i]=0;
		}
		for(int i=0; i<MAX_ARGS; i++) {

			command[i]= NULL;
		}

		return;
	}// end if


	//if input data have '&' run in the background
	if(checkRedir[6]==1) {
		//creat a child progarm	run with background execution
		switch (pid=fork( )) {
			case -1:
				fprintf(stderr,"backgroung exec error\n");

			case  0:  // child   //sleep(1);
				for(int i=0; i<10000; i++) {
				}

//				fprintf(stderr,"\n%s\n", "run in back");
				runOneCommand(command,inputD,checkCom);
				exit(1);
			default:
				puts("execution Run in background");

		}
	} else {
		//if not run with  background execution
		runOneCommand(command,inputD,checkCom);

	} //end run command
	
	for( int ii=0;ii< SYMBOLNUM;ii++)
		checkRedir[ii]=0;

	// reset data, close file and exit child progarm
	if(openin==1&&openout==1) {
		openin=0;
		close(infd);
		openout=0;
		close(outfd);
		exit(1);
	}

	if(openin==1) {
		openin=0;
		close(infd);

		exit(1);
	}
	if(openout==1) {

		openout=0;
		close(outfd);
		exit(1);
	}

}


/*  Function "runOneCommand"  :  used to Run a command */
void runOneCommand(char* command[], char* inputD, int checkCom) {

	switch (checkCom) {

		case cd:
			call_cd(command);
			break;
		case clr:
			call_clr();
			break;
		case dir:
			call_dir();
			break;
		case environ1:
			call_environ();
			break;
		case echo:

			call_echo(inputD);

			break;
		case help:
			call_help();
			break;
		case pause2:
			call_pause();
			break;
		case quit:
			printf("%s\n","Thank for use!! See you !!");
			exit(1);
			break;
		default :

			call_run(command);

			break;
	}
}

/*  Function "check"  :  Check the command type  */
int check(char* command[]) {
//	int i=0;

	if(command[0]==NULL)
		return 9;
	if(strcmp(command[0],"cd")==0 ) {

		return 0;
	} else if (strcmp(command[0],"clr")==0) {
		return 1;

	} else if (strcmp(command[0],"dir")==0) {

		return 2;

	} else if (strcmp(command[0],"environ")==0) {

		return 3;

	} else if (strcmp(command[0],"echo")==0) {
		return 4;

	} else if (strcmp(command[0],"help")==0) {
		return 5;

	} else if (strcmp(command[0],"pause")==0) {
		return 6;

	}  else if (strcmp(command[0],"quit")==0) {
		return 7;

	} else {
		return 8;
	}

}// end Function "check"

/*  Function "call_cd"  :  execute command 'cd'  */
int call_cd(char* command[]) {
	char currPath[MAX_ARGS];
	char pwd[MAX_BUFFER];
//	printf("you are using cd");
	chdir(command[1]);

	strcpy(currPath,"PWD=");
	getcwd(pwd,  sizeof(pwd));
	strcat(currPath,pwd);
	putenv( currPath);

	return 1;
} //end Function "call_cd"

/*  Function "call_clr"  :  execute command 'clr'  */
void call_clr() {
//	printf("\e[1;1H\e[2J");
	printf("\033[2J") ;
	puts("");



}//end Function "call_clr"

/*  Function "call_dir"  :  execute command 'dir'  */
void call_dir() {
	struct dirent *de;  // Pointer for directory entry
	int i=0;
	// opendir() returns a pointer of DIR type.
	DIR *dr = opendir(".");

	//if opendir couldn't open directory
	if (dr == NULL) {
		printf("Could not open current directory" );

	}

	//loop List the contents of directory
	while ((de = readdir(dr)) != NULL) {
		if (strcmp(de->d_name, ".") == 0 || strcmp(de->d_name, "..") == 0)
			continue;
		i++;
		printf("%-15s  ", de->d_name);
		if(i==5) {
			puts("");
			i=0;
		}
	}
	closedir(dr);
	puts("");
}// end Function "call_dir"

/*  Function "call_help"  :  execute command 'help'  */
void call_help() {

	int countline20 = 0; // if print 20 line
	int count;	// count line number
	FILE* readmeFile;
	char line[MAX_BUFFER];	// store char from a line
	readmeFile = fopen(getenv("README"),"r");
	char ch;

	// set up for getchar getting a char that doesn't need to enter
	struct termios old_term;
	struct termios cur_term;
	tcgetattr(STDIN_FILENO, &old_term);
	memcpy(&cur_term, &old_term, sizeof(cur_term));
	cur_term.c_lflag &= ~(ICANON);
	cur_term.c_cc[VMIN] = 1;
	cur_term.c_cc[VTIME] = 0;
	tcsetattr(STDIN_FILENO, TCSANOW, &cur_term);

	// scanf each line, the end of file to exit
	while(fscanf(readmeFile,"%[^\n]%*c\n",line)!=EOF) {
		printf("%s\n",line);
		count++;
		if (count==20) {
			countline20=1;
		}

		if(countline20) {
			printf("\033[7m more\033[m");

			while(1) {
				fflush(stdout);
				read(STDIN_FILENO, &ch, 1);
				//if type enter, print 1 line
				if (ch==10) {
					countline20=1;
					count=0;
					puts("");
					break;
					// type 'q' to exit 'help'
				} else if(ch=='q') {
					fflush(stdout);
					fclose(readmeFile);
					puts("");
					tcsetattr(STDIN_FILENO, TCSANOW, &old_term);
					return;
					//if other input print 20 line
				} else {

					countline20=0;
					count=0;
					puts("");
					break;
				}
			}

		}
	}
	tcsetattr(STDIN_FILENO, TCSANOW, &old_term);

	fclose(readmeFile);

	puts("");

}// end Function "call_help"

/*  Function "call_environ"  :  execute command 'environ'  */
void call_environ() {
	char ** env = environ;
	// List all the environment strings
	while(*env) fprintf(stdout,"%s\n",*env++);
}	// end Function "call_environ"

/*  Function "call_echo"  :  execute command 'echo'  */
void call_echo(char * command) {

	char* q;
	// args after echo
	q = strtok(command, "o");
	q = strtok(NULL, "");
	int i=0;
	int j=0;

	while( q[i]==10 || q[i] ==' ' ||q[i]=='\t'||  q[i]=='<'|| q[i]=='>'|| q[i]=='&'|| q[i]=='|' ) {
		i++;
	}

	//loop for command
	for (; q[i]!='\0'; i++) {
		if(j==0) {
			if(q[i]=='\t'||q[i]==' ' || q[i]=='<'|| q[i]=='>'|| q[i]=='&'|| q[i]=='|' ) {
				printf("%c",' ');
				j=1;
				continue;
			}
			printf("%c",q[i]);
		} else {
			if(!(q[i]=='\t'||q[i]==' ' || q[i]=='<'|| q[i]=='>'|| q[i]=='&'|| q[i]=='|' )) {
				i--;
				j=0;
			}
		}
	}
	puts("");
}

/*  Function "call_pause"  :  execute command 'pause'  */
void call_pause() {
	getpass("\n Please press <Enter> key to continue... ");
}// end Function "call_pause"

/*  Function "call_echo"  :  // execute external command  */
void call_run(char *command[]) {
	char  parent[MAX_ARGS] ;
	int i=0;

	//set up path of parent
	strcpy(parent,"parent=") ;
	strcat(parent, getenv("shell"));

	//creat a child program
	int pid=fork();
	if(pid==-1) {
		perror("fork error6");
	} else if (pid==0) {


		for(; command[i]!=NULL; i++ ) {

		}
		putenv( parent);
		//run command
		execvp(command[0],command);

		printf("\nchilerror\n");
		exit(1);
	} else {


		waitpid(pid,NULL,WUNTRACED );

	}

}// end  Function "call_echo"

/*  Function "splitInput"  : split input for execute */
void splitInput(char*command[],char inputD[], int checkRedir[]) {

	//reset checkRedir
	for(int i=0; i<SYMBOLNUM; i++) {
		checkRedir[i]=0;

	}

	char temp[MAX_ARGS][300];
	int i=0; // the index of iputD
	int j=0;
	int c = 0;	// the index for temp
	int index=0;	// index for commands
	int bool1 = 1;	//when input if not end
	int tick = 0;	// if not first 'space' or enter

	if( inputD[0]=='\0'||inputD[0]==10) {

		return;
	}


	while(bool1) {
		while(  inputD[i]==10||inputD[i] ==' ' ||inputD[i]=='\t'|| inputD[i]=='\0'
		        || inputD[i]=='<'|| inputD[i]=='>'|| inputD[i]=='&'|| inputD[i]=='|' ) {
			if(tick==0) {
				if(inputD[i]=='<') {
					checkRedir[0] = 1;
					checkRedir[1] = index;
				} else if(inputD[i]=='>' ) {
					checkRedir[2] = 1;
					checkRedir[3] = index;
					if(inputD[i]=='>'&&inputD[i+1]=='>' ) {
						i++;
						checkRedir[2]++;
					}
				}  else if(inputD[i]=='&' ) {
					checkRedir[6] = 1;

				} else if(inputD[i]=='|' ) {
					checkRedir[8] = 1;
					checkRedir[9] = index;
				}


				i++;
				continue;
			}
			if(j==0) {
				temp[index][c] = '\0';
				command[index]=temp[index];
				index+=1;
				c=0;
				j++;
			}

			if(inputD[i]=='<') {
				checkRedir[0] = 1;
				checkRedir[1] = index;
			} else if(inputD[i]=='>' ) {
				checkRedir[2] = 1;
				checkRedir[3] = index;
				if(inputD[i]=='>'&&inputD[i+1]=='>' ) {
					i++;
					checkRedir[2]++;
				}
			}  else if(inputD[i]=='&' ) {
				checkRedir[6] = 1;
				checkRedir[7] = index;
			} else if(inputD[i]=='|' ) {
				checkRedir[8] = 1;
				checkRedir[9] = index;
			}


			if( inputD[i]=='\0') {
				bool1=0;
				break;
			}
			i++;
		}

		temp[index][c] = inputD[i];

		tick=1;
		c++;
		j=0;
		i++;
	}

	command[index]=NULL;

}// end Function "splitInput"




























