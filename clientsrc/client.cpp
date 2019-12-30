#include <iostream>
#include <sqlite3.h>
#include "serverconnect_component.h"
//#include "UI_component.h"
using namespace std;

#define PORT 5005

bool CONNECTED = false;
bool LOGGEDIN = false;
bool RUNNING = true;
char SERVERIP[] = "172.25.1.4";

serverconnect_Component server_component;
//UIComponent *user_interface;

int main()
{
    /*
        user_interface = new UIComponent();

        while(user_interface->ui_current_state!=UI_EXIT)
        {
            user_interface->getInput();

            user_interface->printMenu();
        }
    */
    int socket_descriptor;  
    initCmdmap();
    CONNECTED = server_component.connectToServer(SERVERIP, PORT, socket_descriptor);
    if(CONNECTED==false)
        close(socket_descriptor);
    else
        {
            printf("Connected to server\n");
            server_component.sendMsgToServer(socket_descriptor, "login root root");//remove these 2 lines
            server_component.recieveMsgFromServer(socket_descriptor);
        }
    
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

                default: 
                break;
            }
        }

        switch(map_cmdval[buf])
        {
            case CMD_NULL:
            break;

            case CMD_USEREXIT:
                RUNNING = false;
                CONNECTED = false;
                close(socket_descriptor);
            break;

            case CMD_USERCONNECT:
                if(CONNECTED==false)
                    {
                        CONNECTED = server_component.connectToServer(SERVERIP, PORT, socket_descriptor);
                        if(CONNECTED==false)
                            close(socket_descriptor);
                    }
                else 
                    printf("Already connected\n");
            break;

            case CMD_USERDISCONNECT:
                CONNECTED = false;
                close(socket_descriptor);
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