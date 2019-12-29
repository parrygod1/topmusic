#include "UI_component.h"

void UIComponent::printPrompt()
{
    printf("> ");
}

void UIComponent::exec_ConnectionMenu()
{
    printf("Not connected to server\n");
    printf("1. Connect\n");
    printf("2. Exit\n");
    getInput();

    if(entered_key=='1')
        selected_command = CMD_USERCONNECT;
    else if(entered_key=='2')
        selected_command = CMD_USEREXIT;
}

void UIComponent::exec_LoginMenu()
{
    printf("Connected to server\n");
    printf("1. Login\n");
    printf("2. Register\n");
    printf("3. Register Admin\n");
    printf("4. Disconnect\n");
    printf("5. Exit");

    if(entered_key=='1')
        exec_LoginInfo();
    //else if(entered_key=='2')
        //exec_RegUserInfo();
   // else if(entered_key=='3')
        //exec_RegAdminInfo();
    else if(entered_key=='4')
        selected_command = CMD_USERDISCONNECT;
    else if (entered_key=='5')
        selected_command = CMD_USEREXIT;
}

void UIComponent::exec_LoginInfo()
{
    memset(buffer, '\0', MSG_BUFSIZE);
    strcat(buffer, "login ");

    char temp[50];
    printf("Username: ");
    std::cin.getline(temp, 50);

    strcat(buffer, temp);
    memset(temp, '\0', 50);

    termios oldt;
    tcgetattr(STDIN_FILENO, &oldt);
    termios newt = oldt;
    newt.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    printf("Password: ");
    std::cin.getline(temp, 50);

    strcat(buffer, temp);
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
}

void UIComponent::resetCommand()
{
    this->selected_command = CMD_NULL;
}

CMDVAL UIComponent::getCommand()
{
    return this->selected_command;
}


void UIComponent::getInput()
{
    entered_key = getch();
}

UIComponent::UIComponent()
{
}

UIComponent::~UIComponent()
{
}