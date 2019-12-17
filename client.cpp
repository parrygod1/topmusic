#include <iostream>
#include <sqlite3.h>
#include "serverconnect_component.h"
using namespace std;

serverconnect_Component server_component;

int main()
{
    int socket_descriptor;  
    char buf[100];
    memset(buf, '\0', sizeof(buf)); 
    strcpy(buf, "register username: xxxx pass: xd");
    server_component.connectToServer("10.0.2.15", 1234, socket_descriptor);
    server_component.sendMsgToServer(socket_descriptor, buf);
    server_component.recieveMsgFromServer(socket_descriptor);
    close(socket_descriptor);
}