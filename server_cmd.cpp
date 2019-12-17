#include "server_cmd.h"
#include <cstring>

void ServerCmd::parseCommand(std::string command)
{   
    query = new SQLQuery();
    char cmd[100];
    strcpy(cmd, command.c_str());
    char *user = strstr(cmd, "username: ");
    user = user+10;
    
    char *pass = strstr(cmd, "pass: ");

    if(strstr(cmd, "register"))
    {
        query->addUser(std::string(user), std::string(pass));
    }
}

ServerCmd::ServerCmd()
{

}