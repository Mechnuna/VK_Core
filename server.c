#include "miniserv.h"

int main (int argc,char** argv)
{
    int             sfd;
    int             fd;
    unsigned int    len;
    long int        nsid;
    ushort          clt_port;
    char            *folder;
    char            dst[INET_ADDRSTRLEN];
    char            buffer[BUFFERT];
    char            filename[256];
    time_t          intps;
    struct tm*      tmi;
    long int n, m, count = 0;
    

    len = sizeof(struct sockaddr_in);
    if (argc < 2)
    {
        perror("Usage : ./server <num_port> <folder>\n");
        exit(3);
    }
    if (argc == 3)
    {
        folder = strdup(argv[2]);
        if (mkdir(argv[2], S_IRWXU) == -1)
            perror("Folder creation error\n");
    }
    else
        folder = strdup(".");
    bzero(buffer, BUFFERT);
    while (1)
    {
        count = 0;
        sfd = create_server_socket(atoi(argv[1]));
        listen(sfd,BACKLOG);
        
        nsid = accept(sfd,(struct sockaddr*)&sock_clt, &len);
        if (nsid != -1)
        {
            if (inet_ntop(AF_INET, &sock_clt.sin_addr, dst, INET_ADDRSTRLEN) == NULL)
            {
                perror("Error socket");
                exit (4);
            }
            clt_port=ntohs(sock_clt.sin_port);
            printf("Start of the connection for : %s:%d\n", dst, clt_port);
            bzero(filename,256);
            intps = time(NULL);
            tmi = localtime(&intps);
            sprintf(filename,"%s/file.%d.%d.%d.%d.%d.%d", folder, tmi->tm_mday, tmi->tm_mon+1, 1900+tmi->tm_year, tmi->tm_hour, tmi->tm_min, tmi->tm_sec);
            printf("Creating the copied output file : %s\n", filename);
            if ((fd = open(filename, O_CREAT|O_WRONLY, 0600)) == -1)
            {
                perror("Open fail");
                exit (3);
            }
            bzero(buffer,BUFFERT);
            n = recv(nsid,buffer,BUFFERT,0);
            while (n)
            {
                if (n == -1)
                {
                    perror("Recv fail");
                    exit(5);
                }
                if ((m = write(fd, buffer ,n)) == -1)
                {
                    perror("Write fail");
                    exit (6);
                }
                count = count + m;
                bzero(buffer, BUFFERT);
                n = recv(nsid,buffer, BUFFERT,0);
            }
            close(sfd);
            close(fd);
        }
    close(nsid);
    printf("End of transmission from %s.%d\n",dst,clt_port);
    printf("Number of bytes received : %ld \n",count);
    }
    return EXIT_SUCCESS;
}
