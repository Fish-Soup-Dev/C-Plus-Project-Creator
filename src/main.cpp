#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <windows.h>
#include <conio.h>

#include "logo.hpp"

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

void MakeProgramFiles(std::string name)
{
    std::string project_path = "./" + name;
    std::filesystem::create_directory(project_path);
    std::filesystem::create_directory(project_path + "/src");
    std::filesystem::create_directory(project_path + "/lib");
    std::filesystem::create_directory(project_path + "/bin");
    std::filesystem::create_directory(project_path + "/obj");
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
R"(CXX = g++
MAIN = bin/)" << name + R"(.exe
CFLAGS = -Wall -std=c++17
LIBS =
DEFS = 

SRCS = $(wildcard src/*.cpp)
SLIBS = $(wildcard lib/*.a)
INCDIR = ./include
SLIBDIR = ./lib
OBJDIR = ./obj
BINDIR = ./bin

OBJS = $(patsubst %.cpp, $(OBJDIR)/%.o, $(notdir $(SRCS)))

all: $(MAIN)

$(MAIN): $(OBJS)
	@mkdir -p $(BINDIR)
	$(CXX) $(CFLAGS) -o $@ $^ $(addprefix -I, $(INCDIR)) $(addprefix -L, $(SLIBDIR)) $(SLIBS) $(addprefix -D, $(DEFS)) $(LIBS)

$(OBJDIR)/%.o: src/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CFLAGS) -c -o $@ $< $(addprefix -D, $(DEFS)) $(addprefix -I, $(INCDIR))

clean:
	rm -rf $(OBJDIR) $(MAIN)

run:
	$(MAIN))";
    ofs_make.close();
}

int main(int argc, char *argv[])
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
    std::cout << logo << std::endl;

    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
    std::cout << "C++ Project Genrator" << std::endl;
    std::cout << "alpha v0.2 8:28AM 8/30/2024" << std::endl;
    SetConsoleTextAttribute(hConsole, 7);

    std::cout << "Enter project name" << std::endl;
    std::string name;
    std::getline(std::cin, name);

    std::string options[1] = { "Empty" };
    int result = Menu(hConsole, 1, options, "Project Type?");

    switch (result)
    {
    case 0:
        std::cout << "Making project..." << std::endl; 
        MakeProgramFiles(name);
        break;
    
    default:
        break;
    }

    return EXIT_SUCCESS;
}