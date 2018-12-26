#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include <string.h>
#include <time.h>

#include <rpc/xdr.h>

#include "../errlib.h"
#include "../types.h"
#include "../sockwrap.h"

#define LISTENQ 15
#define MAXBUFL 255

#ifdef TRACE
#define trace(x) x
#else
#define trace(x)
#endif

#define MSG_ERR "-ERR\r\n"
#define MSG_OK  "+OK\r\n"
#define MSG_QUIT  "QUIT"
#define MSG_GET "GET "

#define MAX_STR 1023

char *prog_name;



int receiver (int connfd) {
	char buf[MAXBUFL+1]; /* +1 to make room for \0 */
	int ret_val = 0;
	while (1) {
		trace( err_msg("(%s) - waiting for commands ...",prog_name) );
		int nread = 0; char c;
		do {
			// NB: do NOT use Readline since it buffers the input
			int n = read(connfd, &c, sizeof(char));
			if (n==1)
				buf[nread++]=c;
			else{
				ret_val = 0;
				break;
			}
		} while (c != '\n' && nread < MAXBUFL-1);
		if (nread == 0)
			return 0;
		/* append the string terminator after CR-LF */
		buf[nread]='\0';
		while (nread > 0 && (buf[nread-1]=='\r' || buf[nread-1]=='\n')) {
			buf[nread-1]='\0';
			nread--;
		}
		trace( err_msg("(%s) --- received string '%s'",prog_name, buf) );

		
		/* get the command */
		if ((strncmp(buf,MSG_GET,strlen(MSG_GET))==0)){
			/* GET request*/
			if (strlen(buf)>strlen(MSG_GET)){
				
				/* GET + FILENAME */ 
				char fname[MAX_STR+1];
				strcpy(fname, buf+4); /* starting from the 5 char*/
				
				trace( err_msg("(%s) --- client asked to send file '%s'",prog_name, fname) );
	
				struct stat info;
				int ret = stat(fname, &info);
				if (ret == 0) {
				FILE *fp;
				if ( (fp=fopen(fname, "rb")) != NULL) {
					uint32_t val1,val2,ts;
					int i;
					char c;
					int size = info.st_size;
					
					/* NB: strlen, not sizeof(MSG_OK), otherwise the '\0' byte will create problems when receiving data */
					
					/* SEND +OK*/
					Write (connfd, MSG_OK, strlen(MSG_OK) );/* send +OK message*/
					trace( err_msg("(%s) --- sent '%s' to client",prog_name, "+OK") );

					/* SEND SIZE*/
					val1 = htonl(size);	/* send size translated to network */
					Write (connfd, &val1, sizeof(size));
					trace( err_msg("(%s) --- sent '%d' - converted in network order - to client",prog_name, size) );

					/* SEND TIMESTAMP*/
					ts=info.st_mtime;
					val2 = htonl(ts);	/* send timestamp translated to network */
					Write (connfd, &val2, sizeof(ts));
					trace( err_msg("(%s) --- sent '%u' - converted in network order - to client",prog_name, ts) );

					/* SEND THE FILE*/
					for (i=0; i<size; i++) {
						fread(&c, sizeof(char), 1, fp);
						Write (connfd, &c, sizeof(char));
					}
					trace( err_msg("(%s) --- sent file '%s' to client",prog_name, fname) );
					fclose(fp);

				} else {
					ret = -1;
				}
			}
			if (ret != 0) {	
				Write (connfd, MSG_ERR, strlen(MSG_ERR) );
				ret_val = -1;
				break;
			}

				
			}else{
				/* ERROR no filename */
				Write (connfd, MSG_ERR, strlen(MSG_ERR) );
				ret_val = -1;
				break;
			}
		}else if (nread >= strlen(MSG_QUIT) && strncmp(buf,MSG_QUIT,strlen(MSG_QUIT))==0) {
			trace( err_msg("(%s) --- client asked to terminate connection", prog_name) );
			ret_val = 0;
			break;
		} else {
			Write (connfd, MSG_ERR, strlen(MSG_ERR) );
			ret_val = -1;
			break;
		}

	}


	return ret_val;
}


int receiverXdr (int connfd) {
	XDR xdr_in,xdr_out; /* XDR stream */
	FILE *fstreamIn,*fstreamOut; /* FILE stream */
	message msg;
	int ret_val = 0;
	
	char fname[MAX_STR+1];




	/* CREATE IN/OUT STREAM AND XDR */
	fstreamIn = fdopen(connfd, "r");
	xdrstdio_create(&xdr_in, fstreamIn, XDR_DECODE);
	xdr_int(&xdr_in, &ret_val);
	xdr_destroy(&xdr_in);

	printf(" %d \n",ret_val);

	return ret_val;
}

int main (int argc, char *argv[]) {

	int listenfd, connfd, err=0;
	short port;
	struct sockaddr_in servaddr, cliaddr;
	socklen_t cliaddrlen = sizeof(cliaddr);
	int flagXdr = 0;

	/* for errlib to know the program name */
	prog_name = argv[0];

	/* check arguments */
	if (argc<2)
		err_quit ("usage: %s *<-x> <port>\n", prog_name);
	
	if (argc == 3){
		if (strcmp(argv[1],"-x")==0){
		/* XDR MODE */
		flagXdr = 1;
		port=atoi(argv[2]);		
		}else{
			err_quit ("usage: %s *<-x> <port>\n", prog_name);
		}
	}else{
		/* NORMAL MODE */
		port=atoi(argv[1]);
	}

	/* create socket */
	listenfd = Socket(AF_INET, SOCK_STREAM, 0);

	/* specify address to bind to */
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(port);
	servaddr.sin_addr.s_addr = htonl (INADDR_ANY);

	Bind(listenfd, (SA*) &servaddr, sizeof(servaddr));

	trace ( err_msg("(%s) socket created",prog_name) );
	trace ( err_msg("(%s) listening on %s:%u", prog_name, inet_ntoa(servaddr.sin_addr), ntohs(servaddr.sin_port)) );

	Listen(listenfd, LISTENQ);

	while (1) {
		trace( err_msg ("(%s) waiting for connections ...", prog_name) );

		connfd = Accept (listenfd, (SA*) &cliaddr, &cliaddrlen);
		trace ( err_msg("(%s) - new connection from client %s:%u", prog_name, inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port)) );

		if (flagXdr == 0){
			err = receiver(connfd);
		}else{
			err = receiverXdr(connfd);
		}
		Close (connfd);
		trace( err_msg ("(%s) - connection closed by %s", prog_name, (err==0)?"client":"server") );
	}
	return 0;
}

