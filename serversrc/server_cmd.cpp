#include "server_cmd.h"
#include <string>
#include <cstring>

constexpr unsigned int str2int(const char* str, int h = 0)
{
    	return !str[h] ? 5381 : (str2int(str, h+1) * 33) ^ str[h];
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

void ServerCmd::parseCommand(std::string command)
{   
    std::vector<std::string> args;
    args.clear();

    if(strncmp(command.c_str(), "reg", 3)==0)
    {
        getCmdArgs(args, command.substr(3, command.size()), 2);
        query->addUser(USER, args[0], args[1]);
    }
    else if(strncmp(command.c_str(), "adminreg", 8)==0)
    {
        getCmdArgs(args, command.substr(8, command.size()), 2);
        query->addUser(ADMIN, args[0], args[1]);
    }
    else if(strncmp(command.c_str(), "login", 5)==0)
    {
        getCmdArgs(args, command.substr(5, command.size()), 2);
        query->loginUser(args[0], args[1]);
    }
    else
    {
        msg = "Invalid command";
        return;
    }

    this->msg = query->getMessage();
}

std::string ServerCmd::getMessage()
{
    return this->msg;
}


ServerCmd::ServerCmd()
{
    query = new SQLQuery();
}

ServerCmd::~ServerCmd()
{
    delete query;
}