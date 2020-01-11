#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include "server_cmd.h"

#define MAXERRORS 5

typedef struct thData{
	int idThread; 
	int cl; 
}thData;

struct sockaddr_in server;
struct sockaddr_in from;	
int sd;		 
int pid;
pthread_t th[100];   
int i=0;

ServerCmd *server_cmd;
bool RUNNING = true;
int PORT = 5005;

static void *treat(void *);
int startListen();
void acceptClients();
bool readClient(void *arg, char cmd[]);
void executeCommand(char *cmd, userData &user);
bool sendClient(void *arg, char msg[]);

int main(int argc, char **argv)
{
    if(argc>1)
      PORT = atoi(argv[1]);

    server_cmd = new ServerCmd();

    startListen();
    acceptClients();

    delete server_cmd;
    close(sd);
    return 0;
};	


static void *treat(void * arg)
{			
		struct thData tdL; 
		tdL= *((struct thData*)arg);
		fflush (stdout);		 	
    
    userData user;
    user.CONNECTED = true;
    user.LOGGEDIN = false;
    user.type = USER;
    int errorcount = 0;

    char command[MSG_BUFSIZE];
  
    while(user.CONNECTED && RUNNING)
    {
      if(errorcount > MAXERRORS)
      {
          user.CONNECTED = false;
          user.LOGGEDIN = false;
          printf("[Thread %d]",tdL.idThread);
          
			    perror ("Connection closed: error threshold reached\n");

          break;
      }

      if(!readClient((struct thData*)arg, command))
          errorcount++;

      if(RUNNING==false)
        break;

      if(strcmp(command, "disconn")==0 || strcmp(command, "exit")==0)
      {
          user.CONNECTED = false;
          user.LOGGEDIN = false;
          sendClient((struct thData*)arg, "999:Connection closed");
      }
      else if(strcmp(command, "stopsv")==0 && user.type==ADMIN)
      {
        RUNNING=false;
        user.CONNECTED = false;
        user.LOGGEDIN = false;
      }
      else if(strlen(command)>0)
      {
          executeCommand(command, user);
          if(!sendClient((struct thData*)arg, (char*)server_cmd->getMessage().c_str()))
            errorcount++;
      }
    }

    if(RUNNING==false)
      sendClient((struct thData*)arg, "111:Server closing...");

		close ((intptr_t)arg);
    pthread_detach(pthread_self());
		return(NULL);	
};

bool readClient(void *arg, char cmd[])
{
    fflush (stdout);
    bzero(cmd, MSG_BUFSIZE);

	  struct thData tdL; 
	  tdL= *((struct thData*)arg);

	  if (read (tdL.cl, cmd, MSG_BUFSIZE) <= 0)
		{
			  printf("[Thread %d]",tdL.idThread);
			  perror ("Error on read()\n");
        return 0;
		}
    return 1;
}

void executeCommand(char *cmd, userData &user)
{
    server_cmd->parseCommand(std::string(cmd), user);
}

bool sendClient(void *arg, char msg[])
{
    struct thData tdL; 
	  tdL= *((struct thData*)arg);

    if(strcmp(msg,"\0")==0)
    {
      fread(&msg, 1, ftell(stdout), stdout);
    }

	  printf("[Thread %d]MSG: %s\n", tdL.idThread, msg);
		    	      
	  if (write (tdL.cl, msg, MSG_BUFSIZE) <= 0)
		{
		  printf("[Thread %d] ",tdL.idThread);
		  perror ("[Thread]Error writing to client\n");
      return 0;
		}
  	else
		  printf ("[Thread %d]Message sent successfully\n",tdL.idThread);	

    return 1;
}

int startListen()
{
  if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
        {
            perror ("[server]Error on socket()\n");
            return errno;
        }
        int on=1;
        setsockopt(sd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));


        bzero (&server, sizeof (server));
        bzero (&from, sizeof (from));

          server.sin_family = AF_INET;	
          server.sin_addr.s_addr = htonl (INADDR_ANY);
          server.sin_port = htons (PORT);

        if (bind (sd, (struct sockaddr *) &server, sizeof (struct sockaddr)) == -1)
          {
            perror ("[server]Error on bind()\n");
            return errno;
          }

        if (listen (sd, 2) == -1)
          {
            perror ("[server]Errror on listen()\n");
            return errno;
          }
    return 1;
}

void acceptClients()
{
  while (RUNNING)
  {
    int client;
    thData * td; 
    socklen_t length = sizeof (from);
    printf ("[server]Waiting on port %d\n",PORT);
    fflush (stdout);

    if ((client = accept (sd, (struct sockaddr *) &from, &length)) < 0)
	  {
	      perror ("[server]Error on accept()\n");
	      continue;
	  }

	  td=(struct thData*)malloc(sizeof(struct thData));	
	  td->idThread=i++;
	  td->cl=client;

	  pthread_create(&th[i], NULL, &treat, td);	      	
	} 
}