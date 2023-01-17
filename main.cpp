#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <windows.h>
#include <conio.h>

int main_color = 9;
int slected_color = 12;

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
        SetConsoleTextAttribute(hConsole, main_color);

        if (slected == i)
        {
            SetConsoleTextAttribute(hConsole, slected_color);
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
            SetConsoleTextAttribute(hConsole, main_color);

            if (slected == i)
            {
                SetConsoleTextAttribute(hConsole, slected_color);
                std::cout << "> ";
            }

            std::cout << options[i] << "                         " << std::endl;

            SetConsoleTextAttribute(hConsole, 7);
        }
    }
}

void MakeProjectFiles(std::string name, int type)
{
    std::string project_path = "./" + name;
    std::filesystem::create_directory(project_path);
    std::filesystem::create_directory(project_path + "/src");
    std::filesystem::create_directory(project_path + "/lib");
    std::filesystem::create_directory(project_path + "/bin");
    std::filesystem::create_directory(project_path + "/out");
    std::filesystem::create_directory(project_path + "/include");

    std::ofstream ofs_main(project_path + "/src/main.cpp");
    ofs_main << 
R"(#include <iostream>

int main(int argc, char *argv[])
{
    std::cout << "it works" << std::endl;
    return 0;
})";
    ofs_main.close();

    std::ofstream ofs_make(project_path + "/makefile");
    ofs_make <<
R"(CC = g++
MAIN = bin/main.exe
CFLAGS = -std=c++17 -Wall
DEFS = 
LIBS =

SRCS = $(wildcard src/*.cpp)
SLIBDIR = ./lib
SLIBS = $(wildcard lib/*.a)
INCDIR = ./include
OBJDIR = ./out

OBJS = $(patsubst %.cpp, $(OBJDIR)/%.o, $(notdir $(SRCS)))

all: $(MAIN)

$(MAIN): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(addprefix -I, $(INCDIR)) $(addprefix -L, $(SLIBDIR)) $(SLIBS) $(addprefix -D, $(DEFS)) $(LIBS)

$(OBJDIR)/%.o: src/%.cpp
	$(CC) $(CFLAGS) -c -o $@ $< $(addprefix -D, $(DEFS)) $(addprefix -I, $(INCDIR))

clean:
	del out\\*.o
	del bin\\*.exe

run:
	$(MAIN))";
    ofs_make.close();
}

int main(int argc, char *argv[])
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, main_color);

    std::cout << R"(
________/\\\\\\\\\________________________________________/\\\\\\\\\\\\\_______/\\\\\\\\\\\\_        
 _____/\\\////////________________________________________\/\\\/////////\\\___/\\\//////////__       
  ___/\\\/________________/\\\__________/\\\_______________\/\\\_______\/\\\__/\\\_____________      
   __/\\\_________________\/\\\_________\/\\\_______________\/\\\\\\\\\\\\\/__\/\\\____/\\\\\\\_     
    _\/\\\______________/\\\\\\\\\\\__/\\\\\\\\\\\___________\/\\\/////////____\/\\\___\/////\\\_    
     _\//\\\____________\/////\\\///__\/////\\\///____________\/\\\_____________\/\\\_______\/\\\_   
      __\///\\\______________\/\\\_________\/\\\_______________\/\\\_____________\/\\\_______\/\\\_  
       ____\////\\\\\\\\\_____\///__________\///________________\/\\\_____________\//\\\\\\\\\\\\/__ 
        _______\/////////________________________________________\///_______________\////////////____
        )" << std::endl;

    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
    std::cout << "C++ Project Genrator" << std::endl;
    std::cout << "build v0.1 5:16PM 1/15/2023" << std::endl;

    SetConsoleTextAttribute(hConsole, 7);

    std::cout << "Enter project name" << std::endl;
    std::string name;
    std::getline(std::cin, name);

    std::string code_types[3] = { "Program", "Dll", "Static lib" };
    int code_type = Menu(hConsole, 3, code_types, "Project Type?");

    if (code_type == -1)
        return 0;
    
    std::cout << "Making project..." << std::endl; 

    MakeProjectFiles(name, code_type);

    std::cout << "Done!" << std::endl;

    return 0;
}