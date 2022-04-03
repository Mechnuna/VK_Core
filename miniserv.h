#ifndef MINISERV_H
#define MINISERV_H

#include <sys/time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>


#include <fcntl.h>
#include <unistd.h>
#include <strings.h>

#define BUFFERT 512
#define BACKLOG 1

struct sockaddr_in sock_serv,sock_clt;

int duration (struct timeval *start,struct timeval *stop, struct timeval *delta);
int create_client_socket (int port, char* ipaddr);
int create_server_socket(int port);

#endif