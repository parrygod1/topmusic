#include "sql_query.h"

void SQLQuery::addUser(std::string name, std::string pass)
{
    // prepare our sql statements
    std::string pSQL = "INSERT INTO Users(username, password) VALUES (" + name +" ," + pass + ";";

    // execute sql
    rc = sqlite3_exec(db, pSQL.c_str(), callback, 0, &szErrMsg);
    if(rc != SQLITE_OK)
    {
        std::cout << "SQL Error: " << szErrMsg << std::endl;
        sqlite3_free(szErrMsg);
    }

}

void SQLQuery::openDB()
{
    szErrMsg = 0;
    // open database
    rc = sqlite3_open("topmusic.db", &db);

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

// This is the callback function to display the select data in the table
int SQLQuery::callback(void *NotUsed, int argc, char **argv, char **azColName) {
   int i;
   for(i = 0; i<argc; i++) {
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}

SQLQuery::SQLQuery()
{
    openDB();
}

SQLQuery::~SQLQuery()
{
    closeDB();
}