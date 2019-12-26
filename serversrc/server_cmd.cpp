#include "server_cmd.h"
#include <string>
#include <cstring>

constexpr unsigned int str2int(const char* str, int h = 0)
{
    	return !str[h] ? 5381 : (str2int(str, h+1) * 33) ^ str[h];
}

std::string ServerCmd::getCmdkey(std::string command)
{
    char *p = strtok(&command[0], " ");
    return std::string(p);
}

void ServerCmd::getCmdArgs(std::vector<std::string> &vector, std::string args, int count)
{
    if(args[0]==' ')
        args.erase(0,1);

    char *c_args = new char[args.size()];
    strcpy(c_args, args.c_str());
    char *p = strtok(c_args, " ");
    if(p)
        vector.push_back(std::string(p));

    for(count; count > 0; count--)
    {   
        p = strtok (NULL, " ");
        if(p)   
        {   
            vector.push_back(std::string(p));
        } 
        else
        {
            vector.push_back(std::string(""));
        }  
    }
}

void ServerCmd::parseCommand(std::string command, userData &user)
{   
    std::vector<std::string> args;
    args.clear();

    //use return if we set any message in here otherwise it will be null
    switch(map_cmdval[getCmdkey(command)])
    {
        case CMD_NULL:
            setMessage(SQL_ERRGENERIC, "Invalid command");
            return;
        break;

        case CMD_USEREXIT:
            this->msg = "";
            return;
        break;

        case CMD_USERCONNECT:
            this->msg = "";
            return;
        break;

        case CMD_LOGIN:
            if(user.LOGGEDIN == false)
            {
                getCmdArgs(args, command.substr(5, command.size()), 2);
                query->loginUser(args[0], args[1], user);
            }
            else
            {
                setMessage(SQL_ERRGENERIC, "Already logged in");
                return;
            }
        break;

         case CMD_LOGOUT:
            if(user.LOGGEDIN == true)
            {
                user.LOGGEDIN = false;
                user.type = USER;
                setMessage(SQL_LOGOUTSUCCESS, "Logged out");
                return;
            }
            else
            {
                setMessage(SQL_ERRGENERIC, "Error: Not logged into an account");
                return;
            }
        break;

        case CMD_USERREG:
            getCmdArgs(args, command.substr(3, command.size()), 2);
            query->addUser(USER, args[0], args[1]);
        break;

        case CMD_ADMINREG:
            getCmdArgs(args, command.substr(4, command.size()), 2);
            query->addUser(ADMIN, args[0], args[1]);
        break;

        case CMD_SUBMITSONG:
            if(user.LOGGEDIN==true)
            {
                getCmdArgs(args, command.substr(8, command.size()), 4);
                query->submitSong(args[0], args[1], args[2], args[3]);
            }
            else
            {
               setMessage(SQL_ERRGENERIC, "Error: You need to be logged in to execute this command");
               return;
            }
            
        break;

        case CMD_APPROVESONG:
            if(user.type==ADMIN)
            {
                getCmdArgs(args, command.substr(8, command.size()), 1);
                query->approveSong(args[0], user);
            }
            else
            {
               setMessage(SQL_ERRGENERIC, "Error: Permission denied");
               return;
            }
        break;
    }

    this->msg = query->getMessage();
}

void ServerCmd::setMessage(SQLMSG s, std::string msg)
{
    this->msg = std::to_string(s) + ":" + msg;
}

std::string ServerCmd::getMessage()
{
    return this->msg;
}


ServerCmd::ServerCmd()
{
    initCmdmap();
    query = new SQLQuery();
}

ServerCmd::~ServerCmd()
{
    delete query;
}