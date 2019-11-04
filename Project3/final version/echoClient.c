/*
 * client.c
 *
 *  Last Edit: Oct 31, 2019
 *      Author: Junxiang Wen
 */
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<stdio.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<assert.h>
#include<string.h>
#include<unistd.h>

#define MAX_BUFFER 1024

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
	
	//set up port
	int sockfd;
	int tempfd;
	struct sockaddr_in s_addr_in;
	char dataSend[MAX_BUFFER]; //data send to sever
	char dataRec[MAX_BUFFER];	//data receive to sever
	memset(dataSend,0,MAX_BUFFER);
	memset(dataRec,0,MAX_BUFFER);

	//ipv4,TCP
	sockfd = socket(AF_INET,SOCK_STREAM,0);
	if(sockfd == -1) {
		fprintf(stderr,"socket error!\n");
		exit(1);
	}

	//initial the structure of sockaddr.
	memset(&s_addr_in,0,sizeof(s_addr_in));
	s_addr_in.sin_addr.s_addr = inet_addr("127.0.0.1");      //trans char * to in_addr_t
	s_addr_in.sin_family = AF_INET;
	s_addr_in.sin_port = htons(connectionPort);
	
	//connect with the client
	tempfd = connect(sockfd,(struct sockaddr *)(&s_addr_in),sizeof(s_addr_in));
	if(tempfd == -1) {
		fprintf(stderr,"Connect error! \n");
		exit(1);
	}
	
	//read data from serve
	tempfd = read(sockfd,dataRec,MAX_BUFFER);
	assert(tempfd != -1);
	printf("===%s=== \n",dataRec);


	// contact with sever
	while(1) {
		printf("Type ");

		fgets(dataSend, sizeof(char)*MAX_BUFFER, stdin);

		for(int x=0; dataSend[x]!='\0'; x++) {
			if(dataSend[x]==10)
				dataSend[x]='\0';
		}
		tempfd = write(sockfd,dataSend,MAX_BUFFER);

		if(tempfd == -1) {
			fprintf(stderr,"write error\n");
			exit(0);
		} 
		
		if(dataSend[0]=='q'&&dataSend[1]=='u'&&dataSend[2]=='i'&&dataSend[3]=='t'&&dataSend[4]=='1') { 
			break;
		} else {
			tempfd = read(sockfd,dataRec,MAX_BUFFER);
			assert(tempfd != -1);
			printf("%s",dataRec);
			memset(dataRec,0,MAX_BUFFER);
			tempfd = read(sockfd,dataRec,MAX_BUFFER);
			assert(tempfd != -1);
			printf("%s\n\n",dataRec);

		}

	}

	int ret = shutdown(sockfd,SHUT_WR);
	printf("Client end!!\n\n");
	assert(ret != -1);
	return 0;
}// end main  1
