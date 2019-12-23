#ifndef INTERFACECOMPONENT_H
#define INTERFACECOMPONENT_H
#include <iostream>
#include "libs/getch.h"

enum UIState{UI_EXIT};

class UIComponent
{
    private:
    char key;
    
    public:
    UIState ui_current_state;
    void printMenu();
    void getInput();
};

#endif