#include <sys/times.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include "sockwrap.h"
#include "errlib.h"



int main (int  argc,char *argv[]){
char *s_addr;
struct sockaddr_in saddr;
int port;
int s;
int listenfd;
int res;

//check parameters 
if (argc != 3){
	printf("Error in the number of paramteres\n");
	exit(-1);
}

s_addr = argv[1];
port = atoi(argv[2]);



 
//creation of the socket
s = socket (PF_INET,SOCK_STREAM,IPPROTO_TCP); 	// socket tcp type
if ( s < 0)
	exit(-1);



// set address 
saddr.sin_family = AF_INET;
saddr.sin_port = htons(port); // host to network
saddr.sin_addr.s_addr = htonl(INADDR_ANY);

res = connect(s,(struct sockaddr*)&saddr,sizeof(saddr));
if (res < 0 ){
	printf ("Error in the connect\n");
	exit(-1);
}

sleep (10);
close(s); 
}
