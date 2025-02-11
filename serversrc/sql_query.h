#ifndef SQLQUERY_H
#define SQLQUERY_H
#include <algorithm>
#include <string>
#include <vector>
#include <cstring>
#include <iostream>
#include <sqlite3.h>
#include "../shared/enums.h"

class SQLQuery
{
    private:
    std::string message;

    sqlite3 *db;
    int rc;
    char *szErrMsg;

    void setMessage(SQLMSG s, std::string msg);
    bool checkUserExists(USRTYPE user_type, std::string name);
    bool checkSongExists(std::string link);
    bool checkSongIDExists(std::string song_id);
    bool checkCommentExists(std::string user_id, std::string song_id);
    bool checkVoteExists(std::string user_id, std::string song_id);
    bool checkUserSuspended(std::string user_id);
    void getQueryResult(sqlite3_stmt *stmt, char result[]);
    void updateScores();
    static int callback(void *NotUsed, int argc, char **argv, char **azColName); //static cause of sqlite req

    public:
    void addUser(USRTYPE user_type, std::string name, std::string pass);
    void loginUser(std::string name, std::string pass, userData &user);
    void submitSong(std::string name, std::string desc, std::string tags, std::string link);
    void approveSong(std::string submitted_id);
    void listSubmissions();
    void listAll();
    void listTop();
    void listUserInfo(std::string username);
    void setReputation(std::string name, std::string rep_value);
    void listComments(std::string song_id);
    void findTags(std::vector<std::string> &tags);
    void addVote(std::string user_id, std::string song_id, std::string vote_value);
    void addComment(std::string user_id, std::string song_id, std::string comment);
    void deleteSubmission(std::string submitted_id);
    void deleteSong(std::string song_id);
    std::string getMessage();
    void openDB();
    void closeDB();

    SQLQuery();
    ~SQLQuery();

};

#endif