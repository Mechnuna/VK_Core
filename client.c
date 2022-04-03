#include "miniserv.h"

int main (int argc, char**argv)
{
	struct		timeval start, stop, delta;
    int 		socket;
	int			fd;
    char 		buf[BUFFERT];
	long int	n;
	int 		l;
    off_t count = 0, m, sz;
	struct stat buffer;
    
	l = sizeof(struct sockaddr_in);
	if (argc != 4)
	{
		printf("Error usage : %s <ip_serv> <port_serv> <filename>\n",argv[0]);
		return EXIT_FAILURE;
	}
    
    socket = create_client_socket(atoi(argv[2]), argv[1]);
    
	if ((fd = open(argv[3],O_RDONLY)) == -1)
	{
		perror("Open fail");
		return EXIT_FAILURE;
	}
    
	if (stat(argv[3],&buffer) == -1)
	{
		perror("Stat fail");
		return EXIT_FAILURE;
	}
	else
		sz = buffer.st_size;
    
	bzero(&buf,BUFFERT);
    
    if(connect(socket,(struct sockaddr*)&sock_serv,l) == -1)
	{
        perror("Conexion error\n");
        exit (3);
    }
	gettimeofday(&start, NULL);
    n = read(fd, buf, BUFFERT);
	while (n)
	{
		if(n == -1)
		{
			perror("Read fails");
			return EXIT_FAILURE;
		}
		m = sendto(socket, buf, n, 0, (struct sockaddr*)&sock_serv, l);
		if(m == -1)
		{
			perror("Send error");
			return EXIT_FAILURE;
		}
		count += m;
		bzero(buf, BUFFERT);
        n = read(fd, buf, BUFFERT);
	}
	m = sendto(socket, buf, 0, 0, (struct sockaddr*)&sock_serv,l);
	gettimeofday(&stop, NULL);
	duration(&start, &stop, &delta);
    
	printf("Number of bytes transferred : %lld\n",count);
	printf("Total size of : %lld \n",sz);
	printf("Total duration of : %ld.%d \n",delta.tv_sec,delta.tv_usec);
    
    close(socket);
	return EXIT_SUCCESS;
}
