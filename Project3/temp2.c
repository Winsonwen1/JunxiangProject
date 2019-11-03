/*
 * server.c
 *
 *  Last Edit: Oct 31, 2019
 *      Author: Junxiang Wen
 */
#include<stdlib.h>
#include<pthread.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<stdio.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<assert.h>
#include<string.h>
#include<unistd.h>
#include <stdbool.h>
#include <malloc.h>


#define MAX_BUFFER 1024		// max of string length
#define MAX_WAIT_FOR_ACCE 24     //max of wait for accepet()
#define WORKER_NUMBER 1		// the number of worker

// Queue for the Job(connect worker and user ) or log File
typedef struct jobQueue {
	struct jobQueue *next;
	char logbuff[MAX_BUFFER];	// the output to log file
	int fd;		// sock fd
} JobQ;

//typedef struct logQueue {
//	struct logQueue *next;
//	char logbuff[MAX_BUFFER];
//} LogQ;

// data transfer from main thread to worker thread or log thread
typedef struct threadItem {
	int workID;
	JobQ* jq;
	JobQ* lq;
	FILE* fd;
} threadi;

// condition variable cond_product
pthread_cond_t cond_product;

// 定义条件变量cond_consume
pthread_cond_t cond_consume;

//lock for Thread mutex
pthread_mutex_t lock;
pthread_mutex_t lock2;


static void workthread(threadi* threadi);	// worker thread and only can see in the file
void initQueue(JobQ**q); //initial queue
void Destroy_List(JobQ* head);	// destroy queue
void appendPQ(JobQ *q,int sockfd); // add node into queue
void appendPQ2(JobQ *q,char logB[MAX_BUFFER]);	// add node into queue
void popF(JobQ* head); 	//delete the first node in the queue
int Size_List(JobQ* q);	// get the size of queue
bool listIsEmpty(JobQ* q) ;	//	if the list is empty
int spellcheck(char dataRec[MAX_BUFFER], FILE *fo);	// check the word in the dictionary
void connectResult(char worker[],char *dataSend, char* dataRec);
// char arr[100000][MAX_BUFFER];

int main(int argc, char** argv) {
	// check the connection Port that user use
	if(argc == 1) {
		printf("No port number entered.\n");
		return -1;
	}
	int connectionPort = atoi(argv[1]);
	connectionPort = atoi(argv[1]);
	if(connectionPort < 1024 || connectionPort > 65535) {
		printf("Port number is either too low(below 1024), or too high(above 65535).\n");
		return -1;
	}

	int sockfd_server;

	// initial condition variable and lock
	pthread_cond_init(&cond_product, NULL);
	pthread_cond_init(&cond_consume, NULL);
	pthread_mutex_init(&lock, NULL);
	pthread_mutex_init(&lock2, NULL);

	int sockfd;
	int fd_temp;

	int signWorkerID = 1;  //set up id for worker
	int client_length;		// the length of client
	struct sockaddr_in s_addr_in;
	struct sockaddr_in s_addr_client;
	int  optval=1;

	// // open file
	FILE *fp;
	if ( (fp = fopen("words.txt", "r")) == NULL ) {
		puts("Fail to open file!");
		exit(0);
	}

	// initialize job Queue and log queue
	JobQ *jobq = NULL;
	initQueue( &jobq);
	JobQ *logq = NULL;
	initQueue( &logq);

	// set up threadi
	threadi* thre;
	if (NULL == (thre=(threadi *)malloc(sizeof(threadi))))
		return -1;

	thre->fd = fp;
	thre->jq= jobq;
	thre->lq = logq;


	// ipv4,TCP
	if ((sockfd_server = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		return -1;
	}

	// Eliminates "Address already in use" error from bind
	if (setsockopt(sockfd_server, SOL_SOCKET, SO_REUSEADDR,
	               (const void *)&optval , sizeof(int)) < 0) {
		return -1;
	}

	//before bind(), set the attr of structu//Reset the serveraddr struct, setting all of it's bytes to zero.
	//Some properties are then set for the struct, you don't
	//need to worry about these.
	//bind() is then called, associating the port number with the
	//socket descriptor.re sockaddr.
	bzero((char *) &s_addr_in, sizeof(s_addr_in));
	s_addr_in.sin_family = AF_INET;
	s_addr_in.sin_addr.s_addr = htonl(INADDR_ANY);
	s_addr_in.sin_port = htons((unsigned short)connectionPort);
	if(bind(sockfd_server,(struct sockaddr *)(&s_addr_in),sizeof(s_addr_in))<0) {
		fprintf(stderr,"bind error!\n");
		exit(1);
	}

	if(listen(sockfd_server,MAX_WAIT_FOR_ACCE)<0) {
		fprintf(stderr,"listen error!\n");
		exit(1);
	}

	// creat worker thread
	for(int j=0; j<WORKER_NUMBER; j++) {
		pthread_t thread_id;
		// work id for work
		thre->workID = signWorkerID;
		signWorkerID++;

		if(pthread_create(&thread_id,NULL,(void *)(&workthread), thre) == -1) {
			fprintf(stderr,"pthread_create error!\n");
			break;                                  //break while loop
		}
		// wait for set up
		usleep(50000);

	}

	//data send to the user
	char * dataSend = "Please wait for a worker for you...";

	// Receive from client
	while(1) {

		pthread_t thread_id;
		client_length = sizeof(s_addr_client);

		//Block here. Until server accpets a new connection.
		sockfd = accept(sockfd_server,(struct sockaddr *)(&s_addr_client),(socklen_t *)(&client_length));

		if(sockfd == -1) {
			fprintf(stderr,"Accept error!\n");
			continue;                               //ignore current socket ,continue while loop.
		}

		// lock the lock to append data into queue
		pthread_mutex_lock(&lock);
		printf("A new connection accpet! Waitting for worker\n");
		write(sockfd,dataSend,MAX_BUFFER);
		appendPQ(jobq, sockfd);

		pthread_cond_signal(&cond_consume); // signal to the work thread
		pthread_mutex_unlock(&lock);// release the lock to append data into queue


	}

	//Clear
	int ret = shutdown(sockfd_server,SHUT_WR); //shut down the all or part of a full-duplex connection.
	fclose(fp);
	free(thre);
	thre=NULL;
	assert(ret != -1);

	printf("Server shuts down\n");
	return 0;
}// end main()

// worker thread and only can see in the file
static void workthread(threadi*  tempth ) {
	// loading data from main thread
	FILE* fp = tempth->fd;
	JobQ *jobq = tempth->jq;
	JobQ *logq = tempth->lq;
	char* logResult;

	// initial worker
	char worker[8+ WORKER_NUMBER/9]="Worker";
	sprintf(worker,"%s%d",worker,tempth->workID);
	printf("%s is setting up!\n",worker);


	//inital for connect with the user
	int fd;
	int i_recvBytes;
	char dataRec[MAX_BUFFER];
	char * dataSend = "";
	char * dataTemp= "serve for you";
	int check;

	// when worker is working
	while(1) {


		printf("%s waiting for new connection...\n",worker);

		//  if job queue is empty, waitting for filling
		while (listIsEmpty(jobq)) {
			pthread_cond_wait(&cond_consume, &lock);
		}

		// if no worker thread is using the queue, lock it
		while(pthread_mutex_trylock(&lock2)) {
		}
		// get client fd from the queue
		fd = jobq->next->fd;
		popF(jobq);
		printf("A new connection for %s!\n",worker);
		write(fd,worker,MAX_BUFFER);
//		printf("111") ;
		write(fd,dataTemp,MAX_BUFFER);
//		printf("222") ;
		// unlock two lock
		pthread_mutex_unlock(&lock2);
		pthread_mutex_unlock(&lock);

//		printf("333") ;



		// when worker work with a user
		while(1) {

			// reset the fp
			fseek(fp,0,SEEK_SET);

			memset(dataRec,0,MAX_BUFFER);

			i_recvBytes = read(fd,dataRec,MAX_BUFFER);

			if(i_recvBytes == 0) {
				printf("Maybe the client has closed\n");
				break;
			}
			if(i_recvBytes == -1) {
				fprintf(stderr,"read error!\n");
				break;
			}


			check = spellcheck(dataRec, fp);

			// check the work in the dictionary
			if(check==1) {
				dataSend=": Dictionary have the word";
			} else {
				dataSend=": Dictionary doesn't have the word";
			}
//			printf("%s%s %s\n",worker,dataSend, dataRec);
//			printf("===5=======\n");

			connectResult(worker,dataSend, dataRec);
			printf("======6\n");
			// if client type quit, worker is idle
			if(strcmp(dataRec,"quit1")==0) {
				printf("Quit command!\n");
				break;
			}
//			printf("===========\n");
			//	printf("read from client : %s\n",dataRec);
			if(write(fd,worker,MAX_BUFFER) == -1) {
				break;
			}
//			printf("======2=====\n");
			if(write(fd,dataSend,MAX_BUFFER) == -1) {
				break;
			}
//printf("======3=====\n");

		}

		printf("%s terminate current client connection...\n",worker);

	}
}	// end  workthread(threadi* threadi)

// check the word in the dictionary
int spellcheck(char dataRec[MAX_BUFFER], FILE *fo) {

	char wordIndictionary[MAX_BUFFER]= {};
	char c;
	int k=0; // index for char in the file

	while(1) {
		c = fgetc(fo );
		if( c==10 || c==EOF) {
			if(c==EOF) {
				wordIndictionary[k] ='\0';
				k=0;
				if(strcmp(wordIndictionary,dataRec)==0) {
//					printf("%s  have char  %s\n", wordIndictionary, dataRec);
					return 1;
				}
//				printf("No char\n");
				return 0;
			}
			wordIndictionary[k] ='\0';
			k=0;
			if(strcmp(wordIndictionary,dataRec)==0) {
//				printf("%s  have %s %s\n", "dictionary", "have word", dataRec);
				return 1;
			}
			continue;
		} else {
			wordIndictionary[k] =c;
			k++;
			continue;
		}
	}
	printf("error1\n");

} // end spellcheck()


// functiion for queue Initialization
void initQueue(JobQ**q) {
	*q = (JobQ *)malloc(sizeof(JobQ));

	if(*q == NULL) {
		printf("Can't assign to jobQueue, application ends \n");
		exit(1);
	}

	(*q)->next=NULL;
} // end initQueue()

//Destroy_List
void Destroy_List(JobQ* head) {

	JobQ* temp;
	while (head !=NULL) {
		temp = head;
		head = head->next;
		free(temp);
		temp=NULL;

	}
} //  end Destroy_List()

// add node into queue
void appendPQ(JobQ *q,int sockfd) {

	JobQ* n = q;
	JobQ* newJobQ= (JobQ *)malloc(sizeof(JobQ));
	//newJobQ->next = NULL;
	if(newJobQ == NULL) {
		printf("Failed to assign JobQ, program end \n");
		exit(-1);
	}
	// set data for new jobQueue
	newJobQ->fd = sockfd;

	newJobQ->next=NULL;

	while (n->next !=NULL) {
		n = n->next;
	}
	n->next= newJobQ;
	n= newJobQ;
}// end appendPQ()

// add node into queue
void appendPQ2(JobQ *q,char logB[MAX_BUFFER]) {

	JobQ* n = q;
	JobQ* newJobQ= (JobQ *)malloc(sizeof(JobQ));
	//newJobQ->next = NULL;
	if(newJobQ == NULL) {
		printf("Failed to assign JobQ, program end \n");
		exit(-1);
	}
	// set data for new jobQueue
	strcpy(newJobQ->logbuff, logB);

	newJobQ->next=NULL;

	while (n->next !=NULL) {
		n = n->next;
	}
	n->next= newJobQ;
	n= newJobQ;
}// end appendPQ2()




//delete first JobQ in list
void popF(JobQ* head) {
	JobQ* temp = head;
	JobQ* p;
	p = temp->next->next;
	free(temp->next);
	temp->next = NULL;
	temp->next = p;

} // end popF()

// Get the size of list or queue
int Size_List(JobQ* q) {
	q=q->next;
	int size = 0;
	while(q!=NULL) {
		q=q->next;
		size++;
	}
	return size;
} // end Size_List()

// verifying the list or queue is empty or not
bool listIsEmpty(JobQ* q) {
	if(q->next ==NULL) {
		return true;
	}
	return false;
} // end listIsEmpty()


void connectResult(char worker[],char *dataSend, char* dataRec) {
	
	char result[MAX_BUFFER];
	
	
	strcpy(result,worker);
	int index = strlen(result);

	while (*(dataSend)!='\0') {
		result[index]  = *dataSend;
		index++;
		dataSend++;
	}
	result[index] = ' ';
	index++;

	while (*(dataRec)!='\0') {
		result[index]  = *dataRec;
		index++;
		dataRec++;
	}
	result[index+1] = '\0';
	printf("%s\n",result);




}
























