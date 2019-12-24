#include <iostream>
#include <sqlite3.h>
#include "serverconnect_component.h"
//#include "UI_component.h"
using namespace std;

#define PORT 1234

bool CONNECTED = 0;
bool LOGGEDIN = 0;
bool RUNNING = 1;
char SERVERIP[] = "10.0.2.15";

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
                case SQL_LOGINSUCCESS:
                    LOGGEDIN = true;
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
                if(!CONNECTED)
                    CONNECTED = server_component.connectToServer(SERVERIP, PORT, socket_descriptor);
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
        close(socket_descriptor);
}