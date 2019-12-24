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
#include "../shared/enums.h"

class serverconnect_Component
{
    private:
    char buf[MSG_BUFSIZE];
    struct sockaddr_in server;	// structura folosita pentru conectare 
    SQLMSG msgstatus;

    SQLMSG readMsgstatus(char *buf)
    {
        char digits[MSG_STATLEN+1];
        strncpy(digits, buf, MSG_STATLEN);
        return SQLMSG(atoi(digits));
    }

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
        memset(buf, '\0', MSG_BUFSIZE); 
        strcpy(buf, msg);
        fflush (stdout);
        this->msgstatus = SQL_NULL;

        if(buf[0]!=NULL)
        {
            if (write (socketDescriptor, &buf, MSG_BUFSIZE) <= 0)
            {
              perror ("[client]Eroare la write() spre server.\n");
              return errno;
            } 
        }
        else
        {
          return -1;
        }
        return 1;
    }

    int recieveMsgFromServer(int &socketDescriptor)
    {
        memset(buf, '\0', MSG_BUFSIZE);

        if (read (socketDescriptor, &buf, MSG_BUFSIZE) < 0)
        {
            perror ("[client]Eroare la read() de la server.\n");
            return errno;
        }

        msgstatus = readMsgstatus(buf);

        printf ("[client]Mesajul primit este:\n%s\n", buf + MSG_STATLEN);
    }

    SQLMSG getMsgstatus()
    {
        return this->msgstatus;
    }
};

#endif