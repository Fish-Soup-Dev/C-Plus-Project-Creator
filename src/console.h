#ifndef CONSOLE_H
#define CONSOLE_H

#include <iostream>
#include <tuple>
#include <string>

#if defined(_WIN32) || defined(_WIN64)
    #include <windows.h>
    #include <conio.h>
#elif defined(__linux__)
    // nothing
#elif defined(__APPLE__) || defined(__MACH__)
    // nothing
#endif

enum COLOR
{
    BLUE = 1,
    GREEN = 2,
    RED = 4,
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
    #if defined(_WIN32) || defined(_WIN64)
        HANDLE m_hConsole;
    #elif defined(__linux__)
        // nothing
    #elif defined(__APPLE__) || defined(__MACH__)
        // nothing
    #endif

    void SetCursorPosition(int XPos, int YPos);

    std::tuple<int, int> GetConsoleCursorPosition();

    int GetInput();
};

#endif