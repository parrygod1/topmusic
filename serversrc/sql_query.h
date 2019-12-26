#ifndef SQLQUERY_H
#define SQLQUERY_H
#include <algorithm>
#include <string>
#include <cstring>
#include <iostream>
#include <sqlite3.h>
#include "../shared/enums.h"

class SQLQuery
{
    private:
    std::string message;

    void setMessage(SQLMSG s, std::string msg);
    bool checkUserExists(USRTYPE user_type, std::string name);
    bool checkSongExists(std::string link);
    void getQueryResult(sqlite3_stmt *stmt, char result[]);
    static int callback(void *NotUsed, int argc, char **argv, char **azColName); //static cause of sqlite req

    public:
    sqlite3 *db;
    int rc;
    char *szErrMsg;
    void addUser(USRTYPE user_type, std::string name, std::string pass);
    void loginUser(std::string name, std::string pass, userData &user);
    void submitSong(std::string name, std::string desc, std::string tags, std::string link);
    void approveSong(std::string submitted_id);
    void listSubmissions();
    void deleteSubmission(std::string submitted_id);
    std::string getMessage();
    void openDB();
    void closeDB();
    SQLQuery();
    ~SQLQuery();

};

#endif