#include <iostream>
#include <filesystem>
#include <algorithm>
#include <fstream>
#include <string>
#include <windows.h>
#include <conio.h>

#include "logo.h"
#include "template_makefile_genorator.h"

void SetCursorPosition(int XPos, int YPos)
{
   COORD coord;
   coord.X = XPos; coord.Y = YPos;
   SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

COORD GetConsoleCursorPosition(HANDLE hConsoleOutput)
{
    CONSOLE_SCREEN_BUFFER_INFO cbsi;
    if (GetConsoleScreenBufferInfo(hConsoleOutput, &cbsi))
    {
        return cbsi.dwCursorPosition;
    }
    else
    {
        // The function failed. Call GetLastError() for details.
        COORD invalid = { 0, 0 };
        return invalid;
    }
}

int GetInput()
{
    int key;

    key = _getch();

    if (key == 27)
        return -1;
    
    if (key == 13)
        return 5;

    if (key != 224)
        return 0;
    
    key = _getch();

    switch (key)
    {
    case 72: // up key
        return 1;
        break;
    case 77: // right key
        return 2;
        break;
    case 80: // donw key
        return 3;
        break;
    case 75: // left key
        return 4;
        break;

    default:
        break;
    }

    return 0;
}

int Menu(HANDLE hConsole, int numOfOptions, std::string options[64], std::string mesage)
{
    int slected = 0;

    std::cout << mesage << std::endl;

    for (int i = 0; i < numOfOptions; i++)
    {
        SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);

        if (slected == i)
        {
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
            std::cout << "> ";
        }

        std::cout << options[i] << "                         " << std::endl;

        SetConsoleTextAttribute(hConsole, 7);
    }

    while (1)
    {
        int key = GetInput();
        
        switch (key)
        {
        case -1:
            return -1;
            break;
        case 1:
            if (slected - 1 != -1)
                slected--;
            break;
        case 3:
            if (slected + 1 != numOfOptions)
                slected++;
            break;
        case 5:
            return slected;
            break;
        
        default:
            continue;
            break;
        }

        int y = GetConsoleCursorPosition(hConsole).Y;

        SetCursorPosition(0, y - numOfOptions);

        for (int i = 0; i < numOfOptions; i++)
        {
            SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);

            if (slected == i)
            {
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
                std::cout << "> ";
            }

            std::cout << options[i] << "                         " << std::endl;

            SetConsoleTextAttribute(hConsole, 7);
        }
    }
}

void MakeProgramFiles(std::string name, int type)
{
    std::string project_path = "./" + name;
    std::filesystem::create_directory(project_path);
    std::filesystem::create_directory(project_path + "/src");
    std::filesystem::create_directory(project_path + "/lib");
    std::filesystem::create_directory(project_path + "/include");

    if (type == 0)
    {
        std::ofstream ofs_main(project_path + "/src/main.cpp");
        ofs_main << 
R"(#include <iostream>

int main(int argc, char *argv[])
{
    std::cout << "it works" << std::endl;
    return 0;
})";
        ofs_main.close();
    }
    else if (type == 1)
    {
        std::string nameupper = name;
        std::transform(nameupper.begin(), nameupper.end(), nameupper.begin(), ::toupper);
        std::ofstream ofs_main(project_path + "/src/" + name + ".h");
        ofs_main << 
R"(#ifndef )" + nameupper + R"(_HPP
#define )" + nameupper + R"(_HPP

#ifdef __cplusplus
    extern "C" {
#endif

#ifdef BUILD_DLL
    #define )" + nameupper + R"( __declspec(dllexport)
#else
    #define )" + nameupper + R"( __declspec(dllimport)
#endif

// functions here

#ifdef __cplusplus
    }
#endif

// or here

#endif)";

        ofs_main.close();

        std::ofstream ofs_main1(project_path + "/src/" + name +".cpp");
        ofs_main1 << 
R"(#include ")" + name + R"(.h"
)";
        ofs_main1.close();
    }


    std::ofstream ofs_make(project_path + "/makefile");

    if (type == 0)
        ofs_make << basicMakefile(name);
    else if (type == 1)
        ofs_make << dllMakefile(name);

    ofs_make.close();
}

int main(int argc, char *argv[])
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
    std::cout << logo << std::endl;

    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
    std::cout << "C++ Project Genrator" << std::endl;
    std::cout << "alpha v0.3.1 8:02PM 9/2/2024" << std::endl;
    SetConsoleTextAttribute(hConsole, 7);

    std::cout << "Enter project name" << std::endl;
    std::string name;
    std::getline(std::cin, name);

    std::string options[2] = { "Empty", "DLL" };
    int result = Menu(hConsole, 2, options, "Project Type?");

    switch (result)
    {
    case 0:
        std::cout << "Making Empty project..." << std::endl; 
        MakeProgramFiles(name, result);
        break;
    case 1:
        std::cout << "Making DLL project..." << std::endl; 
        MakeProgramFiles(name, result);
        break;
    
    default:
        break;
    }

    return EXIT_SUCCESS;
}