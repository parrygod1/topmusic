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
            checkcommand = "SELECT password FROM Users WHERE Username = \'" + name + "\';";
            usert = USER;
        }
    else if(checkUserExists(ADMIN, name))
        {
            checkcommand = "SELECT password FROM Admins WHERE Username = \'" + name + "\';";
            usert = ADMIN;
        }
    else
        {
            setMessage(SQL_ERRGENERIC, "Invalid username or password");
            return;
        }
    

    struct sqlite3_stmt *selectstmt;
    rc = sqlite3_prepare_v2(db, checkcommand.c_str(), -1, &selectstmt, NULL);

    if(rc == SQLITE_OK)
    {
        if(sqlite3_step(selectstmt) == SQLITE_ROW)
        {
            user.type = usert;
            user.LOGGEDIN = true;
            if(usert == USER)
                setMessage(SQL_USRLOGINSUCCESS, "Login successful");
            else
                setMessage(SQL_ADMINLOGINSUCCESS, "Login successful");
            
        }
        else
        {
            setMessage(SQL_ERRGENERIC, "Invalid username or password");
        }
        sqlite3_finalize(selectstmt);
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