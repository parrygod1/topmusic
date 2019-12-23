#ifndef SERVERCONNECTCOMPONENT_H
#define SERVERCONNECTCOMPONENT_H
#include <errno.h>
#include <cstring>
#include <netdb.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/socket.h>

class serverconnect_Component
{
    private:
    struct sockaddr_in server;	// structura folosita pentru conectare 

    public:

    int connectToServer(char *ip, int port, int &socketDescriptor)
    {
        if ((socketDescriptor = socket (AF_INET, SOCK_STREAM, 0)) == -1)
          {
            perror ("Eroare la socket().\n");
            return errno;
          } 

        /* umplem structura folosita pentru realizarea conexiunii cu serverul */
        server.sin_family = AF_INET;
        server.sin_addr.s_addr = inet_addr(ip);
        server.sin_port = htons (port);

        if (connect (socketDescriptor, (struct sockaddr *) &server,sizeof (struct sockaddr)) == -1)
          {
            perror ("[client]Eroare la connect().\n");
            return errno;
          }
        return 1;
    }

    int sendMsgToServer(int &socketDescriptor, char *msg)
    {
        char buf[1000];
        memset(buf, '\0', sizeof(buf)); 
        strcpy(buf,msg);
        printf ("[client]Introduceti mesajul: \n");
        fflush (stdout);

        char *p = strstr(buf, "\n");
        if(p){*p=0;}

        printf("[client]Am citit %s\n",buf);

        if (write (socketDescriptor,&buf,sizeof(buf)) <= 0)
          {
            perror ("[client]Eroare la write() spre server.\n");
            return errno;
          } 
    }

    int recieveMsgFromServer(int &socketDescriptor)
    {
        char answer[1000];
        /* (apel blocant pina cind serverul raspunde) */
        if (read (socketDescriptor, &answer, sizeof(answer)) < 0)
          {
            perror ("[client]Eroare la read() de la server.\n");
            return errno;
          }
        printf ("[client]Mesajul primit este:\n%s\n", answer);
    }
};

#endif