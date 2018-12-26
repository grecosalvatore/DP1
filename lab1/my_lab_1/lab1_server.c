//server lab 1 

#include <sys/times.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include "sockwrap.h"
#include "errlib.h"

/*
struct in_addr{
	unit32_t s_addr;
}

struct sockaddr_in {
	unit8_t sin_family;
	unit16_t sin_port;
	struct in_addrt sin_addr;
	unit8_t sin_zero[8];


}	
*/

char *prog_name;


int main (int  argc,char *argv[]){
char *s_addr;
struct sockaddr_in saddr,cliaddr;
short port;
int s;
int listenfd;
socklen_t cliaddrlen = sizeof(cliaddr);
int res,connfd;

//check parameters 
if (argc != 3){
	printf("Error in the number of paramteres\n");
	exit(-1);
}

prog_name = argv[0];
s_addr = argv[1];
port = atoi(argv[2]);

// set address for the bind 
memset(&saddr, 0, sizeof(saddr));
saddr.sin_family = AF_INET;
saddr.sin_port = htons(port);
//saddr.sin_addr.s_addr = htonl(INADDR_ANY);

res = inet_aton(&s_addr,&saddr);

printf("\n %s \n",s_addr);
 
//creation of the socket
s = socket (PF_INET,SOCK_STREAM,IPPROTO_TCP); 	// socket tcp type
if ( s < 0)
	exit(-1);


//binding
listenfd = bind (s,(struct sockaddr*)&saddr,sizeof(saddr));
//Bind (s,(struct sockaddr*)&saddr,sizeof(saddr));

if (listenfd != 0 ) {
	printf ("Error in the binding\n");
	exit(-1);
}

printf(" listenfd = %d\n" , listenfd );

//passive socket in listen mode
res = listen(s,5);

printf (" res = %d \n" , res ); 

while (1) {
printf("waiting connections ... \n");

		int retry = 0;
		do {
			printf("\n1\n");
			printf(" connfd = %d \n",connfd);
			connfd = accept (s, (SA*) &cliaddr, &cliaddrlen);
			if (connfd>=0) {
				printf("\n3\n");
				printf("Connection estabilished\n");	
				retry = 1 ;
				 
			}
			else 	{
				printf("\n4\n");
				return 0;
			}

			
		} while (retry);
printf("\n2\n");
printf(" connfd = %d \n",connfd);

printf("Connection closed\n");

}



}
