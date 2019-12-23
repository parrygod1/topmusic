#include "sql_query.h"

enum msgreturnstatus {SQL_ERRGENERIC = 0, SQL_SUCCESS = 1} msgresturnstatus;

bool SQLQuery::checkAdminExists(std::string name)
{
    std::string checkcommand = "SELECT ID FROM Admins WHERE Username = \'" + name + "\';";
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

bool SQLQuery::checkUserExists(std::string name)
{
    std::string checkcommand = "SELECT ID FROM USERS WHERE Username = \'" + name + "\';";
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

void SQLQuery::addUser(std::string name, std::string pass, std::string *message)
{
    if(checkUserExists(name) || checkAdminExists(name))
    {
        *message = "Error: User already exists";
        return;
    }
    else if(!(std::any_of(std::begin(name), std::end(name), ::isalpha)))
    {
        *message = "Error: Invalid username -- only alphanumeric characters allowed";
        return;
    }
    else if(pass.size()<4)
    {
        *message = "Error: Invalid password -- password must have at least 4 characters";
        return;
    }

    // prepare our sql statements
    std::string addcommand = "INSERT INTO Users(username, password) VALUES (\'" + name + "\', \'" + pass + "\');";

    // execute sql
    rc = sqlite3_exec(db, addcommand.c_str(), callback, 0, &szErrMsg);
    if(rc != SQLITE_OK)
    {
        std::cout << "SQL Error: " << szErrMsg << std::endl;
        sqlite3_free(szErrMsg);
        *message = "Error: Register failed";
    }
    else
    {
        std::cout << "SQL: reg successful \n" << szErrMsg;
        *message = "User registered successfully";
    }
}

void SQLQuery::addAdmin(std::string name, std::string pass, std::string *message)
{
    if(checkUserExists(name) || checkAdminExists(name))
    {
        *message = "Error: User already exists";
        return;
    }
    else if(!(std::any_of(std::begin(name), std::end(name), ::isalpha)))
    {
        *message = "Error: Invalid username -- only alphanumeric characters allowed";
        return;
    }
    else if(pass.size()<4)
    {
        *message = "Error: Invalid password -- password must have at least 4 characters";
        return;
    }

    // prepare our sql statements
    std::string addcommand = "INSERT INTO Admins(username, password) VALUES (\'" + name + "\', \'" + pass + "\');";

    // execute sql
    rc = sqlite3_exec(db, addcommand.c_str(), callback, 0, &szErrMsg);
    if(rc != SQLITE_OK)
    {
        std::cout << "SQL Error: " << szErrMsg << std::endl;
        sqlite3_free(szErrMsg);
        *message = "Error: Register failed";
    }
    else
    {
        std::cout << "SQL: adminreg successful \n" << szErrMsg;
        *message = "Admin registered successfully";
    }
}

void SQLQuery::loginUser(std::string name, std::string pass, std::string *message)
{
    std::string checkcommand = "SELECT password FROM Users WHERE Username = \'" + name + "\';";
    struct sqlite3_stmt *selectstmt;
    rc = sqlite3_prepare_v2(db, checkcommand.c_str(), -1, &selectstmt, NULL);
    if(rc == SQLITE_OK)
    {
        if(sqlite3_step(selectstmt) == SQLITE_ROW)
        {
            *message = SQL_SUCCESS + ":Login successful";
        }
        sqlite3_finalize(selectstmt);
    }
    else
    {
        *message = SQL_ERRGENERIC + ":Invalid username or password";
    }
}

// This is the callback function to display the select data in the table
int SQLQuery::callback(void *NotUsed, int argc, char **argv, char **azColName) {
   int i;
   for(i = 0; i<argc; i++) {
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}


void SQLQuery::openDB()
{
    szErrMsg = 0;
    // open database
    rc = sqlite3_open_v2("topmusic.db", &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_URI, NULL);
    
    if(rc)
    {
        std::cout << "Can't open database\n";
    } else {
        std::cout << "Open database successfully\n";
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