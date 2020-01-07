#ifndef SERVERCMD_H
#define SERVERCMD_H
#include <string>
#include <vector>
#include <sqlite3.h>
#include "sql_query.h"
#include "../shared/enums.h"

class ServerCmd
{
    private:
    SQLQuery *query;
    std::string msg;
    
    void setMessage(SQLMSG s, std::string msg);

    public:
    void parseCommand(std::string command, userData &user);
    std::string getMessage();
    ServerCmd();
    ~ServerCmd();

};

#endif