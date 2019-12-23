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

    void getCmdArgs(std::vector<std::string> &vector, std::string args, int count);

    public:
    void parseCommand(std::string command);
    std::string getMessage();
    ServerCmd();
    ~ServerCmd();

};

#endif