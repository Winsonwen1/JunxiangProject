/*
 * testCharSeek.c
 *
 *  Last edit: Oct 30, 2019
 *   final version for method spellcheck(verify a word in dictionary )
 */
#include<stdlib.h>
#include<stdio.h>
#include<string.h>          //Func :memset

#define BUFFER_LENGTH 1024	// max of string length
#define MAX_CONN_LIMIT 512     //MAX connection limit


int main() {
	//initial 
	char  data_recv[BUFFER_LENGTH] = "test" ;
	FILE *fp;
	char inputD[BUFFER_LENGTH]= {};
	fp = fopen("words.txt", "r");
	char c;

	int k=0 ;

	while(1) {

		c = fgetc(fp);
		//	printf("%c\n",c);
		if( c==10 || c==EOF) {
			if(c==EOF) {
				inputD[k] ='\0';
				k=0;
				if(strcmp(inputD,data_recv)==0) {
				printf("have char\n");
					return 1;
				}
				fclose(fp);
				printf("No char\n");
				return 0;
			}
			inputD[k] ='\0';
			k=0;
			if(strcmp(inputD,data_recv)==0) {
				printf("%s  have char  %s\n", inputD, data_recv);
				fclose(fp);
				return 1;
			}
			continue;
		} else {
			inputD[k] =c;
			k++;
			continue;
		}
	}



	printf("error1\n");
	fclose(fp);

}
