#include "errlib.h"
#include "sockwrap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <rpc/xdr.h>

#define BUFLEN	128 
#define BUFSIZE 256

//global variables
char *prog_name;


int main (int argc, char *argv[]) {
struct sockaddr_in  saddr;
short port;
char *addr;
int connfd,result;
struct in_addr tmpAddr;
unsigned int n1,n2;
char sendBuf[BUFLEN],recBuf[BUFLEN];
int nOut,nIn;
int str_len;
XDR xdrs; /* XDR stream */
int res;
FILE *fstream; /* FILE stream */

	//take the program name
	prog_name = argv[0];

	if (argc != 3){
		err_quit ("Error in the number of parameters\n", prog_name);
	}

	port = atoi(argv[2]);
	addr = argv[1];

	printf("ADDRESS: %s  PORT: %d\n",addr,port);

	//translation from string to in_addr
	if (inet_aton(addr,&tmpAddr)==0){
		err_quit ("Error in translation of the address\n", prog_name);
	}

	//configuration of the address
	memset(&saddr, 0, sizeof(saddr));
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(port); // port converted to network format
	saddr.sin_addr = tmpAddr; //htonl(addr);
	
	//create the socket
	connfd = Socket(AF_INET, SOCK_STREAM, 0);

	
	
	//create connection
	result = connect(connfd,(struct sockaddr*)&saddr,sizeof(saddr));
	if (result == -1 ){
		err_quit ("Connection failed\n", prog_name);
	}

	//input from stdin
	printf("insert first number: ");
	scanf(" %u",&n1);
	printf("insert second number: ");
	scanf(" %u",&n2);

	/* create FILE stream */
	fstream = fdopen(connfd, “w”);

	/* create XDR stream */
	xdrstdio_create(&xdrs, fstream, XDR_ENCODE);

	xdr_int(&xdrs, &n1); /* convert/append first number int n1*/
	xdr_int(&xdrs,&n2); /* convert/append second number int n2*/

	/* destroy (free) stream */
	xdr_destroy(&xdrs);	

	/* create FILE stream */
	fstream = fdopen(connfd, “r”);

	
	/* create XDR stream */
	xdrstdio_create(&xdrs, fstream, XDR_DECODE);
	
	xdr_int(&xdrs, &res); /* extract/convert C int i */
	
	printf("risultato: %d \n",res);

	/* destroy (free) stream */
	xdr_destroy(&xdrs);
	return 0;
}
