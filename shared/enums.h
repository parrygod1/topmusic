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
    SQL_SONGSUBMITSUCCESS = 105,
    SQL_SONGAPPROVESUCCESS = 106,
    SQL_DELETESUBMSUCCESS = 107,
    SQL_NULL = 999
};

enum CMDVAL 
{ //cmd_null is the default case when a command is not recognized
    CMD_NULL,
    CMD_USEREXIT,
    CMD_USERCONNECT,
    CMD_USERDISCONNECT,
    CMD_LOGIN,
    CMD_LOGOUT,
    CMD_USERREG,
    CMD_ADMINREG,
    CMD_SUBMITSONG,
    CMD_APPROVESONG,
    CMD_DELETESUBM,
    CMD_LIST   
};

static std::map<std::string, CMDVAL> map_cmdval;

inline void initCmdmap()
{
    
    map_cmdval[""] = CMD_NULL; 
    map_cmdval["exit"] = CMD_USEREXIT;
    map_cmdval["conn"] = CMD_USERCONNECT;
    map_cmdval["disconn"] = CMD_USERDISCONNECT;
    map_cmdval["login"] = CMD_LOGIN;
    map_cmdval["logout"] = CMD_LOGOUT;
    map_cmdval["reg"] = CMD_USERREG;
    map_cmdval["areg"] = CMD_ADMINREG;
    map_cmdval["submsong"] = CMD_SUBMITSONG;
    map_cmdval["appvsong"] = CMD_APPROVESONG;
    map_cmdval["delsubm"] = CMD_DELETESUBM;
    map_cmdval["list"] = CMD_LIST;
    
}

#endif