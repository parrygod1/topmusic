#ifndef SERVERCMD_H
#define SERVERCMD_H
#include <string>
#include "sql_query.h"

class ServerCmd
{
    private:
    SQLQuery *query;

    public:
    void parseCommand(std::string command);
    ServerCmd();

};

#endif