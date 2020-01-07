#include <iostream>
#include <sqlite3.h>
#include "serverconnect_component.h"
#include "../shared/parse_func.h"

//#include "UI_component.h"
using namespace std;

bool CONNECTED = false;
bool LOGGEDIN = false;
bool RUNNING = true;
char helpstring[2000];

serverconnect_Component server_component;
//UIComponent *user_interface;

void openHelpFile();

int main()
{
    int socket_descriptor;  
    initCmdmap();
    openHelpFile();
    char buf[MSG_BUFSIZE];

    while(RUNNING)
    {
        memset(buf, '\0', MSG_BUFSIZE); 
        cin.getline(buf, MSG_BUFSIZE);

        if(CONNECTED)
        {
            if(server_component.sendMsgToServer(socket_descriptor, buf) == true)
                server_component.recieveMsgFromServer(socket_descriptor);

            switch(server_component.getMsgstatus())
            {
                case SQL_USRLOGINSUCCESS:
                    LOGGEDIN = true;
                break;

                case SQL_ADMINLOGINSUCCESS:
                    LOGGEDIN = true;
                break;

                case SQL_LOGOUTSUCCESS:
                    LOGGEDIN = false;
                break;

                case SV_FORCECLOSE:
                    CONNECTED = false;
                    LOGGEDIN = false;
                    printf("Disconnected from server\n");
                break;

                default: 
                break;
            }
        }

        switch(map_cmdval[getCmdkey(buf)])
        {
            case CMD_NULL:
            break;
            
            case CMD_HELP:
                printf("\x1b[H\x1b[J"); //clear screen
                printf("%s\n", helpstring);
            break;

            case CMD_USEREXIT:
                RUNNING = false;
                CONNECTED = false;
                close(socket_descriptor);
            break;

            case CMD_USERCONNECT:
                if(CONNECTED==false)
                    {
                        std::string cmd(buf+5);
                        std::vector<std::string> args;
                        getCmdArgs(args, cmd, 2);
                        CONNECTED = server_component.connectToServer((char*)args[0].c_str(), atoi(args[1].c_str()), socket_descriptor);
                        if(CONNECTED==false)
                                close(socket_descriptor);
                            else 
                                printf("Connected to server\n");
                    }   
                else 
                    printf("Already connected\n");
            break;

            case CMD_USERDISCONNECT:
                CONNECTED = false;
                close(socket_descriptor);
                printf("Disconnected from server\n");
            break;

            default:
            break;
        }
    }

    if(CONNECTED)
        {
            server_component.sendMsgToServer(socket_descriptor, "exit");
            close(socket_descriptor);
        }
}

void openHelpFile()
{
    char file_name[]="help.txt";
    char buffer[100];
    FILE *fp; 

    memset(helpstring, '\0', 2000);
    fp = fopen(file_name, "r");

    if (fp == NULL)
    {
       perror("Error while opening help file\n");
       strcpy(helpstring, "Could not display help, make sure 'help.txt' is in the same folder");
       return;
    }
    fgets(buffer, 100, fp);
    while(fgets(buffer, 100, fp))
    {
        strcat(helpstring, buffer);
    }
    fclose(fp);
    printf("\x1b[H\x1b[J");
    printf("Type 'help' for a list of commands\n");
}