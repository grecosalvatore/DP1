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

#include "errlib.h"

#include "sockwrap.h"

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
char prog_name_with_pid[MAXBUFL];
int nchildren;
pid_t *pids;


static void sig_int(int signo) {
	int i;
	err_msg ("(%s) info - sig_int() called", prog_name);
	for (i=0; i<nchildren; i++)
		kill(pids[i], SIGTERM);

	while( wait(NULL) > 0)  // wait for all children
		;

	if (errno != ECHILD)
		err_quit("(%s) error: wait() error");

	exit(0);
}

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




int main (int argc, char *argv[]) {
	struct sigaction action;
	int listenfd, connfd,i,sigact_res, err=0;
	short port;
	struct sockaddr_in servaddr, cliaddr;
	socklen_t cliaddrlen = sizeof(cliaddr);
	pid_t childpid;
	/* for errlib to know the program name */
	prog_name = argv[0];

	/* check arguments */
	if (argc!=3)
		err_quit ("usage: %s <port> <#children>\n", prog_name);
	port=atoi(argv[1]);
	nchildren=atoi(argv[2]);
	if (nchildren>10)
		err_quit("(%s) too many children requested", prog_name);

	pids = calloc(nchildren, sizeof(pid_t));
	if (pids == NULL)
		err_quit("(%s) calloc() failed", prog_name);
	

	/* create socket */
	listenfd = Socket(AF_INET, SOCK_STREAM, 0);

	/* This is needed only for multiple bind to the same address and port, or for bind after the server crashes, not to wait the timeout */
	/* Here, bind is performed in the parent process, thus this is not needed */
	/* However, it is useful to immediately restart the server if it closed while a connection is active */
	int option = 1;
	int ret_s = setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
	if (ret_s < 0)
		err_quit("(%s) setsockopt() failed", prog_name);	

	

	/* specify address to bind to */
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(port);
	servaddr.sin_addr.s_addr = htonl (INADDR_ANY);

	Bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr));
	
	Listen(listenfd, LISTENQ);

	for (i=0; i<nchildren; i++) {
		if ( (pids[i]= Fork()) > 0) {
			/* parent */
		} else {
			/* child */
			int cpid = getpid();
			sprintf(prog_name_with_pid, "%s child %d", prog_name, cpid);
			prog_name = prog_name_with_pid;
			trace ( err_msg("(%s) child %d starting", prog_name, cpid) );
			while (1) {
				struct sockaddr_in cliaddr;
				socklen_t clilen;

				clilen = sizeof(struct sockaddr_in);
				//cliaddr = malloc( clilen );
				//if (cliaddr == NULL)
				//	err_quit("(%s) malloc() failed", prog_name);

				connfd = Accept(listenfd, (struct sockaddr *)&cliaddr, &clilen);
				trace ( err_msg("(%s) - new connection from client %s:%u", prog_name, inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port)) );
				int ret = receiver(connfd);
				if (ret == 0) {
					err_msg("(%s) - connection terminated by client", prog_name);
				} else {
					err_msg("(%s) - connection terminated by server", prog_name);
				}
				Close(connfd);
			}
		}
	}
			
	/* This is to capture CTRL-C and terminate children */	
	memset(&action, 0, sizeof (action));
	action.sa_handler = sig_int;
	sigact_res = sigaction(SIGINT, &action, NULL);
	if (sigact_res == -1)
		err_quit("(%s) sigaction() failed", prog_name);

	while(1) {
		pause();
	}
	return 0;
}

