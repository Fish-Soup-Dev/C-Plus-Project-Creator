#ifndef CONSOLE_H
#define CONSOLE_H

#include <iostream>
#include <string>
#include <windows.h>
#include <conio.h>

enum COLOR
{
    BLUE = FOREGROUND_BLUE,
    GREEN = FOREGROUND_GREEN,
    RED = FOREGROUND_RED,
    WHITE = 7,
};

class Console
{
public:
    Console();
    ~Console();

    void print(std::string text, COLOR textColor);
    int menu(int numOfOptions, std::string options[64], std::string mesage);

private:
    HANDLE m_hConsole;

    void SetCursorPosition(int XPos, int YPos);
    COORD GetConsoleCursorPosition(HANDLE hConsoleOutput);
    int GetInput();
};

#endif