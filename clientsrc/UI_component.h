#ifndef INTERFACECOMPONENT_H
#define INTERFACECOMPONENT_H
#include <iostream>
#include <cstring>
#include <termios.h>
#include <unistd.h>
#include "libs/getch.h"
#include "../shared/enums.h"

enum UISTATE
{
    UI_CONNECT,
    UI_LOGINMENU,
    UI_LOGININFO,
};

class UIComponent
{
    private:
    char entered_key;
    
    CMDVAL selected_command = CMD_NULL;

    public:
    char buffer[MSG_BUFSIZE];    

    void printPrompt();
    void exec_ConnectionMenu();
    void exec_LoginMenu();
    void exec_LoginInfo();
    void exec_RegUserInfo();
    void exec_RegAdminInfo();
    void getInput(); 
    void resetCommand();
    CMDVAL getCommand();

    UIComponent();
    ~UIComponent();
};

#endif