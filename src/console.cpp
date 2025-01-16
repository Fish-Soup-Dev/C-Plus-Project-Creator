#include "console.h"

Console::Console()
{
    #if defined(_WIN32) || defined(_WIN64)
        m_hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    #elif defined(__linux__)
        // nothing
    #elif defined(__APPLE__) || defined(__MACH__)
        // nothing
    #endif
}

Console::~Console()
{

}

void Console::print(std::string text, COLOR textColor)
{
    setColor(textColor);
    std::cout << text << std::endl;
    setColor(COLOR::WHITE);
}

int Console::menu(int numOfOptions, std::string options[64], std::string mesage)
{
    int slected = 0;

    std::cout << mesage << std::endl;

    for (int i = 0; i < numOfOptions; i++)
    {
        setColor(COLOR::BLUE);

        if (slected == i)
        {
            setColor(COLOR::RED);
            std::cout << "> ";
        }

        std::cout << options[i] << "                         " << std::endl;

        setColor(COLOR::WHITE);
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

        int y = std::get<1>(GetConsoleCursorPosition());

        SetCursorPosition(0, y - numOfOptions);

        for (int i = 0; i < numOfOptions; i++)
        {
            setColor(COLOR::BLUE);

            if (slected == i)
            {
                setColor(COLOR::RED);
                std::cout << "> ";
            }

            std::cout << options[i] << "                         " << std::endl;

            setColor(COLOR::WHITE);
        }
    }
}

void Console::SetCursorPosition(int XPos, int YPos)
{
    #if defined(_WIN32) || defined(_WIN64)
        COORD coord;
        coord.X = XPos; coord.Y = YPos;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    #elif defined(__linux__)
        std::cout << "\033[" << (YPos + 1) << ";" << (XPos + 1) << "H";  // 1-based index in ANSI escape codes
        std::flush(std::cout);
    #elif defined(__APPLE__) || defined(__MACH__)
        // nothing
    #endif
}

std::tuple<int, int> Console::GetConsoleCursorPosition()
{
    #if defined(_WIN32) || defined(_WIN64)
        CONSOLE_SCREEN_BUFFER_INFO cbsi;

        if (GetConsoleScreenBufferInfo(m_hConsole, &cbsi))
            return std::make_tuple(cbsi.dwCursorPosition.X, cbsi.dwCursorPosition.Y);
        else
            return std::make_tuple(0, 0);
    #elif defined(__linux__)
        std::cout << "\033[6n"; // Send escape sequence to get cursor position
        std::flush(std::cout);

        char buf[32];
        if (fgets(buf, sizeof(buf), stdin)) {
            int row, col;
            // Parsing the response (ESC [ row;col R)
            if (sscanf(buf, "\033[%d;%dR", &row, &col) == 2) {
                return std::make_tuple(col - 1, row - 1);  // Return 0-based indexing
            }
        }
        return std::make_tuple(0, 0);  // Default if position cannot be fetched
    #elif defined(__APPLE__) || defined(__MACH__)
        // nothing
    #endif
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

void Console::setColor(COLOR color)
{
    #if defined(_WIN32) || defined(_WIN64)
        SetConsoleTextAttribute(m_hConsole, color);
    #elif defined(__linux__)
        switch (color)
        {
        case COLOR::BLUE:
            std::cout << "\033[34m";
            break;
        case COLOR::GREEN:
            std::cout << "\033[32m";
            break;
        case COLOR::RED:
            std::cout << "\033[31m";
            break;
        case COLOR::WHITE:
            std::cout << "\033[37m";
            break;
        
        default:
            break;
        }
    #elif defined(__APPLE__) || defined(__MACH__)
        // nothing
    #endif
}