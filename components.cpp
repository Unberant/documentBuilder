#include "components.h"

namespace global {
    TConsole console;
}

void draw_horizontal_line(char _symbol, unsigned int start, unsigned int end, unsigned int y)
{
    for (size_t i = start; i <= end; ++i)
    {
        global::console.GotoXY(i, y);
        std::cout << _symbol;
    }
}
void draw_vertical_line(char _symbol, unsigned int start, unsigned int end, unsigned int x)
{
    for (size_t i = start; i <= end; ++i)
    {
        global::console.GotoXY(x, i);
        std::cout << _symbol;
    }
}









//-----------------------------------------------------
//конструктор
TConsole::TConsole():In(std::cin), Out(std::cout)
{
    InputHandle = GetStdHandle(STD_INPUT_HANDLE);
    OutputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(OutputHandle, &ScreenBufInfo);
}

//-----------------------------------------------------
//функция удаления символов от курсора до конца строки
void TConsole::ClrEol()
{
    GetConsoleScreenBufferInfo(OutputHandle, &ScreenBufInfo);
    LPDWORD NOAW;
    ZeroMemory(&NOAW, sizeof(NOAW));
    DWORD Count, Size;
    Size = ScreenBufInfo.srWindow.Right - ScreenBufInfo.dwCursorPosition.X;
    FillConsoleOutputAttribute(OutputHandle, ScreenBufInfo.wAttributes, Size, ScreenBufInfo.dwCursorPosition, &Count);
    FillConsoleOutputCharacter(OutputHandle, ' ',
        ScreenBufInfo.dwSize.X - ScreenBufInfo.dwCursorPosition.X,
        ScreenBufInfo.dwCursorPosition, NOAW);
}

//-----------------------------------------------------
//функция очистки экрана
void TConsole::ClrScr()
{
    LPDWORD NOAW;
    ZeroMemory(&NOAW, sizeof(NOAW));
    ScreenBufInfo.dwCursorPosition.X = 0;
    ScreenBufInfo.dwCursorPosition.Y = 0;
    FillConsoleOutputCharacter(OutputHandle, ' ', ScreenBufInfo.dwSize.X * ScreenBufInfo.dwSize.Y,
        ScreenBufInfo.dwCursorPosition, NOAW);
    GotoXY(0, 0);
}

//-----------------------------------------------------
//функция задержки выполнения
void TConsole::Delay(WORD MS)
{
    SleepEx(MS, FALSE);
}

//-----------------------------------------------------
//функция удаления строки
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

//-----------------------------------------------------
//функция перемещения курсора
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

//-----------------------------------------------------
//функция добавления строки
void TConsole::InsLine()
{
    GetConsoleScreenBufferInfo(OutputHandle, &ScreenBufInfo);
    SMALL_RECT ScreenRect = ScreenBufInfo.srWindow;
    ScreenRect.Top = ScreenBufInfo.dwCursorPosition.Y - 1 + ScreenBufInfo.srWindow.Top;
    ScreenRect.Bottom = ScreenBufInfo.srWindow.Bottom - 1;
    CHAR_INFO CI;
    CI.Char.UnicodeChar = ' ';
    CI.Attributes = ScreenBufInfo.wAttributes;
    COORD Coord;
    Coord.X = ScreenRect.Left;
    Coord.Y = ScreenRect.Top + 1;
    DWORD dwSize = ScreenRect.Right - ScreenRect.Left + 1;
    ScrollConsoleScreenBuffer(OutputHandle, &ScreenRect, 0, Coord, &CI);
    Coord.Y--;
    DWORD dwCount;
    FillConsoleOutputAttribute(OutputHandle, ScreenBufInfo.wAttributes, dwSize, Coord, &dwCount);
}

//-----------------------------------------------------
//функция проверки нажатия клавиши
bool TConsole::KeyPressed()
{
    bool flag = false;
    DWORD NumberOfEvents;
    GetNumberOfConsoleInputEvents(InputHandle, &NumberOfEvents);
    if (NumberOfEvents > 0)
    {
        INPUT_RECORD IR;
        DWORD NumRead;
        if (PeekConsoleInput(InputHandle, &IR, 1, &NumRead))
        {
            if ((IR.EventType == KEY_EVENT) && (IR.Event.KeyEvent.bKeyDown)) flag = true;
            else ReadConsoleInput(InputHandle, &IR, 1, &NumRead);
        }
    }
    return flag;
}

//-----------------------------------------------------
//функция чтения символа с клавиатуры
WCHAR TConsole::ReadKey()
{
    DWORD NumRead;
    INPUT_RECORD IR;
    do
    {
        do
        {
        } while (!KeyPressed());
        ReadConsoleInput(InputHandle, &IR, 1, &NumRead);
    } while (IR.Event.KeyEvent.uChar.UnicodeChar == 0);
    return IR.Event.KeyEvent.uChar.UnicodeChar;
}

//-----------------------------------------------------
//функция задания цвета фона
void TConsole::TextBackground(BYTE Color)
{
    GetConsoleScreenBufferInfo(OutputHandle, &ScreenBufInfo);
    WORD TextAttr = ScreenBufInfo.wAttributes;
    TextAttr = (Color << 4) | (TextAttr & 0x0F);
    SetConsoleTextAttribute(OutputHandle, TextAttr);
}

//-----------------------------------------------------
//функция задания цвета текста
void TConsole::TextColor(BYTE Color)
{
    GetConsoleScreenBufferInfo(OutputHandle, &ScreenBufInfo);
    WORD TextAttr = ScreenBufInfo.wAttributes;
    TextAttr = (Color & 0x0F) | (TextAttr & 0xF0);
    SetConsoleTextAttribute(OutputHandle, TextAttr);
}

//-----------------------------------------------------
//функция получения текущей координаты X курсора
int TConsole::WhereX()
{
    GetConsoleScreenBufferInfo(OutputHandle, &ScreenBufInfo);
    return ScreenBufInfo.dwCursorPosition.X;
}

//-----------------------------------------------------
//функция получения текущей координаты Y курсора
int TConsole::WhereY()
{
    GetConsoleScreenBufferInfo(OutputHandle, &ScreenBufInfo);
    return ScreenBufInfo.dwCursorPosition.Y;
}

//-----------------------------------------------------
//функция задания размеров окна
void TConsole::Window(BYTE X, BYTE Y)
{
    SMALL_RECT ConsoleRect;
    ConsoleRect.Top = 0;
    ConsoleRect.Left = 0;
    ConsoleRect.Right = X - 1;
    ConsoleRect.Bottom = Y - 1;
    COORD BufSize;
    BufSize.X = X;
    BufSize.Y = Y;
    SetConsoleScreenBufferSize(OutputHandle, BufSize);
    SetConsoleWindowInfo(OutputHandle, TRUE, &ConsoleRect);
    GotoXY(0, 0);
}
