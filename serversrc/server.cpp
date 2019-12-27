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

#define PORT 1234
#define MAXERRORS 5

typedef struct thData{
	int idThread; 
	int cl; //descriptorul intors de accept
}thData;

struct sockaddr_in server;	// structura folosita de server
struct sockaddr_in from;	
int sd;		//descriptorul de socket 
int pid;
pthread_t th[100];    //Identificatorii thread-urilor care se vor crea
int i=0;
ServerCmd *server_cmd;


static void *treat(void *); /* functia executata de fiecare thread ce realizeaza comunicarea cu clientii */
void raspunde(void *);
int startListen();
void acceptClients();
bool readClient(void *arg, char cmd[]);
void executeCommand(char *cmd, userData &user);
bool sendClient(void *arg, char msg[]);

int main()
{
    server_cmd = new ServerCmd();

    startListen();
    acceptClients();

    delete server_cmd;
};	


static void *treat(void * arg)
{		
    pthread_detach(pthread_self());	
		struct thData tdL; 
		tdL= *((struct thData*)arg);
		fflush (stdout);		 	
    
    userData user;
    user.CONNECTED = true;
    user.LOGGEDIN = false;
    user.type = USER;
    int errorcount = 0;

    char command[MSG_BUFSIZE];
  
    while(user.CONNECTED)
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

      if(strcmp(command, "disconn")==0 || strcmp(command, "exit")==0)
      {
          user.CONNECTED = false;
          user.LOGGEDIN = false;
          sendClient((struct thData*)arg, "999:Connection closed");
      }
      else if(strlen(command)>0)
      {
          executeCommand(command, user);
          if(!sendClient((struct thData*)arg, (char*)server_cmd->getMessage().c_str()))
            errorcount++;
      }
    }
		/* am terminat cu acest client, inchidem conexiunea */
		close ((intptr_t)arg);
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
			  perror ("Eroare la read() de la client.\n");
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

	  printf("[Thread %d]Trimitem mesajul inapoi... %s\n", tdL.idThread, msg);
		    	      
	  if (write (tdL.cl, msg, MSG_BUFSIZE) <= 0)
		{
		  printf("[Thread %d] ",tdL.idThread);
		  perror ("[Thread]Eroare la write() catre client.\n");
      return 0;
		}
  	else
		  printf ("[Thread %d]Mesajul a fost trasmis cu succes.\n",tdL.idThread);	

    return 1;
}

int startListen()
{
  if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
        {
            perror ("[server]Eroare la socket().\n");
            return errno;
        }
        /* utilizarea optiunii SO_REUSEADDR */
        int on=1;
        setsockopt(sd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));


        bzero (&server, sizeof (server));
        bzero (&from, sizeof (from));

          server.sin_family = AF_INET;	
          server.sin_addr.s_addr = htonl (INADDR_ANY);
          server.sin_port = htons (PORT);

        if (bind (sd, (struct sockaddr *) &server, sizeof (struct sockaddr)) == -1)
          {
            perror ("[server]Eroare la bind().\n");
            return errno;
          }

        if (listen (sd, 2) == -1)
          {
            perror ("[server]Eroare la listen().\n");
            return errno;
          }
    return 1;
}

void acceptClients()
{
  while (1)
  {
    int client;
    thData * td; //parametru functia executata de thread     
    socklen_t length = sizeof (from);
    printf ("[server]Asteptam la portul %d...\n",PORT);
    fflush (stdout);
    /* acceptam un client (stare blocanta pina la realizarea conexiunii) */
    if ( (client = accept (sd, (struct sockaddr *) &from, &length)) < 0)
	  {
	      perror ("[server]Eroare la accept().\n");
	      continue;
	  }

    /* s-a realizat conexiunea, se astepta mesajul */
	  //int cl; //descriptorul intors de accept

	  td=(struct thData*)malloc(sizeof(struct thData));	
	  td->idThread=i++;
	  td->cl=client;

	  pthread_create(&th[i], NULL, &treat, td);	      
				
	} 
}