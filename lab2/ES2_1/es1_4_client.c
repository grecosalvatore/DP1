/*CLIENT UDP, SEND THE NAME (3 PPARAMETER) AND WAIT THE ANSWER*/


#include "errlib.h"
#include "sockwrap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#define MAX_CHAR 31

//global variables
char *prog_name;


int main (int argc, char *argv[]) {
struct sockaddr_in  saddr;
short port;
char *addr;
int s,nOut,nIn,count;
struct in_addr tmpAddr;
char name[MAX_CHAR],answerMess[MAX_CHAR];
socklen_t addrlen;


	//take the program name
	prog_name = argv[0];

	if (argc != 4){
		err_quit ("Error in the number of parameters\n", prog_name);
	}

	if (strlen(argv[3]) > MAX_CHAR){
		err_quit ("Error: max lenght of third parameter = MAX_CHAR\n", prog_name);
	}

	port = atoi(argv[2]);
	addr = argv[1];
	strcpy(name,argv[3]);

	printf("ADDRESS: %s  PORT: %d  NAME: %s \n",addr,port,name);

	//translation from string to in_addr
	if (inet_aton(addr,&tmpAddr)==0){
		err_quit ("Error in translation of the address\n", prog_name);
	}

	

	//configuration of the address
	memset(&saddr, 0, sizeof(saddr));
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(port); // port converted to network format
	saddr.sin_addr = tmpAddr; //htonl(addr);
	
	//create the socket (UDP)
	s = Socket(AF_INET, SOCK_DGRAM, 0);
	if (s < 0){
		err_quit ("Error in the creation of the socket\n", prog_name);
	}

	//resend after 3 secondo, max 5 times
	count = 0;nOut = 0;
	while(count < 5) {
		alarm(3);
		printf("Try to send\n");
		nOut = sendto(s,&name,MAX_CHAR,0,&saddr,sizeof(saddr));
		printf("nOut : %d\n",nOut);
		if (nOut > 0){
			//if the send have succes,exit from loop
			count = 5;
		}
		count++;
	}
	nIn = recvfrom(s,&answerMess,MAX_CHAR,0,&saddr,&addrlen);
	printf("answer : %s\n",answerMess);
return 0;
}
