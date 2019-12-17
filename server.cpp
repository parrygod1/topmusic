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
ServerCmd server_cmd;


static void *treat(void *); /* functia executata de fiecare thread ce realizeaza comunicarea cu clientii */
void raspunde(void *);
int startListen();
void acceptClients();

int main()
{

    startListen();
    acceptClients();
};	


static void *treat(void * arg)
{		
		struct thData tdL; 
		tdL= *((struct thData*)arg);	
		printf ("[thread]- %d - Asteptam mesajul...\n", tdL.idThread);
		fflush (stdout);		 
		pthread_detach(pthread_self());		
		raspunde((struct thData*)arg);
		/* am terminat cu acest client, inchidem conexiunea */
		close ((intptr_t)arg);
		return(NULL);	
  		
};

void raspunde(void *arg)
{
    int i=0;
    char command[1000];
    memset(command, '\0', sizeof(command));

	  struct thData tdL; 
	  tdL= *((struct thData*)arg);

	  if (read (tdL.cl, &command,sizeof(command)) <= 0)
		{
			  printf("[Thread %d]\n",tdL.idThread);
			  perror ("Eroare la read() de la client.\n");
		}
    printf ("[Thread %d]Mesajul a fost receptionat... %s\n",tdL.idThread, command);
		  
    server_cmd.parseCommand(std::string(command));

    char answer[1000];
    memset(answer, '\0', sizeof(answer));
    answer[0]='\0';

    sprintf(answer,"done");

    if(strcmp(answer,"\0")==0)
    {
      fread(answer, 1, ftell(stdout), stdout);
    }

	  printf("[Thread %d]Trimitem mesajul inapoi... %s\n",tdL.idThread,answer);
		    	      
	  if (write (tdL.cl, &answer, sizeof(answer)) <= 0)
		{
		  printf("[Thread %d] ",tdL.idThread);
		  perror ("[Thread]Eroare la write() catre client.\n");
		}
  	else
		printf ("[Thread %d]Mesajul a fost trasmis cu succes.\n",tdL.idThread);	
    
    
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
    
	// int idThread; //id-ul threadului
	// int cl; //descriptorul intors de accept

	td=(struct thData*)malloc(sizeof(struct thData));	
	td->idThread=i++;
	td->cl=client;

	pthread_create(&th[i], NULL, &treat, td);	      
				
	} 
}