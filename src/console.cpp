#include "console.h"

Console::Console()
{
    m_hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
}

Console::~Console()
{

}

void Console::print(std::string text, COLOR textColor)
{
    SetConsoleTextAttribute(m_hConsole, textColor);
    std::cout << text << std::endl;
    SetConsoleTextAttribute(m_hConsole, 7);
}

int Console::menu(int numOfOptions, std::string options[64], std::string mesage)
{
    int slected = 0;

    std::cout << mesage << std::endl;

    for (int i = 0; i < numOfOptions; i++)
    {
        SetConsoleTextAttribute(m_hConsole, FOREGROUND_BLUE);

        if (slected == i)
        {
            SetConsoleTextAttribute(m_hConsole, FOREGROUND_RED);
            std::cout << "> ";
        }

        std::cout << options[i] << "                         " << std::endl;

        SetConsoleTextAttribute(m_hConsole, 7);
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

        int y = GetConsoleCursorPosition(m_hConsole).Y;

        SetCursorPosition(0, y - numOfOptions);

        for (int i = 0; i < numOfOptions; i++)
        {
            SetConsoleTextAttribute(m_hConsole, FOREGROUND_BLUE);

            if (slected == i)
            {
                SetConsoleTextAttribute(m_hConsole, FOREGROUND_RED);
                std::cout << "> ";
            }

            std::cout << options[i] << "                         " << std::endl;

            SetConsoleTextAttribute(m_hConsole, 7);
        }
    }
}

void Console::SetCursorPosition(int XPos, int YPos)
{
    COORD coord;
    coord.X = XPos; coord.Y = YPos;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

COORD Console::GetConsoleCursorPosition(HANDLE hConsoleOutput)
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

int Console::GetInput()
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