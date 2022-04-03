#include "miniserv.h"

int duration (struct timeval *start,struct timeval *stop,struct timeval *delta)
{
    suseconds_t microstart, microstop, microdelta;
    
    microstart = (suseconds_t) (100000*(start->tv_sec))+ start->tv_usec;
    microstop = (suseconds_t) (100000*(stop->tv_sec))+ stop->tv_usec;
    microdelta = microstop - microstart;
    
    delta->tv_usec = microdelta%100000;
    delta->tv_sec = (time_t)(microdelta/100000);
    
    if((*delta).tv_sec < 0 || (*delta).tv_usec < 0)
        return -1;
    else
        return 0;
}

int create_server_socket(int port)
{
    int l;
	int sfd;
    int yes;

	yes = 1;
	sfd = socket(PF_INET,SOCK_STREAM,0);
	if (sfd == -1){
        perror("Socket fail");
        return EXIT_SUCCESS;
	}
    
    if (setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR,&yes,sizeof(int)) == -1 )
	{
        perror("Setsockopt error");
        exit(5);
    }
    
	l=sizeof(struct sockaddr_in);
	bzero(&sock_serv,l);
	
	sock_serv.sin_family=AF_INET;
	sock_serv.sin_port=htons(port);
	sock_serv.sin_addr.s_addr=htonl(INADDR_ANY);
    
	if(bind(sfd,(struct sockaddr*)&sock_serv,l)==-1)
	{
		perror("Bind fail");
		return EXIT_FAILURE;
	}
    
    
    return sfd;
}

int create_client_socket(int port, char* ipaddr)
{
    int l;
	int sfd;
    
	sfd = socket(PF_INET,SOCK_STREAM,0);
	if (sfd == -1)
	{
        perror("Socket fail");
        return EXIT_FAILURE;
	}
    
	l = sizeof(struct sockaddr_in);
	bzero(&sock_serv,l);
	
	sock_serv.sin_family = AF_INET;
	sock_serv.sin_port = htons(port);
    if (inet_pton(AF_INET,ipaddr,&sock_serv.sin_addr) == 0)
	{
		printf("Invalid IP adress\n");
		return EXIT_FAILURE;
	}
    
    return sfd;
}