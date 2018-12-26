/* 	CLIENT WITH THE SELECT --> USER INTERFACE NEVER BLOCKED  */
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

#include "errlib.h"
#include "sockwrap.h"

#define MAXBUFL 255  // max size of the buffer
#define MAX_STR 1023 // max size of the file name

#define MSG_OK "+OK"

#ifdef TRACE
#define trace(x) x
#else
#define trace(x)
#endif

char *prog_name;




int main (int argc, char *argv[]) {

	int sockfd, err=0;
	char *dest_h, *dest_p;
	struct sockaddr_in destaddr;
	struct sockaddr_in *solvedaddr;
	int op1, op2, res, nconv;
	char buf[MAXBUFL];
	struct addrinfo *list;
	int err_getaddrinfo;
	char *fname;
	char inputString[MAX_STR];

	/* for errlib to know the program name */
	prog_name = argv[0];

	/* check arguments */
	if (argc!=3)
		err_quit ("usage: %s <dest_host> <dest_port> ", prog_name);
	dest_h=argv[1];
	dest_p=argv[2];
	

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

	while (1){
		int terminate = 0;
		fd_set rset;
		
			
			struct timeval tv;
			FD_ZERO(&rset);
			
			/* CREATE THE FD SET */
			FD_SET(sockfd, &rset); 
			FD_SET(fileno(stdin), &rset);
			
			tv.tv_sec = 10; /* TIMEOUT SETTED TO 10 SEC*/
			tv.tv_usec = 0;
	
		trace( err_msg("(%s) --- Insert filename, <q> for terminate, <a> for abort, <GET> <filename> for file transfer",prog_name) );
		
		if (select(sockfd+1, &rset, NULL, NULL, &tv) > 0) {

			if (FD_ISSET(fileno(stdin), &rset)){
				//printf(" STANDARD INPUT \n");
				// standard input is readable)
				FD_CLR(fileno(stdin), &rset);
				
				
				if ( (fgets(inputString, MAX_STR, stdin)) == NULL) {
					//Shutdown(sockfd, SHUT_WR);
				}

				
				
			
				//printf("input string : %s\n",inputString);
				if ((inputString[0]=='Q')||(inputString[0]=='q')){
					/* THE USER WANT TERMINATE AFTER RECIVE THE FILE */
					trace( err_msg("(%s) --- The user request to quit after the file transfer",prog_name) );
		
					terminate == 1;
				}

				if ((inputString[0]=='A')||(inputString[0]=='a')){
					/* THE USER WANT EXIT FROM THE PROGRAM */
					trace( err_msg("(%s) --- The user request to abort",prog_name) );
					/* Create quit message*/
					sprintf(buf, "QUIT\r\n");
					printf("bbbbbbbbbbbbbbbb\n");
					/* send the QUIT message */
					Write(sockfd, buf, strlen(buf));
	
					break;
				}
				
				if (((strncmp(inputString,"GET ",4)==0)||(strncmp(inputString,"get ",4)==0))&&strlen(inputString)>4){
					/* THE USER REQUIRE A FILE TRANSFER */
					printf("aaaaaaaaaaaaaaaaaaaaa\n");
					fname = inputString;

					/* Create get message*/
					sprintf(buf, "GET %s\r\n", fname+4);
					
					/* sedn the GET message */
					writen(sockfd, buf, strlen(buf));	

					trace ( err_msg("(%s) - data has been sent", prog_name) );
				}
				
			}

			if (FD_ISSET(sockfd, &rset)){
				/* READ is readable*/
				int nread = 0; char c;

				printf("sleeeeeep\n"),
				sleep(5);
				
			/* read the answer until \n*/
			
			nread = Readline_unbuffered(sockfd, buf, MAXBUFL);
			buf[nread]='\0';
			while (nread > 0 && (buf[nread-1]=='\r' || buf[nread-1]=='\n')) {
				buf[nread-1]='\0';
				nread--;
			}
			trace( err_msg("(%s) --- received string '%s'",prog_name, buf) );
			
			if (nread >= strlen(MSG_OK) && strncmp(buf,MSG_OK,strlen(MSG_OK))==0) {
				/* Recived OK message form the server*/
				char fnamestr[MAX_STR+1];
				uint32_t file_bytes;
				sprintf(fnamestr, "down_%s", fname+4);
				int n = Read(sockfd, &file_bytes, sizeof(file_bytes)); //read file size
				
				file_bytes = ntohl(file_bytes);

				uint32_t ts=  Read(sockfd, &ts, sizeof(ts)); //read file size
				ts =  ntohl(ts);
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
						FD_CLR(sockfd, &rset);
						trace( err_msg("(%s) --- received and wrote file '%s'",prog_name, fnamestr) );
						continue ;
					} else {
						trace( err_msg("(%s) --- cannot open file '%s'",prog_name, fnamestr) );
					}
				} else {
					trace ( err_quit("(%s) - protocol error: received response '%s'", prog_name, buf) );
				}

			 
			}
		
		
		}//end select
		else {
				printf("Timeout waiting for an answer from server\n");
			}

		
				
	}	// end while 
		
			
		
	
	Close (sockfd); //release resource of the socket

	return 0;
}

