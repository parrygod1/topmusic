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
    struct sockaddr_in server;
    SQLMSG msgstatus;

    SQLMSG readMsgstatus(char *buf)
    {
        char digits[MSG_STATLEN+1];
        strncpy(digits, buf, MSG_STATLEN);
        return SQLMSG(atoi(digits));
    }

    public:

    bool connectToServer(char *ip, int port, int &socketDescriptor)
    {
        if ((socketDescriptor = socket (AF_INET, SOCK_STREAM, 0)) == -1)
          {
            perror ("Error on socket().\n");
            return false;
          } 


        server.sin_family = AF_INET;
        server.sin_addr.s_addr = inet_addr(ip);
        server.sin_port = htons (port);

        if (connect (socketDescriptor, (struct sockaddr *) &server,sizeof (struct sockaddr)) == -1)
          {
            perror ("[client]Error on connect().\n");
            return false;
          }
        return true;
    }

    int sendMsgToServer(int &socketDescriptor, char *msg)
    {
        memset(buf, '\0', MSG_BUFSIZE); 
        strcpy(buf, msg);
        fflush (stdout);
        this->msgstatus = SQL_NULL;

        if(buf[0]!='\0')
        {
            if (write (socketDescriptor, &buf, MSG_BUFSIZE) <= 0)
            {
              perror ("[client]Error on write()\n");
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
            perror ("[client]Error on read()\n");
            return errno;
        }

        msgstatus = readMsgstatus(buf);
        printf ("[server]%s\n", buf + MSG_STATLEN);
        return 1;
    }

    SQLMSG getMsgstatus()
    {
        return this->msgstatus;
    }
};

#endif