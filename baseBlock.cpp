#include "components.h"

namespace variables
{
    TConsole console;
    unsigned console_x{ 0 };
    unsigned console_y{ 0 };
}
namespace functions
{
    void draw_horizontal_line(char _symbol, unsigned int start, unsigned int end, unsigned int y)
    {
        for (size_t i = start; i <= end; ++i)
        {
            variables::console.GotoXY(i, y);
            std::cout << _symbol;
        }
    }
    void draw_vertical_line(char _symbol, unsigned int start, unsigned int end, unsigned int x)
    {
        for (size_t i = start; i <= end; ++i)
        {
            variables::console.GotoXY(x, i);
            std::cout << _symbol;
            if (i >= variables::console_y)
                variables::console_y = i + 2;
        }
    }
    size_t minimum(size_t n1, size_t n2, size_t n3, size_t n4)
    {
        return min(min(n1, n2), min(n3, n4));
    }
    size_t maxInVector(const std::vector<size_t>& _vec)
    {
        if (_vec.size() == 0)
            return 0;
        return *std::max_element(_vec.begin(), _vec.end());
    }
    alignment stringToAlignment(std::string _string)
    {
        if (_string.find("left") != std::string::npos) {
            return alignment::LEFT;
        }
        else if (_string.find("right") != std::string::npos) {
            return alignment::RIGHT;
        }
        else if (_string.find("center") != std::string::npos) {
            return alignment::CENTER;
        }
    }
    indents stringToIndents(std::string _string)
    {
        std::vector<unsigned> indets(4);
        unsigned iter = 0;
        for (int i = 0; i < 4; ++i)
        {
            std::string value = "";
            for (; iter != _string.length() && _string.at(iter) != '\n' && _string.at(iter) != ' '; iter++)
                value += _string.at(iter);
            ++iter;
            indets.at(i) = static_cast<unsigned>(std::stoi(value));
        }

        return indents(indets[0], indets[1], indets[2], indets[3]);

    }
    std::string file_to_string(std::istream& _file)
    {
        if (_file)
        {
            std::ostringstream osStream;
            osStream << _file.rdbuf();
            return osStream.str();
        }
    }
}

TConsole::TConsole():In(std::cin), Out(std::cout)
{
    InputHandle = GetStdHandle(STD_INPUT_HANDLE);
    OutputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(OutputHandle, &ScreenBufInfo);
}

void TConsole::Delay(WORD MS)
{
    SleepEx(MS, FALSE);
}

void TConsole::DelLine()
{
    GetConsoleScreenBufferInfo(OutputHandle, &ScreenBufInfo);
    CHAR_INFO CI;
    ScreenBufInfo.srWindow.Top = WhereY();
    CI.Char.UnicodeChar = ' ';
    CI.Attributes = ScreenBufInfo.wAttributes;
    COORD Coord;
    Coord.X = ScreenBufInfo.srWindow.Left;
    Coord.Y = ScreenBufInfo.srWindow.Top - 1;
    DWORD Size, Count;
    Size = ScreenBufInfo.srWindow.Right - ScreenBufInfo.srWindow.Left + 1;
    ScrollConsoleScreenBuffer(OutputHandle, &ScreenBufInfo.srWindow, 0, Coord, &CI);
    FillConsoleOutputAttribute(OutputHandle, ScreenBufInfo.wAttributes, Size, Coord, &Count);
}

void TConsole::GotoXY(int X, int Y)
{
    ScreenBufInfo.dwCursorPosition.X = X;
    ScreenBufInfo.dwCursorPosition.Y = Y;
    if (!SetConsoleCursorPosition(OutputHandle, ScreenBufInfo.dwCursorPosition))
    {
        GotoXY(0, 0);
        DelLine();
    }
}

int TConsole::WhereY()
{
    GetConsoleScreenBufferInfo(OutputHandle, &ScreenBufInfo);
    return ScreenBufInfo.dwCursorPosition.Y;
}
