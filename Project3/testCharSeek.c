/*
 * testCharSeek.c
 *
 *  Last edit: Oct 30, 2019
 *   final version for method spellcheck(verify a word in dictionary )
 */
#include<stdlib.h>
#include<stdio.h>
#include<string.h>          //Func :memset

#define MAX_BUFFER 1024	// max of string length



int main() {
	//initial
	char inputD[MAX_BUFFER]= {};
	FILE *fp;
	char dataSend[MAX_BUFFER]= {};
	fp = fopen("words.txt", "r");
	char c;

	int k=0 ;

	while(1) {
		printf("Type a words\n");
		fgets(dataSend, sizeof(char)*MAX_BUFFER, stdin);

		for(int x=0; dataSend[x]!='\0'; x++) {
			if(dataSend[x]==10)
				dataSend[x]='\0';
		}
		fseek(fp,0,SEEK_SET);
		while(1) {
			c = fgetc(fp);
			//	printf("%c\n",c);
			if( c==10 || c==EOF) {
				if(c==EOF) {
					inputD[k] ='\0';
					k=0;
					if(strcmp(inputD,dataSend)==0) {
						printf("%s  have char  %s\n", inputD, dataSend);
						break;
					}
//					fclose(fp);
					printf("No char\n");
					break;
				}
				inputD[k] ='\0';
				k=0;
				if(strcmp(inputD,dataSend)==0) {
					printf("%s  have char  %s\n", inputD, dataSend);
//					fclose(fp);
					break;
				}
				continue;
			} else {
				inputD[k] =c;
				k++;
				continue;
			}
		}

	}

	printf("error1\n");
	fclose(fp);

}
