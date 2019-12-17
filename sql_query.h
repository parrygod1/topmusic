#ifndef SQLQUERY_H
#define SQLQUERY_H
#include <string>
#include <iostream>
#include <sqlite3.h>

class SQLQuery
{
    private:
    sqlite3 *db;
    int rc;
    char *szErrMsg;

    static int callback(void *NotUsed, int argc, char **argv, char **azColName);

    void openDB();
    void closeDB();

    public:
    void addUser(std::string name, std::string pass);
    SQLQuery();
    ~SQLQuery();

};

#endif