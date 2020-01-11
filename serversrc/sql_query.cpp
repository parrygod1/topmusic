#include "sql_query.h"
bool SQLQuery::checkUserExists(USRTYPE user_type, std::string name)
{
    std::string checkcommand;
    if(user_type==USER)
        checkcommand = "SELECT ID FROM Users WHERE Username = \'" + name + "\';";
    else if(user_type==ADMIN)
        checkcommand = "SELECT ID FROM Admins WHERE Username = \'" + name + "\';";

    struct sqlite3_stmt *selectstmt;
    rc = sqlite3_prepare_v2(db, checkcommand.c_str(), -1, &selectstmt, NULL);
    
    if(rc == SQLITE_OK)
    {
        if (sqlite3_step(selectstmt) == SQLITE_ROW)
            {
                sqlite3_finalize(selectstmt);
                return true; 
            }
        else 
            {
                sqlite3_finalize(selectstmt);
                return false;
            }
    }
    else
    {
        sqlite3_finalize(selectstmt);
        return false;
    }
}

bool SQLQuery::checkSongExists(std::string link)
{
    std::string checkcommand = "SELECT ID FROM Songs WHERE Link = \'" + link + "\';";

    struct sqlite3_stmt *selectstmt;
    rc = sqlite3_prepare_v2(db, checkcommand.c_str(), -1, &selectstmt, NULL);
    
    auto selected = sqlite3_step(selectstmt);
    sqlite3_finalize(selectstmt);
    
    if(rc == SQLITE_OK)
    {
        if (selected == SQLITE_ROW)
            {
                return true; 
            }
        else 
            { 
                return false;
            }
    }
    else
    {
        return false;
    }
}

bool SQLQuery::checkSongIDExists(std::string song_id)
{
    std::string checkcommand = "SELECT ID FROM Songs WHERE ID = \'" + song_id + "\';";

    struct sqlite3_stmt *selectstmt;
    rc = sqlite3_prepare_v2(db, checkcommand.c_str(), -1, &selectstmt, NULL);
    
    auto selected = sqlite3_step(selectstmt);
    sqlite3_finalize(selectstmt);
    
    if(rc == SQLITE_OK)
    {
        if (selected == SQLITE_ROW)
            {
                return true; 
            }
        else 
            { 
                return false;
            }
    }
    else
    {
        return false;
    }
}

bool SQLQuery::checkVoteExists(std::string user_id, std::string song_id)
{
    std::string checkcommand = "SELECT USERID FROM Votes WHERE USERID =" + user_id + " and SONGID=" + song_id + ";";

    struct sqlite3_stmt *selectstmt;
    rc = sqlite3_prepare_v2(db, checkcommand.c_str(), -1, &selectstmt, NULL);
    
    auto selected = sqlite3_step(selectstmt);
    sqlite3_finalize(selectstmt);
    
    if(rc == SQLITE_OK)
    {
        if (selected == SQLITE_ROW)
            {
                return true; 
            }
        else 
            { 
                return false;
            }
    }
    else
    {
        return false;
    }
}

bool SQLQuery::checkUserSuspended(std::string user_id)
{
    std::string checkcommand = "SELECT ID FROM Users WHERE ID =" + user_id + " and Reputation=1;";

    struct sqlite3_stmt *selectstmt;
    rc = sqlite3_prepare_v2(db, checkcommand.c_str(), -1, &selectstmt, NULL);
    
    auto selected = sqlite3_step(selectstmt);
    sqlite3_finalize(selectstmt);
    
    if(rc == SQLITE_OK)
    {
        if (selected == SQLITE_ROW)
            {
                return true; 
            }
        else 
            { 
                return false;
            }
    }
    else
    {
        return false;
    }
}

bool SQLQuery::checkCommentExists(std::string user_id, std::string song_id)
{
    std::string checkcommand = "SELECT USERID FROM Comments WHERE USERID =" + user_id + " and SONGID=" + song_id + ";";

    struct sqlite3_stmt *selectstmt;
    rc = sqlite3_prepare_v2(db, checkcommand.c_str(), -1, &selectstmt, NULL);
    
    auto selected = sqlite3_step(selectstmt);
    sqlite3_finalize(selectstmt);
    
    if(rc == SQLITE_OK)
    {
        if (selected == SQLITE_ROW)
            {
                return true; 
            }
        else 
            { 
                return false;
            }
    }
    else
    {
        return false;
    }
}

void SQLQuery::addUser(USRTYPE user_type, std::string name, std::string pass)
{
    if(checkUserExists(USER, name) || checkUserExists(ADMIN, name))
    {
        setMessage(SQL_ERRGENERIC, "Error: User already exists");
        return;
    }
    else if(!(std::any_of(std::begin(name), std::end(name), ::isalpha)))
    {
        setMessage(SQL_ERRGENERIC, "Error: Invalid username -- only alphanumeric characters allowed");
        return;
    }
    else if(pass.size()<4)
    {
        setMessage(SQL_ERRGENERIC, "Error: Invalid password -- password must have at least 4 characters");
        return;
    }

    std::string addcommand;
    // prepare our sql statements
    if(user_type==USER)
        addcommand = "INSERT INTO Users(username, password) VALUES (\'" + name + "\', \'" + pass + "\');";
    else if(user_type==ADMIN)
        addcommand = "INSERT INTO Admins(username, password) VALUES (\'" + name + "\', \'" + pass + "\');";


    // execute sql
    rc = sqlite3_exec(db, addcommand.c_str(), callback, 0, &szErrMsg);
    if(rc != SQLITE_OK)
    {
        std::cout << "SQL Error: " << szErrMsg << std::endl;
        sqlite3_free(szErrMsg);
        setMessage(SQL_ERRGENERIC, "Error: Register failed");
    }
    else
    {
        setMessage(SQL_REGSUCCESS, "User registered successfully");
    }
}

void SQLQuery::loginUser(std::string name, std::string pass, userData &user)
{
    std::string checkcommand;
    USRTYPE usert;

    if(checkUserExists(USER, name))
        {
            checkcommand = "SELECT ID FROM Users WHERE Username = \'" + name + "\' and Password = \'" + pass + "\';";
            usert = USER;
        }
    else if(checkUserExists(ADMIN, name))
        {
            checkcommand = "SELECT ID FROM Admins WHERE Username = \'" + name + "\' and Password = \'" + pass + "\';";
            usert = ADMIN;
        }
    else
        {
            setMessage(SQL_ERRGENERIC, "Invalid username or password\n");
            return;
        }
    

    sqlite3_stmt *selectstmt;
    rc = sqlite3_prepare_v2(db, checkcommand.c_str(), -1, &selectstmt, NULL);

    if(rc == SQLITE_OK)
    {
        if(sqlite3_step(selectstmt) == SQLITE_ROW)
        {
            user.type = usert;
            user.LOGGEDIN = true;
            user.ID = (char*)sqlite3_column_text(selectstmt, 0);
            if(usert == USER)
                setMessage(SQL_USRLOGINSUCCESS, "Login successful");
            else
                setMessage(SQL_ADMINLOGINSUCCESS, "Logged in as admin");
        } 
        else
        {
            setMessage(SQL_ERRGENERIC, "Invalid username or password");
        }
        sqlite3_finalize(selectstmt);
    }
}

void SQLQuery::submitSong(std::string name, std::string desc, std::string tags, std::string link)
{
    if(checkSongExists(link))
    {
        setMessage(SQL_ERRGENERIC, "Error: Song is already in list");
        return;
    }

    if(name.size()>0 && desc.size()>0 && tags.size()>0 && link.size()>0)
    {
        std::string addcommand = "INSERT INTO Submitted_Songs(Name, Description, Tags, Link) VALUES (\'" + name + "\', \'" + desc + "\', \'" + tags + "\', \'" + link + "\');";
        
        rc = sqlite3_exec(db, addcommand.c_str(), callback, 0, &szErrMsg);
        if(rc != SQLITE_OK)
        {
            std::cout << "SQL Error: " << szErrMsg << std::endl;
            sqlite3_free(szErrMsg);
            setMessage(SQL_ERRGENERIC, "Error: Submission failed");
        }
        else
        {
            setMessage(SQL_SONGSUBMITSUCCESS, "Song submitted successfully");
        }
    }
    else
    {
        setMessage(SQL_ERRGENERIC, "Error: Entry must have a name, a description, tags and a link");
    }

    

}

void SQLQuery::approveSong(std::string submitted_id)
{
    std::string command = "INSERT INTO Songs(Name, Description, Tags, Link) SELECT Name, Description, Tags, Link FROM Submitted_Songs WHERE ID=" + submitted_id + ";";
    
    rc = sqlite3_exec(db, command.c_str(), callback, 0, &szErrMsg);
    if(rc != SQLITE_OK)
    {
        setMessage(SQL_ERRGENERIC, "Error: Approval failed\n");
    }
    else
    {
        deleteSubmission(submitted_id);
    }
      
}

void SQLQuery::deleteSubmission(std::string submitted_id)
{
    std::string command = "DELETE FROM Submitted_Songs WHERE ID=" + submitted_id + ";";
    rc = sqlite3_exec(db, command.c_str(), callback, 0, &szErrMsg);
    if(rc != SQLITE_OK)
    {
        setMessage(SQL_ERRGENERIC, "Warning: Failed to delete song ID=" + submitted_id);
    } 
    else
    {
        setMessage(SQL_DELETESUBMSUCCESS, "Cleared submission\n");
    }  
}

void SQLQuery::deleteSong(std::string song_id)
{
    std::string command = "DELETE FROM Songs WHERE ID=" + song_id + ";";
    rc = sqlite3_exec(db, command.c_str(), callback, 0, &szErrMsg);
    if(rc != SQLITE_OK)
    {
        setMessage(SQL_ERRGENERIC, "Warning: Failed to delete song ID=" + song_id);
    } 
    else
    {
        setMessage(SQL_DELETESUBMSUCCESS, "Deleted song\n");
    }  
}

void SQLQuery::addVote(std::string user_id, std::string song_id, std::string vote_value)
{
    if(vote_value!="up" && vote_value!="down")
    {
        setMessage(SQL_ERRGENERIC, "Invalid vote");
        return;
    }
    if(checkUserSuspended(user_id)==false)
    {
        if(checkSongIDExists(song_id))
        {
            if(checkVoteExists(user_id, song_id))
            {
                setMessage(SQL_ERRGENERIC, "Song already voted");
                return;
            }

            std::string voteval;
            if(vote_value=="up")
                voteval="1";
            else
                voteval="-1";
            std::string command = "INSERT INTO Votes (USERID, SONGID, Vote) VALUES (" + user_id + "," + song_id + "," + voteval + ");";

            int rc = sqlite3_exec(db, command.c_str(), callback, 0, &szErrMsg);
            if(rc != SQLITE_OK)
            {
                printf("error: %s", sqlite3_errmsg(db));
                setMessage(SQL_ERRGENERIC, "Error: Vote failed");
            }
            else
            {
                setMessage(SQL_VOTESUCCESS, "Vote registered");
                updateScores();
            }
        }
        else
        {
            setMessage(SQL_ERRGENERIC, "Invalid song ID");
        }
    }
    else
    {
        setMessage(SQL_ERRGENERIC, "Error: This user is suspended from voting");
    }
    
    
}

void SQLQuery::addComment(std::string user_id, std::string song_id, std::string comment)
{
    if(comment.size()<1)
    {
        setMessage(SQL_ERRGENERIC, "Error: Comment is empty");
        return;
    }

    if(checkSongIDExists(song_id))
    {
        if(checkCommentExists(user_id, song_id))
        {
            setMessage(SQL_ERRGENERIC, "You already commented on this song");
            return;
        }

        std::string command = "INSERT INTO Comments (USERID, SONGID, Comment) VALUES (" + user_id + "," + song_id + ",\'" + comment + "\');";
    
        int rc = sqlite3_exec(db, command.c_str(), callback, 0, &szErrMsg);
        if(rc != SQLITE_OK)
        {
            printf("error: %s", sqlite3_errmsg(db));
            setMessage(SQL_ERRGENERIC, "Error: Comment failed");
        }
        else
        {
            setMessage(SQL_VOTESUCCESS, "Comment registered");
            updateScores();
        }
    }
    else
    {
        setMessage(SQL_ERRGENERIC, "Invalid song ID");
    }
    
}

void SQLQuery::findTags(std::vector<std::string> &tags)
{
    sqlite3_stmt *stmt;
    std::string command = "SELECT * FROM Songs WHERE ";

    int i;
    for(i=0;i<tags.size();i++)
    {
        if(i!=0)
            command += " or ";
        command += "Tags like (\'%" + tags[i] + "%\')";
    }
    command += "ORDER BY Score LIMIT 5;";

    int rc = sqlite3_prepare_v2(db, command.c_str(), -1, &stmt, NULL);
    if(rc != SQLITE_OK)
    {
        printf("error: %s", sqlite3_errmsg(db));
        setMessage(SQL_ERRGENERIC, "Error: Could not find tags");
    }
    else
    {
        char result[2000];
        getQueryResult(stmt, result);
        setMessage(SQL_NULL, result);
    }
    sqlite3_finalize(stmt);
}

void SQLQuery::setReputation(std::string name, std::string rep_value)
{//set reputation for a user 0=neutral 1=vote suspended 2=banned 
    if(checkUserExists(USER, name))
    {
        std::string command = "UPDATE Users SET Reputation=" + rep_value + " WHERE username=\'" + name +"\';";

        int rc = sqlite3_exec(db, command.c_str(), callback, 0, &szErrMsg);
        if(rc != SQLITE_OK)
        {
            printf("SQLerror:%s\n", sqlite3_errmsg(db));
            setMessage(SQL_ERRGENERIC, "Error: Could not set reputation, check rep value (0 normal, 1 suspended, 2 banned)");
        }
        else
        {
            setMessage(SQL_SETREPSUCCESS, "Changed user reputation"); 
        }
    }
    else
    {
        setMessage(SQL_ERRGENERIC, "Error: Could not find user");
    }
}

void SQLQuery::listSubmissions()
{
    sqlite3_stmt *stmt;
    char command[] = "SELECT * FROM Submitted_Songs LIMIT 5;";
    
    int rc = sqlite3_prepare_v2(db, command, -1, &stmt, NULL);
    if(rc != SQLITE_OK)
    {
        printf("error: %s", sqlite3_errmsg(db));
        setMessage(SQL_ERRGENERIC, "Error: List failed");
    }
    else
    {
        char result[500];
        getQueryResult(stmt, result);
        setMessage(SQL_NULL, result);
    }
    sqlite3_finalize(stmt);
}

void SQLQuery::listAll()
{
    sqlite3_stmt *stmt;
    char command[] = "SELECT * FROM Songs;";
    
    int rc = sqlite3_prepare_v2(db, command, -1, &stmt, NULL);
    if(rc != SQLITE_OK)
    {
        printf("error: %s", sqlite3_errmsg(db));
        setMessage(SQL_ERRGENERIC, "Error: List failed");
    }
    else
    {
        char result[MSG_BUFSIZE];
        getQueryResult(stmt, result);
        setMessage(SQL_NULL, result);
    }
    sqlite3_finalize(stmt);
}

void SQLQuery::listTop()
{
    sqlite3_stmt *stmt;
    char command[] = "SELECT * FROM Songs ORDER BY Score DESC LIMIT 5;";
    
    int rc = sqlite3_prepare_v2(db, command, -1, &stmt, NULL);
    if(rc != SQLITE_OK)
    {
        printf("error: %s", sqlite3_errmsg(db));
        setMessage(SQL_ERRGENERIC, "Error: List failed");
    }
    else
    {
        char result[MSG_BUFSIZE];
        getQueryResult(stmt, result);
        setMessage(SQL_NULL, result);
    }
    sqlite3_finalize(stmt);
}

void SQLQuery::listComments(std::string song_id)
{
    sqlite3_stmt *stmt;
    std::string command = "select u.username, c.Comment from Users u join Comments c on c.SONGID=" + song_id + " and u.ID=c.USERID;";

    int rc = sqlite3_prepare_v2(db, command.c_str(), -1, &stmt, NULL);
    if(rc != SQLITE_OK)
    {
        printf("error: %s", sqlite3_errmsg(db));
        setMessage(SQL_ERRGENERIC, "Error: Song has no comments");
    }
    else
    {
        char result[2000];
        getQueryResult(stmt, result);
        setMessage(SQL_NULL, result);
    }
    sqlite3_finalize(stmt);
}

void SQLQuery::listUserInfo(std::string username)
{
    if(checkUserExists(USER, username))
    {
        sqlite3_stmt *stmt;
        std::string command = "SELECT ID, username, Reputation FROM Users where username=\'" + username + "\';";

        int rc = sqlite3_prepare_v2(db, command.c_str(), -1, &stmt, NULL);
        if(rc != SQLITE_OK)
        {
            printf("error: %s", sqlite3_errmsg(db));
            setMessage(SQL_ERRGENERIC, "Error: Could not list information for user");
        }
        else
        {
            char result[MSG_BUFSIZE];
            getQueryResult(stmt, result);
            setMessage(SQL_NULL, result);
        }
        sqlite3_finalize(stmt);
    }
    else
    {
        setMessage(SQL_ERRGENERIC, "Error: Could not find user");
    }
    
}

void SQLQuery::updateScores()
{
    char command[] = "UPDATE Songs SET Score = (SELECT sum(Votes.Vote) FROM Votes WHERE Songs.ID=Votes.SONGID);";
    sqlite3_stmt *stmt;

    int rc = sqlite3_prepare_v2(db, command, -1, &stmt, NULL);
    if(rc != SQLITE_OK)
    {
        printf("SQLerror: Failed to update songs scores -- %s\n", sqlite3_errmsg(db));
    }
    else
    {
        printf("Updated songs scores\n");
    }
    sqlite3_finalize(stmt);
}

void SQLQuery::getQueryResult(sqlite3_stmt *stmt, char result[])
{
    memset(result, '\0', 500);
    strcat(result, "\n");
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) 
    {
        for(int j=0;j<sqlite3_column_count(stmt);j++)
        {
            strcat(result, sqlite3_column_name(stmt, j));
            strcat(result, ": ");
            strcat(result, (char*)sqlite3_column_text(stmt, j));
            strcat(result, "\n");
        }
        strcat(result, "\n");
    }
}

void SQLQuery::setMessage(SQLMSG s, std::string msg)
{
    this->message = std::to_string(s) + ":" + msg;
}

std::string SQLQuery::getMessage()
{
    return this->message;
}

// This is the callback function to display the select data in the table
int SQLQuery::callback(void *NotUsed, int argc, char **argv, char **azColName) 
{
    return 0;
}

void SQLQuery::openDB()
{
    szErrMsg = 0;
    // open database
    rc = sqlite3_open_v2("topmusic.db", &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_URI, NULL);
    
    if(rc)
    {
        std::cout << "Can't open database -- Make sure 'topmusic.db' is in the same folder as the server\n";
        exit(0);
    } else {
        std::cout << "Opened database successfully\n";
    }
}

void SQLQuery::closeDB()
{
    // close database
    if(db)
    {
        sqlite3_close(db);
    }
}

SQLQuery::SQLQuery()
{
    openDB();
}

SQLQuery::~SQLQuery()
{
    closeDB();
}