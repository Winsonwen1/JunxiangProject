/*
 * Name:	Junxiang Wen
 * Last Edited:	09/011/2019
 * Topic:     Project 1: Giorgio's Discrete Event Simulator
 */

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdbool.h>
#define  IDEL 0
#define  BUSY  1


// Struct for creating Queue
typedef struct node {
	struct node *next;
	int processID;
	int processTime;
	int processSate;
} Node;

// Struct for JOB arr to store all job information
typedef struct JOB {
	int processID;  //process ID
	int arriveTime, endingTime, endInCPU,endInD1,endInD2;  //the time of the event happen

} job;


void initQueue(Node**q1) ; // functiion for queue Initialization
void loading(int arr[], char arr2[][13]); //loading data from config file
void Destroy_List(Node* head); // Destry queue for release Memory
// append data into priority queue
void appendPQ(Node *q, int pid, int ps,int pt, job arr[]); //void append(Node *q, int processID, int processState,int proccess time, job arr[])
void Simulation(); // the main Simulation systm
int Size_List(Node* head);	// Get the size of list or queue
bool listIsEmpty(Node* q); // verifying the list or queue is empty or not
void PrintList(struct JOB arr[] ); // Print information from arr
int RandomNumber(int low, int high); // Get the random number between low and high
void ListSort(Node* head);  // Sort the list or queue
void TransferQ(Node *pq, Node *q);  // transfer data from a queue in to the other queue
void popF(Node* head); // remove the first node from queue
void printListInfor(Node* q); // print data in the list or queue

//enum for show the state Of Process
enum stateOfProcess { processArrival, processFinishInCPU,processFinishInD1, processFinishInD2};

//Register Job id
static int jobID = 0;

int main(int argc, char* argv[]) {
	printf(" %s\n", "Simulation begin");
	Simulation();
	printf(" %s\n", "Simulation end");

	return 0;
}

// the main Simulation systm
void Simulation() {

	// initialize system
	char conf[30][13]= {};	// for store String data from config
	int confi[14];	//for store integer data from config
	loading(confi, conf);  // loading data from config file
	srand(confi[0]); // set the seed for random
	job job[4000];  // for store the time for Job
	Node *priorityQueue = NULL;   // initialize priority Queue
	initQueue( &priorityQueue);
	Node *cpuQueue = NULL;		 // initialize CPU Queue
	initQueue( &cpuQueue);
	Node *disk1_queue = NULL;	//initialize Disk1 Queue
	initQueue( &disk1_queue);
	Node *disk2_queue = NULL;	//initialize Disk2 Queue
	initQueue( &disk2_queue);

	// loading data from config for variable
	const int INIT_TIME = confi[1];
	const int FIN_TIME = confi[2];
	const int ARRIVE_MIN = confi[3];
	const int ARRIVE_MAX = confi[4];
	const int QUIT_PROB = confi[5];
	const int CPU_MIN = confi[6];
	const int CPU_MAX = confi[7];
	const int DISK1_MIN = confi[8];
	const int DISK1_MAX = confi[9];
	const int DISK2_MIN = confi[10];
	const int DISK2_MAX = confi[11];
	const int QUEUE_SIZE = confi[12];
	// for collect data
	int pqAveLen=0, cpuQueueAveLen = 0,  d1QueueAveLen =0, d2QueueAveLen=0; //average length for queue
	int pqMAXLen = 0, cpuQueueMaxLen = 0,  d1QueueMaxLen =0, d2QueueMaxLen=0; // the max length for queue
	int maxTemp= 0; // temp for max
	int cpuBusyTime=0, d1BusyTime=0, d2BusyTime = 0; // the busy time for each server
	int cpuAveResT = 0,  d1AveResT =0, d2AveResT=0; //average response time for each server
	int cpuCounTime=0, d1CounTime=0, d2CounTime=0; // Store the current count time
	int cpuMaxResT = 0,  d1MaxResT =0, d2MaxResT=0; // the max response time for each server
	int cpuFinshTimes = 0, d1FinshTimes = 0, d2FinshTimes = 0; // the number of completion times of each server
	// writing data into log.txt
	FILE *logF;
	if ((logF = fopen("log.txt", "w+") )== NULL) {
		exit(0);
	}

	// writing data from config file into log file
	int i = 0;
	int j = 0;
	for(; i<13; i++) {

		for (j = 0; conf[j][i] !='\0'; j++) {
			fprintf(logF,"%c",conf[j][i]);
		}
		fprintf(logF," %d\r\n", confi[i]);
	}

	// set the state for server
	int cpuState=IDEL, disk1_State=IDEL, disk2_State=IDEL;
	int rn;  //for stroe randomnumber
	printf("%s\n"," -------------Progarm Begin---------------");

	// Initializate the first Job
	if(cpuState==IDEL && listIsEmpty(priorityQueue) && listIsEmpty(cpuQueue)) {
		rn = RandomNumber(CPU_MIN,CPU_MAX);
		// append data into priority queue
		appendPQ(priorityQueue, jobID, processArrival,0,job);
	}

	//star loop
	for(int count=INIT_TIME; count<FIN_TIME; count++) {

		// printf("%d\n",count); //print the value of count for each loop
		ListSort(priorityQueue); // sort list in the beginning of loop
		 printListInfor(priorityQueue); // print the information inside of priority queue

		//if the first node == the times of loop
		if(priorityQueue->next->processTime == count ) {
			//if the state of  first node in priority queue is processArrival
			if(priorityQueue->next->processSate == processArrival) {
				rn = RandomNumber(ARRIVE_MIN,ARRIVE_MAX);
				fprintf(logF,"%s%4d%s%2d%s\r\n","at time ", count, " job ", priorityQueue->next->processID, " arrive at CPU_Q");
				//transfer the arrive "node" from priority queue into cpu queue
				TransferQ(priorityQueue,cpuQueue);
				// set the arrive time for next Job
				appendPQ(priorityQueue, jobID,processArrival, rn+count, job );
			}
		}

		if(priorityQueue->next->processTime == count ) {
			//if the state of  first node in priority queue is processFinishInCPU
			if(priorityQueue->next->processSate == processFinishInCPU) {

				rn = RandomNumber(0,100);
				fprintf(logF,"%s%4d%s%2d%s\r\n","at time ", count, " job ", priorityQueue->next->processID, " end CPU");

				if(rn>QUIT_PROB) {
					if(Size_List(disk1_queue)<=Size_List(disk2_queue)) {
						fprintf(logF,"%s%4d%s%2d%s\r\n","at time ", count, " job ", priorityQueue->next->processID, " enter d1_Q");
						TransferQ(priorityQueue,disk1_queue);
					} else {
						fprintf(logF,"%s%4d%s%2d%s\r\n","at time ", count, " job ", priorityQueue->next->processID, " enter d2_Q");
						TransferQ(priorityQueue,disk2_queue);
					}
				} else {
					// Job out of system
					fprintf(logF,"%s%4d%s%2d%s\r\n","at time ", count, " job ", priorityQueue->next->processID, " exit system");
					popF(priorityQueue);
				}

				cpuState = IDEL;

				cpuAveResT = cpuAveResT + (count- cpuCounTime);
				cpuFinshTimes++;

				if(cpuMaxResT<count- cpuCounTime) {
					cpuMaxResT=count- cpuCounTime;
				}
			}
		}

		if(priorityQueue->next->processTime == count ) {
			//if the state of  first node in priority queue is processFinishInD1
			if(priorityQueue->next->processSate == processFinishInD1) {
				fprintf(logF,"%s%4d%s%2d%s\r\n","at time ", count, " job ", priorityQueue->next->processID, " end d1");
				fprintf(logF,"%s%4d%s%2d%s\r\n","at time ", count, " job ", priorityQueue->next->processID, " enter CPU_Q");
				// transfer the node into Cpuqueue
				TransferQ(priorityQueue,cpuQueue);
				disk1_State = IDEL;

				d1AveResT =  d1AveResT+ (count- d1CounTime);
				d1FinshTimes++;

				if(d1MaxResT<count- d1CounTime) {
					d1MaxResT=count- d1CounTime;
				}
			}
		}

		if(priorityQueue->next->processTime == count ) {
			//if the state of  first node in priority queue is processFinishInD1
			if(priorityQueue->next->processSate == processFinishInD2) {
				fprintf(logF,"%s%4d%s%2d%s\r\n","at time ", count, " job ", priorityQueue->next->processID, " end d2");
				fprintf(logF,"%s%4d%s%2d%s\r\n","at time ", count, " job ", priorityQueue->next->processID, " enter CPU_Q");
				// transfer the node into Cpuqueue
				TransferQ(priorityQueue,cpuQueue);
				disk2_State = IDEL;

				d2AveResT += (count- d2CounTime);
				d2FinshTimes++;

				if(d2MaxResT<count- d2CounTime) {
					d2MaxResT=count- d2CounTime;
				}
			}
		}

		// get the max len for each queue
		maxTemp = Size_List(cpuQueue);
		if(cpuQueueMaxLen < maxTemp) {
			cpuQueueMaxLen = maxTemp;
		}
		maxTemp = Size_List(disk1_queue);
		if(d1QueueMaxLen < maxTemp) {
			d1QueueMaxLen = maxTemp;
		}
		maxTemp = Size_List(disk2_queue);
		if(d2QueueMaxLen < maxTemp) {
			d2QueueMaxLen = maxTemp;
		}

		// if the state of CPU is IDEL
		if(cpuState == IDEL) {
			if(!listIsEmpty(cpuQueue)) {
				rn = RandomNumber(CPU_MIN,CPU_MAX);
				//Set the time when the value finish in CPU
				appendPQ(priorityQueue,cpuQueue->next->processID,processFinishInCPU,rn+count,job);
				fprintf(logF,"%s%4d%s%2d%s\r\n","at time ", count, " job ", cpuQueue->next->processID, " begin CPU");
				popF(cpuQueue);
				cpuState = BUSY;

				cpuCounTime= count;
			}
		}

		// if the state of disk1 is IDEL
		if(disk1_State == IDEL) {

			if(!listIsEmpty(disk1_queue)) {

				rn = RandomNumber(DISK1_MIN,DISK1_MAX);
				//Set the time when the value finish in Disk1
				appendPQ(priorityQueue,disk1_queue->next->processID,processFinishInD1,rn+count,job);
				fprintf(logF,"%s%4d%s%2d%s\r\n","at time ", count, " job ", disk1_queue->next->processID, " begin d1");

				popF(disk1_queue);
				disk1_State = BUSY;

				d1CounTime= count;
			}
		}

		// if the state of disk2 is IDEL
		if(disk2_State == IDEL ) {
			if(!listIsEmpty(disk2_queue)) {
				rn = RandomNumber(DISK2_MIN,DISK2_MAX);
				//Set the time when the value finish in Disk2
				appendPQ(priorityQueue,disk2_queue->next->processID,processFinishInD2,rn+count,job);
				fprintf(logF,"%s%4d%s%2d%s\r\n","at time ", count, " job ", disk2_queue->next->processID, " begin d2");
				popF(disk2_queue);
				disk2_State = BUSY;

				d2CounTime= count;
			}
		}
		// get the max len for priority queue
		maxTemp = Size_List(priorityQueue);
		if(pqMAXLen < maxTemp) {
			pqMAXLen = maxTemp;
		}

		//add the current len for ave len for each server
		pqAveLen += Size_List(priorityQueue);
		cpuQueueAveLen += Size_List(cpuQueue);
		d1QueueAveLen += Size_List(disk1_queue);
		d2QueueAveLen += Size_List(disk2_queue);

		//When the component is busy. BusyTime++
		if(cpuState==BUSY) {
			cpuBusyTime++;
		}
		if(disk1_State==BUSY) {
			d1BusyTime++;
		}
		if(disk2_State==BUSY) {
			d2BusyTime++;
		}
		
		if(Size_List(priorityQueue)>=QUEUE_SIZE &&Size_List(priorityQueue)>=QUEUE_SIZE&&Size_List(priorityQueue)>=QUEUE_SIZE ){
			printf("%s","The size of queue is overlap, System ending!!!");
			exit(1);
		}		
		

	}// end loop

	// Calculate and print the Average len for each queue
	printf("%s %f\n","The average len of priority queue is ",(float)pqAveLen/(FIN_TIME-INIT_TIME));
	printf("%s %f\n","The average len of CPU queue is ",(float)cpuQueueAveLen/(FIN_TIME-INIT_TIME));
	printf("%s %f\n","The average len of Disk1 queue is ",(float)d1QueueAveLen/(FIN_TIME-INIT_TIME));
	printf("%s %f\n","The average len of Disk2 queue is ",(float)d2QueueAveLen/(FIN_TIME-INIT_TIME));
	// Calculate and print the max len for each queue
	printf("%s %d\n","The max len of Priotiry queue is ",pqMAXLen);
	printf("%s %d\n","The max len of CPU queue is ",cpuQueueMaxLen);
	printf("%s %d\n","The max len of Disk1 queue is ",d1QueueMaxLen);
	printf("%s %d\n","The max len of Disk2 queue is ",d2QueueMaxLen);
	// Calculate and print the max len for each queue
	printf("%s %f\n","The utilization of CPU is ",(float)cpuBusyTime/(FIN_TIME-INIT_TIME));
	printf("%s %f\n","The utilization of Disk1 is ",(float)d1BusyTime/(FIN_TIME-INIT_TIME));
	printf("%s %f\n","The utilization of Disk2 is ",(float)d2BusyTime/(FIN_TIME-INIT_TIME));
	// Calculate and print average response time for each server
	printf("%s %f\n","The average response of CPU is ",(float)cpuAveResT/cpuFinshTimes);
	printf("%s %f\n","The average response of Disk1 is ",(float)d1AveResT/d1FinshTimes);
	printf("%s %f\n","The average response of Disk2 is ",(float)d2AveResT/d2FinshTimes);
	// Calculate and print the maximum response time for each server
	printf("%s %d\n","The maximum response of CPU is ",cpuMaxResT);
	printf("%s %d\n","The maximum response of Disk1 is ",d1MaxResT);
	printf("%s %d\n","The maximum response of Disk2 is ",d2MaxResT);
	// Calculate and print the maximum response time for each server
	printf("%s %f\n","The throughput of CPU is ",(float)cpuFinshTimes/(FIN_TIME-INIT_TIME));
	printf("%s %f\n","The throughput of Disk1 is ",(float)d1FinshTimes/(FIN_TIME-INIT_TIME));
	printf("%s %f\n","The throughput of Disk2 is ",(float)d2FinshTimes/(FIN_TIME-INIT_TIME));

	//	PrintList(job); // print the information in job arr

	// Destory all the queue to release Memory
	Destroy_List(priorityQueue);
	Destroy_List(cpuQueue);
	Destroy_List(disk1_queue);
	Destroy_List(disk2_queue);
	priorityQueue=NULL;
	cpuQueue=NULL;
	disk1_queue=NULL;
	disk2_queue = NULL;
	// close log file
	fclose(logF);

	puts("program compelete");

}// end Simulation()


// loading data from config file
void loading(int arr[], char arr2[][13]) {
	FILE *fp;
	char str[20];// Store string data
	int i = 0;
	int j =0;

	if ((fp = fopen("config.txt", "r") )== NULL) {
		puts("File could not be opened");
		exit(0);
	} else {

		while(!feof(fp)) {

			fscanf(fp,"%s",str);
			//loading string data from config.txt
			for (j = 0; str[j] != '\0'; j++) {
				arr2[j][i]= str[j];
			
			}
			//loading integer data from config.txt
			fscanf(fp, "%d",&arr[i]);
			i++;
		}
	}

	fclose(fp);
}// end loading

// functiion for queue Initialization
void initQueue(Node**q) {
	*q = (Node *)malloc(sizeof(Node));

	if(*q == NULL) {
		printf("Can't assign to node, application ends \n");
		exit(1);
	}

	(*q)->next=NULL;
}

//Destroy_List
void Destroy_List(Node* head) {

	Node* temp;
	while (head !=NULL) {
		temp = head;
		head = head->next;
		free(temp);
		temp=NULL;

	}
}

//void append(Node *q, int processID, int processState,int proccess time, job arr[])
void appendPQ(Node *q, int pid, int ps,int pt, job arr[]) {

	Node* n = q;
	Node* newNode= (Node *)malloc(sizeof(Node));
	//newNode->next = NULL;
	if(newNode == NULL) {
		printf("Failed to assign Node, program end \n");
		exit(-1);
	}
	// set data for new node
	newNode->processID = pid;
	newNode->processSate = ps;
	newNode-> processTime = pt;
	newNode->next=NULL;

	while (n->next !=NULL) {
		n = n->next;
	}

	n->next= newNode;
	n= newNode;

	//set data into job arr
	arr[pid].processID = pid;
	if(ps == processArrival) {
		arr[pid].arriveTime = pt;
		jobID++;
	} else if(ps ==processFinishInCPU) {
		arr[pid].endInCPU = pt;
	} else if(ps ==processFinishInD1) {
		arr[pid].endInD1 = pt;
	} else if(ps ==processFinishInD2) {
		arr[pid].endInD2 = pt;
	}

}

//Transfer data into queue from priority queue
void TransferQ(Node *pq, Node *q) {

	Node* n = q;
	Node* newNode= (Node *)malloc(sizeof(Node));
//	newNode->next = NULL;
	if(newNode == NULL) {
		printf("Failed to assign Node, program end \n");
		exit(-1);
	}

	newNode->processID = pq->next->processID;
	newNode->next=NULL;

	while (n->next !=NULL) {
		n = n->next;
	}

	n->next= newNode;
	n= newNode;
	// remove the first node in priority queue
	popF(pq);
}// end TransferQ(Node *pq, Node *q)


//print the information in arr
void PrintList(struct JOB arr[] ) {
	printf("%s%d ", "Job ", arr[0].processID);
	printf("%d ", arr[0].arriveTime);
	puts("");
	for(int i=1; arr[i].processID!=0; i++) {

		printf("%s%d ", "Job ", arr[i].processID);
		printf("%d ", arr[i].arriveTime);
		puts("");
	}

}

//delete first Node in list
void popF(Node* head) {
	Node* temp = head;
	Node* p;
	p = temp->next->next;
	free(temp->next);
	temp->next = NULL;
	temp->next = p;

}

// Get the size of list or queue
int Size_List(Node* q) {
	q=q->next;
	int size = 0;
	while(q!=NULL) {
		q=q->next;
		size++;
	}
	return size;
}

// verifying the list or queue is empty or not
bool listIsEmpty(Node* q) {
	q=q->next;

	if(q ==NULL) {
		return true;
	}
	return false;
}

// Sort the list or queue using bubble sort
void ListSort(Node* head) {
	Node *cur,*cur2;
	Node *temp;
	cur = head;
	cur2 = head;
	Node * tail= NULL;
	Node * tail2= NULL;
	if(cur->next ==NULL || cur->next->next==NULL) {

		return;
	}
	if(cur2->next ==NULL || cur2->next->next==NULL) {

		return;
	}

	//using bubble sort to sort data by its processSate
	while(cur-> next !=tail) {
		while(cur->next->next!=tail) {
			if(cur->next->processSate > cur->next->next->processSate) {
				temp = cur->next;
				cur->next = cur->next->next;
				temp->next = temp->next->next;
				cur->next->next = temp;
			}
			cur = cur->next;
		}

		tail = cur->next;
		cur = head;

	}

	//using bubble sort to sort data by its processTime
	while(cur2-> next !=tail2) {
		while(cur2->next->next!=tail2) {
			if(cur2->next->processTime > cur2->next->next->processTime) {
				temp = cur2->next;
				cur2->next = cur2->next->next;
				temp->next = temp->next->next;
				cur2->next->next = temp;
			}
			cur2 = cur2->next;
		}

		tail2 = cur2->next;
		cur2 = head;

	}
}// end ListSort

// print data in the list or queue
void printListInfor(Node* q) {
	q=q->next;

	while(q!=NULL) {
		printf("%d-", q->processID);
		printf("%d-", q->processSate);
		printf("%d  ", q->processTime);
		q=q->next;
	}
	puts("");
}// end printListInfor()

// Get the random number between low and high
int RandomNumber(int low, int high) {

	return (low + rand()%(high-low+1));

}// end  RandomNumber(int low, int high)





















