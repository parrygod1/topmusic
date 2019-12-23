#ifndef SQLQUERY_H
#define SQLQUERY_H
#include <algorithm>
#include <string>
#include <cstring>
#include <iostream>
#include <sqlite3.h>

class SQLQuery
{
    private:
    enum msgreturnstatus {SQL_ERRGENERIC = 0, SQL_SUCCESS = 1} msgresturnstatus;

    std::string setMessage(msgreturnstatus, std::string msg);
    bool checkUserExists(std::string name);
    bool checkAdminExists(std::string name);
    static int callback(void *NotUsed, int argc, char **argv, char **azColName);

    public:
    sqlite3 *db;
    int rc;
    char *szErrMsg;
    void addUser(std::string name, std::string pass, std::string *message);
    void addAdmin(std::string name, std::string pass, std::string *message);
    void loginUser(std::string name, std::string pass, std::string *message);
    void openDB();
    void closeDB();
    SQLQuery();
    ~SQLQuery();

};

#endif