#pragma once
#include <iostream>
#include <istream>
#include <fstream>
#include<sstream>
#include<windows.h>
#include <conio.h>
#include <vector>
//цвета
#define COLOR_BLACK 0           //черный
#define COLOR_BLUE 1            //синий
#define COLOR_GREEN 2           //зеленый
#define COLOR_CYAN 3            //голубой
#define COLOR_RED 4             //красный
#define COLOR_MAGENTA 5         //фиолетовый
#define COLOR_BROWN 6           //коричневый
#define COLOR_LIGHTGRAY 7       //светло-серый
#define COLOR_DARKGRAY 8        //темно-серый
#define COLOR_LIGHTBLUE 9       //светло-синий
#define COLOR_LIGHTGREEN 10     //светло-зеленый
#define COLOR_LIGHTRED 12       //светло-красный
#define COLOR_LIGHTMAGENTA 13   //светло-фиолетовый
#define COLOR_YELLOW 14         //желтый
#define COLOR_WHITE 15          //белый


//класс консоли
class TConsole
{
private:
    HANDLE InputHandle;                         //дескриптор буфера ввода
    HANDLE OutputHandle;                        //дескриптор буфера вывода
    CONSOLE_SCREEN_BUFFER_INFO ScreenBufInfo;   //информация об экранном буфере консоли
public:
    std::ostream& Out;              //поток вывода
    std::istream& In;               //поток ввода
    TConsole();                     //конструктор
    void ClrEol();                  //функция удаления символов от курсора до конца строки
    void ClrScr();                  //функция очистки экрана
    void Delay(WORD MS);            //функция задержки выполнения
    void DelLine();                 //функция удаления строки
    void GotoXY(int X, int Y);      //функция перемещения курсора
    void InsLine();                 //функция добавления строки
    bool KeyPressed();              //функция проверки нажатия клавиши
    WCHAR ReadKey();                //функция чтения символа с клавиатуры
    void TextBackground(BYTE Color);//функция задания цвета фона
    void TextColor(BYTE Color);     //функция задания цвета текста
    int WhereX();                   //функция получения текущей координаты X курсора
    int WhereY();                   //функция получения текущей координаты Y курсора
    void Window(BYTE X, BYTE Y);    //функция задания размеров окна
};

namespace global {
    extern TConsole console;
}

enum alignment
{
    LEFT = -1,
    CENTER = 0,
    RIGHT = 1
};

struct coordinates
{
    coordinates() = default;
    coordinates(unsigned int _x, unsigned int _y)
    {
        x = _x;
        y = _y;
    }

    void set_coordinates(unsigned int _x, unsigned int _y)
    {
        x = _x;
        y = _y;
    }

    unsigned int x = 0;
    unsigned int y = 0;
};

struct indents
{
    explicit indents(const char _symbol) : symbol(_symbol) {}
    indents(char _symbol, unsigned int _top, unsigned int _right, unsigned int _bottom, unsigned int _left)
    {
        symbol = _symbol;
        top = _top;
        bottom = _bottom;
        left = _left;
        right = _right;
    }
    void set_indents(unsigned int _top, unsigned int _right, unsigned int _bottom, unsigned int _left)
    {
        top = _top;
        bottom = _bottom;
        left = _left;
        right = _right;
    }
    void set_symbol(char _symbol)
    {
        symbol = _symbol;

    }
    unsigned int top = 0;
    unsigned int bottom = 0;
    unsigned int left = 0;
    unsigned int right = 0;
    char symbol;
};

void draw_horizontal_line(char _symbol, unsigned int start, unsigned int end, unsigned int y);
void draw_vertical_line(char _symbol, unsigned int start, unsigned int end, unsigned int x);

class blocksBorders
{
public:
    blocksBorders()
    {
        startsCoordinates.x = 0;
        startsCoordinates.y = 0;
        margin.set_indents(0, 0, 0, 0);
        padding.set_indents(0, 0, 0, 0);
        border.set_indents(1, 1, 1, 1);
        border_size = 1;
    }
    blocksBorders(int start_x, int start_y)
    {
        startsCoordinates.x = start_x;
        startsCoordinates.y = start_y;
        margin.set_indents(0, 0, 0, 0);
        padding.set_indents(0, 0, 0, 0);
        border.set_indents(1, 1, 1, 1);
        border_size = 1;
    }
    blocksBorders(coordinates _coordinates)
    {
        startsCoordinates.set_coordinates(_coordinates.x, _coordinates.y);
        margin.set_indents(0, 0, 0, 0);
        padding.set_indents(0, 0, 0, 0);
        border.set_indents(1, 1, 1, 1);
        border_size = 1;
    }
    ~blocksBorders() = default;

    void set_align(int _align)
    {
        //align = _align;
    }

    void set_border(unsigned int _size, char _symbol)
    {
        border.set_indents(_size, _size, _size, _size);
        border_size = _size;
        border.set_symbol(_symbol);
    }

    void set_margin(unsigned int _top, unsigned int _right, unsigned int _bottom, unsigned int _left)
    {
        margin.set_indents(_top, _right, _bottom, _left);
    }

    void set_padding(unsigned int _top, unsigned int _right, unsigned int _bottom, unsigned int _left)
    {
        padding.set_indents(_top, _right, _bottom, _left);
    }

    void set_mask(char _symbol)
    {
        mask = _symbol;
    }

    void set_width(unsigned int _width)
    {
        width = _width;
    }

    void set_height(unsigned int _height)
    {
        height = _height;
    }

    void render()
    {
        coordinates left_top = startsCoordinates;
        coordinates right_top(margin.left + border.left + padding.left + width + margin.right + border.right + padding.right, left_top.y);
        coordinates left_bottom(left_top.x, margin.top + border.top + padding.top + height + margin.bottom + border.bottom + padding.bottom);
        coordinates right_bottom(right_top.x, left_bottom.y);

        std::vector<indents> borders;
        borders.push_back(margin);
        borders.push_back(border);
        borders.push_back(padding);

        for (size_t border_type = 0; border_type < borders.size(); ++border_type)
        {
            int top_indent = borders.at(border_type).top;
            int left_indent = borders.at(border_type).left;
            int right_indent = borders.at(border_type).right;
            int bottom_indent = borders.at(border_type).bottom;
            char symbol = borders.at(border_type).symbol;

            for (size_t top_border = 0; top_border < top_indent; ++top_border)
            {
                draw_horizontal_line(symbol, left_top.x, right_top.x, left_top.y + top_border);
            }
            for (size_t left_border = 0; left_border < left_indent; ++left_border)
            {
                draw_vertical_line(symbol, left_top.y + top_indent, left_bottom.y - bottom_indent, left_top.x + left_border);
            }
            for (size_t right_border = 0; right_border < right_indent; ++right_border)
            {
                draw_vertical_line(symbol, right_top.y + top_indent, right_bottom.y - bottom_indent, right_top.x - right_border);
            }
            for (size_t bottom_border = 0; bottom_border < bottom_indent; ++bottom_border)
            {
                draw_horizontal_line(symbol, left_bottom.x, right_bottom.x, left_bottom.y - bottom_border);
            }
            
            left_top.set_coordinates(left_top.x + borders.at(border_type).left, left_top.y + borders.at(border_type).top);
            right_top.set_coordinates(right_top.x - borders.at(border_type).right, right_top.y + borders.at(border_type).top);
            left_bottom.set_coordinates(left_bottom.x + borders.at(border_type).left, left_bottom.y - borders.at(border_type).bottom);
            right_bottom.set_coordinates(right_bottom.x - borders.at(border_type).right, right_bottom.y - borders.at(border_type).bottom);

        }
        startsCoordinates.set_coordinates(left_top.x, left_top.y);
    }

    coordinates get_coordinates() const
    {
        return startsCoordinates;
    }

private:
    coordinates startsCoordinates;
    alignment align;

    indents margin = indents('-');
    indents padding = indents('+');
    indents border = indents('%');

    unsigned int border_size;

    char mask;

    unsigned int width;
    unsigned int height;

   // TConsole console;
};

class blocksLine
{
public:
    blocksLine()
    {
        startsCoordinates.set_coordinates(0, 0);
        text = "none";
        width = text.size();
    }
    blocksLine(const std::string& _text)
    {
        startsCoordinates.set_coordinates(0, 0);
        text = _text;
        width = text.size();
    }
    blocksLine(const std::string& _text, const coordinates _coordinates)
    {
        startsCoordinates.set_coordinates(_coordinates.x, _coordinates.y);
        text = _text;
        width = text.size();
    }
    ~blocksLine() = default;

    void set_text(const std::string& _new_text)
    {
        text = _new_text;
        width = text.size();
    }

    void set_align(int _align)
    {
        //align = _align;
    }

    void set_mask(char _symbol)
    {
        mask = _symbol;
    }

    void render()
    {
        switch (align)
        {
        case LEFT:
        {
            break;
        }
        case CENTER:
        {
            break;
        }
        case RIGHT:
        {
            break;
        }
        }
        global::console.GotoXY(startsCoordinates.x, startsCoordinates.y);
        std::cout << text;
    }

private:
    coordinates startsCoordinates;

    alignment align = alignment::CENTER;

    std::string text;

    char mask;

    unsigned int width;
    unsigned int height = 1;

   // TConsole console;
};






