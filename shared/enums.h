#ifndef SHAREDENUMS_H
#define SHAREDENUMS_H
#include <map>

#define MSG_BUFSIZE 1000
#define MSG_STATLEN 4

enum USRTYPE {USER, ADMIN};

struct userData
{
  char *ip;
  bool CONNECTED;
  bool LOGGEDIN;
  USRTYPE type;
};

enum SQLMSG 
{
    SQL_ERRGENERIC = 100, 
    SQL_USRLOGINSUCCESS = 101,
    SQL_ADMINLOGINSUCCESS = 102,
    SQL_REGSUCCESS = 103,
    SQL_LOGOUTSUCCESS = 104,
    SQL_NULL = 999
};

enum CMDVAL 
{ 
    CMD_NULL,
    CMD_USEREXIT,
    CMD_USERCONNECT,
    CMD_USERDISCONNECT,
    CMD_LOGIN,
    CMD_LOGOUT,
    CMD_USERREG,
    CMD_ADMINREG
    
};

static std::map<std::string, CMDVAL> map_cmdval;

inline void initCmdmap()
{
    //cmd_null is the default case when a command is not recognized
    map_cmdval[""] = CMD_NULL; 
    map_cmdval["exit"] = CMD_USEREXIT;
    map_cmdval["conn"] = CMD_USERCONNECT;
    map_cmdval["disconn"] = CMD_USERDISCONNECT;
    map_cmdval["login"] = CMD_LOGIN;
    map_cmdval["logout"] = CMD_LOGOUT;
    map_cmdval["reg"] = CMD_USERREG;
    map_cmdval["areg"] = CMD_ADMINREG;
    
}

#endif