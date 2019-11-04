/*
 * server.c
 *
 *  Last Edit: Nov 3, 2019
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





int main(int argc, char** argv) {
	// check the connection Port that user use
	if(argc == 1) {
		printf("%s\n","No port number entered.");
		return -1;
	}
	int connectionPort = atoi(argv[1]);
	connectionPort = atoi(argv[1]);
	if(connectionPort < 1024 || connectionPort > 65535) {
		printf("Port number is either too low(below 1024), or too high(above 65535).\n");
		return -1;
	}



	int sockfd;
	int fd_temp;

	int client_length;		// the length of client
	struct sockaddr_in s_addr_in;
	struct sockaddr_in s_addr_client;
	int  optval=1;
	int i_recvBytes;

		int sockfd_server;
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
	
	char dataSend[MAX_BUFFER]; //data send to sever
	char dataRec[MAX_BUFFER];	//data receive to sever
	memset(dataSend,0,MAX_BUFFER);
	memset(dataRec,0,MAX_BUFFER);
	client_length = sizeof(s_addr_client);

	//Block here. Until server accpets a new connection.
	sockfd = accept(sockfd_server,(struct sockaddr *)(&s_addr_client),(socklen_t *)(&client_length));

	if(sockfd == -1) {
		fprintf(stderr,"Accept error!\n");
	}
	
		printf("connect successfully \n");
	write(sockfd,"Testing program",MAX_BUFFER);
	

	while(1) {
		
		i_recvBytes = read(sockfd,dataRec,MAX_BUFFER);

		if(i_recvBytes == 0) {
			printf("Maybe the client has closed\n");
			break;
		}
		if(i_recvBytes == -1) {
			fprintf(stderr,"read error!\n");
			break;
		}
		
	
		// if client type quit, worker is idle
		if(strcmp(dataRec,"quit1")==0) {
			break;
		}

		if(write(sockfd,"server receive data: ",MAX_BUFFER) == -1) {
			break;
		}

		if(write(sockfd,dataRec,MAX_BUFFER) == -1) {
			break;
		}

			printf("receive data: %s\n",dataRec);
	}// client disconnection


}// end main()

