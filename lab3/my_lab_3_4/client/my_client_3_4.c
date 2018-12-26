/*

 */
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>
#include <stdio.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include <string.h>
#include <time.h>

#include <rpc/xdr.h>

#include "../types.h"
#include "../errlib.h"
#include "../sockwrap.h"

#define MAXBUFL 255  // max size of the buffer
#define MAX_STR 1023 // max size of the file name

#define MSG_OK "+OK"

#ifdef TRACE
#define trace(x) x
#else
#define trace(x)
#endif

char *prog_name;

void getFile(int sockfd) {
		char *fname;
		char inputString[MAX_STR];
		char buf[MAXBUFL];

		while (1){
	
		trace( err_msg("(%s) --- Insert filename, <quit> for terminate",prog_name) );
		scanf("%s",inputString);

		if ((strcmp(inputString,"quit")!=0)&&(strcmp(inputString,"QUIT")!=0)){

			fname = inputString;

			/* Create get message*/
			sprintf(buf, "GET %s\r\n", fname);
	
			/* sedn the GET message */
			Write(sockfd, buf, strlen(buf));

			trace ( err_msg("(%s) - data has been sent", prog_name) );

			fd_set rset;
			struct timeval tv;
			FD_ZERO(&rset);
			FD_SET(sockfd, &rset);
			tv.tv_sec = 2;
			tv.tv_usec = 0;

		if (select(sockfd+1, &rset, NULL, NULL, &tv) > 0) {
			int nread = 0; char c;
		
			/* read the answer until \n*/
			do {
				// NB: do NOT use Readline since it buffers the input
				Read(sockfd, &c, sizeof(char));
				buf[nread++]=c;
			} while (c != '\n' && nread < MAXBUFL-1);
			buf[nread]='\0'; // put string terminator

			while (nread > 0 && (buf[nread-1]=='\r' || buf[nread-1]=='\n')) {
				buf[nread-1]='\0';
				nread--;
			}
			trace( err_msg("(%s) --- received string '%s'",prog_name, buf) );

			if (nread >= strlen(MSG_OK) && strncmp(buf,MSG_OK,strlen(MSG_OK))==0) {
				/* Recived OK message form the server*/
				char fnamestr[MAX_STR+1];
				sprintf(fnamestr, "down_%s", fname);
				int n = Read(sockfd, buf, 4); //read file size
				//printf("n=%d\n",n);
				//printf("%0x%0x%0x%0x\n",buf[0], buf[1], buf[2], buf[3]);
				uint32_t file_bytes = ntohl((*(uint32_t *)buf));
				trace( err_msg("(%s) --- received file size '%u'",prog_name, file_bytes) );
	
				/* Creation of the file */
				FILE *fp;
				if ( (fp=fopen(fnamestr, "wb"))!=NULL) {
					char c;
					int i;
					for (i=0; i<file_bytes; i++) {
						/* Read one char at time and write in the file */
						Read (sockfd, &c, sizeof(char));
						fwrite(&c, sizeof(char), 1, fp);
					}
						fclose(fp); // close the file
						trace( err_msg("(%s) --- received and wrote file '%s'",prog_name, fnamestr) );
					} else {
						trace( err_msg("(%s) --- cannot open file '%s'",prog_name, fnamestr) );
					}
				} else {
					trace ( err_quit("(%s) - protocol error: received response '%s'", prog_name, buf) );
				}
	
			} else {
				printf("Timeout waiting for an answer from server\n");
			}
		}else{
		/* THE USER ENTERED QUIT*/
			/* Create quit message*/
			sprintf(buf, "QUIT\r\n");

			/* send the QUIT message */
			Write(sockfd, buf, strlen(buf));

			break;
		}
	/* At this point, the client can wait on Read , but if the server is shut down, the connection is closed and the Read returns */
	/* If the client is not waiting on Read, the server shut down does not affect the client */
	/* Read on a socket closed by the server immediately returns 0 bytes */
	/*	
	trace( err_msg("(%s) --- sleeping 5 sec",prog_name) );
	sleep(5);
	char d;
	trace( err_msg("(%s) --- reading 1 char",prog_name) );
	int nr = Read (sockfd, &d, sizeof(char));
	trace( err_msg("(%s) --- Read returned %d bytes",prog_name, nr) );
	*/	
	}/*end while*/

	return;

}

void getFileXdr(int sockfd) {
	XDR xdr_in,xdr_out; /* XDR stream */
	FILE *fstreamOut; /* FILE stream */
	char *fname;
	char inputString[MAX_STR];
	int z = 9;

	/* CREATE IN/OUT STREAM AND XDR */
	printf("---get file xdr \n");
	fstreamOut = fdopen(sockfd, "w");
	xdrstdio_create(&xdr_out, fstreamOut, XDR_ENCODE);

	
	xdr_int(&xdr_out,&z);


	sleep(10);
	xdr_destroy(&xdr_out);
	

	return;

}

int main (int argc, char *argv[]) {

	int sockfd, err=0;
	char *dest_h, *dest_p;
	struct sockaddr_in destaddr;
	struct sockaddr_in *solvedaddr;
	int op1, op2, res, nconv;
	
	struct addrinfo *list;
	int err_getaddrinfo;
	int flagXdr = 0;


	/* for errlib to know the program name */
	prog_name = argv[0];

	/* check arguments */
	if (argc<3)
		err_quit ("usage: %s *<-x> <dest_host> <dest_port> ", prog_name);
	if (argc == 3){
		dest_h=argv[1];
		dest_p=argv[2];
	}else{
		if ((argc == 4)&&(strcmp(argv[1],"-x")==0)){
			/* USE XDR */			
			flagXdr = 1;
			dest_h=argv[2];
			dest_p=argv[3];
		}else{
			err_quit ("usage: %s *<-x> <dest_host> <dest_port> ", prog_name);
		}
	}
	

	Getaddrinfo(dest_h, dest_p, NULL, &list);
	solvedaddr = (struct sockaddr_in *)list->ai_addr;

	/* create socket */
	sockfd = Socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	/* specify address to bind to */
	memset(&destaddr, 0, sizeof(destaddr));
	destaddr.sin_family = AF_INET;
	destaddr.sin_port = solvedaddr->sin_port;
	destaddr.sin_addr.s_addr = solvedaddr->sin_addr.s_addr;

	trace ( err_msg("(%s) socket created",prog_name) );


	/* connect to the socket */
	Connect ( sockfd, (struct sockaddr *)&destaddr, sizeof(destaddr) );

	trace ( err_msg("(%s) connected to %s:%u", prog_name, inet_ntoa(destaddr.sin_addr), ntohs(destaddr.sin_port)) );

	if (flagXdr == 0){
		getFile(sockfd);
	}else{
		getFileXdr(sockfd);
	}
	Close (sockfd); //release resource of the socket

	return 0;
}

