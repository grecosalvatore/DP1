/*SERVER UDP THAT WAIT FOR A DATAGRAM ON THE PORT SPECIFY ON THE FIRST PARAMETER*/
//LIMITING SERVER --> MAX 3 MESSAGE FROM A CLIENT



#include "errlib.h"
#include "sockwrap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAX_CHAR 31
#define MAX_REQ 10



//global variables
char *prog_name;


int main (int argc, char *argv[]) {
struct sockaddr_in  saddr,clientaddr;
short port;
char *addr;
int listenfd,nOut,nIn;
struct in_addr tmpAddr;
char name[MAX_CHAR],answerMess[MAX_CHAR];
struct sockaddr_in clients[MAX_REQ];
int request,i ,count_client,index;
socklen_t cliaddrlen = sizeof(clientaddr);


	//take the program name
	prog_name = argv[0];

	if (argc != 2){
		err_quit ("Error in the number of parameters\n", prog_name);
	}

	//first parameter is the port
	port = atoi(argv[1]);


	//configuration of the address
	memset(&saddr, 0, sizeof(saddr));
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(port); // port converted to network format
	saddr.sin_addr.s_addr =  htonl(INADDR_ANY); //address converted

	//inizialization of circular buffer
	request = 0;

	
	//create the socket (UDP)
	listenfd = Socket(AF_INET, SOCK_DGRAM, 0);
	if (listenfd < 0){
		err_quit ("Error in the creation of the socket\n", prog_name);
	}

	if (bind(listenfd,(SA*) &saddr, sizeof(struct sockaddr)) < 0){
		err_quit ("Error in the binding of the socket\n", prog_name);
	}
	//nOut = sendto(s,&name,MAX_CHAR,0,&saddr,sizeof(saddr));
	//printf("nOut : %d\n",nOut);


	request = 0;
	while (1){
		count_client = 0;
		printf("Waiting for udp message on address : 0.0.0.0 and port: %d \n",port);
		nIn = recvfrom(listenfd,&answerMess,MAX_CHAR,0,(SA*)&clientaddr,&cliaddrlen);
		
		for (i=0;i<MAX_REQ;i++){
			if (clientaddr.sin_addr.s_addr == clients[i].sin_addr.s_addr){
				count_client++;
			}
		}
		if (count_client < 3 ){
			request++;
			index = (request-1)%10;
			clients[index] = clientaddr;
			printf("Udp message Received : %s \n",answerMess);
			nOut = sendto(listenfd,&answerMess,MAX_CHAR,0,(SA*)&clientaddr,sizeof(clientaddr));
			printf("Udp message sended \n");
		}else {
			printf("maximum number of request\n");
		}
	}
return 0;
}
