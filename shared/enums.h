#ifndef SHAREDENUMS_H
#define SHAREDENUMS_H
#include <map>
#define MSG_BUFSIZE 1000
#define MSG_STATLEN 4

enum SQLMSG 
{
    SQL_ERRGENERIC = 100, 
    SQL_LOGINSUCCESS = 101,
    SQL_REGSUCCESS = 102,
    SQL_NULL = 999
};

enum USRTYPE {USER, ADMIN};

enum CMDVAL 
{ 
    CMD_NULL,
    CMD_USEREXIT,
    CMD_USERCONNECT,
    CMD_USERDISCONNECT,
    CMD_LOGIN,
    CMD_USERREG,
    CMD_ADMINREG
    
};

// Map to associate the strings with the enum values
static std::map<std::string, CMDVAL> map_cmdval;

inline void initCmdmap()
{
    map_cmdval[""] = CMD_NULL;
    map_cmdval["exit"] = CMD_USEREXIT;
    map_cmdval["conn"] = CMD_USERCONNECT;
    map_cmdval["disconn"] = CMD_USERDISCONNECT;
    map_cmdval["login"] = CMD_LOGIN;
    map_cmdval["reg"] = CMD_USERREG;
    map_cmdval["areg"] = CMD_ADMINREG;
    
}

#endif