#include <iostream>
#include <sqlite3.h>
#include "serverconnect_component.h"
//#include "UI_component.h"
using namespace std;

bool CONNECTED = 0;
bool LOGGEDIN = 0;

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
    if(server_component.connectToServer("10.0.2.15", 1234, socket_descriptor)==1)
    {
        CONNECTED = 1;
    }

    char buf[100];
    while(strcmp(buf,"exit")!=0)
    {
        memset(buf, '\0', sizeof(buf)); 
        cin.getline(buf, 100);

        if(CONNECTED)
        {
            server_component.sendMsgToServer(socket_descriptor, buf);
            server_component.recieveMsgFromServer(socket_descriptor);
        }

    }

    if(CONNECTED)
        close(socket_descriptor);

    
    
}