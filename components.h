#pragma once
#include <iostream>
#include <istream>
#include <fstream>
#include<sstream>
#include <algorithm>
#include<windows.h>
#include <conio.h>
#include <vector>
#include <list>

class TConsole
{
private:
    HANDLE InputHandle;                        
    HANDLE OutputHandle;                     
    CONSOLE_SCREEN_BUFFER_INFO ScreenBufInfo; 
public:
    std::ostream& Out;         
    std::istream& In;            
    TConsole();   
    void DelLine();
    void Delay(WORD MS);            
    void GotoXY(int X, int Y);   
    int WhereY();
};

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
    void set_coordinates(const coordinates& _coordinates)
    {
        x = _coordinates.x;
        y = _coordinates.y;
    }

    unsigned int x = 0;
    unsigned int y = 0;
};

struct indents
{
    explicit indents(const char _symbol) : symbol(_symbol) { }

    indents(unsigned int _top, unsigned int _right, unsigned int _bottom, unsigned int _left)
    {
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

namespace variables
{
    extern TConsole console;
    extern unsigned console_x;
    extern  unsigned console_y;
}

namespace functions
{
    void draw_horizontal_line(char _symbol, unsigned int start, unsigned int end, unsigned int y);
    void draw_vertical_line(char _symbol, unsigned int start, unsigned int end, unsigned int x);


    size_t minimum(size_t n1, size_t n2, size_t n3, size_t n4);
    size_t maxInVector(const std::vector<size_t>& _vec);

    alignment stringToAlignment(std::string _string);

    indents stringToIndents(std::string _string);
    std::string file_to_string(std::istream& _file);
}

template<typename T, typename U>
class Iterator {
public:
    typedef typename std::list<T*>::iterator filesIt;

    Iterator(const U& _children) : children(_children) { it = children.begin(); }

    void first() { it = children.begin(); }

    void next() { it++; }

    T* getPrevious() { return *(it - 1); }

    T* current() { return *it; }

    bool isDone() { return (it == children.end()); }
private:
    U children;
    filesIt it;
};

class baseBlock
{
    typedef baseBlock* ptrBase;
public:
    baseBlock() { startsCoordinates.set_coordinates(0, 0); }
    baseBlock(int start_x, int start_y) { startsCoordinates.set_coordinates(start_x, start_y); }

    virtual ~baseBlock() = default;

    virtual ptrBase clone() const = 0;

    virtual void addConnection(ptrBase component) { }
    virtual void deleteConnection() = 0;
    virtual void deleteElement(ptrBase component) { }

    virtual ptrBase getParent()	const final { return parent; }
    virtual void setParent(ptrBase _parent) final { this->parent = _parent; }

    virtual std::list<ptrBase> getChildren() const { return std::list<ptrBase>{}; }

    virtual void addElementAfter(ptrBase component, ptrBase new_component) { }

    virtual Iterator<baseBlock, std::list<ptrBase>>* create_iterator() const { return nullptr; }
    virtual Iterator<baseBlock, std::list<ptrBase>>* create_iterator(ptrBase _element) const { return nullptr; }

    virtual void set_align(alignment _align) final { align = _align; }
    virtual void set_align_ForIt(alignment _align) final { alignForIt = _align; }

    virtual void set_mask(char _symbol) final { mask = _symbol; }
    virtual void set_mask_mode(bool _mode) final { useMask = _mode; }

    virtual void set_border(unsigned int _size, char _symbol) { }

    virtual void set_margin(unsigned int _top, unsigned int _right, unsigned int _bottom, unsigned int _left) { }
    virtual void set_margin(indents _indent) { }

    virtual void set_padding(unsigned int _top, unsigned int _right, unsigned int _bottom, unsigned int _left) { }
    virtual void set_padding(indents _indent) { }

    virtual void set_text(const std::string& _new_text) { }

    virtual void set_width(unsigned int _width) final { width = _width; }

    virtual void set_height(unsigned int _height) final { height = _height; }

    virtual void set_coordinates(const coordinates& _coordinates) final { startsCoordinates = _coordinates; }

    virtual void set_chars(char _mask) = 0;

    virtual unsigned get_width() const { return width; }
    virtual unsigned get_inside_width() const final { return width; }

    virtual unsigned get_height() const { return height; }
    virtual alignment get_alignment() const final { return align; }

    virtual coordinates get_coordinates() const final { return startsCoordinates; }
    virtual coordinates get_end_coords() const { return coordinates(startsCoordinates.x, startsCoordinates.y + 1); }

    virtual char get_mask() const final { return mask; }
    virtual char get_mask_mode() const final { return useMask; }

    virtual bool isBlock() const = 0;

    virtual void configureDimensions(const coordinates& _coordinates) = 0;
    virtual void configureTextByMask() { }
    virtual void configureCoordinatesByAlign(unsigned parental_relocation) = 0;

    virtual void render() = 0;
protected:
    coordinates startsCoordinates;

    char mask{ ' ' };
    bool useMask{ false };

    alignment align{ LEFT };
    alignment alignForIt{ LEFT };

    unsigned int width{ 0 };
    unsigned int height{ 0 };

    ptrBase parent;
};

class blocksBorders : public baseBlock
{
    typedef baseBlock* ptrBase;
public:
    blocksBorders();
    blocksBorders(int start_x, int start_y);

    virtual ~blocksBorders() = default;

    ptrBase clone() const override;

    void addConnection(ptrBase _component) override;
    void deleteConnection() override;

    void deleteElement(ptrBase component) override;

    std::list<ptrBase> getChildren() const override;

    void addElementAfter(ptrBase component, ptrBase new_component) override;

    Iterator<baseBlock, std::list<ptrBase>>* create_iterator() const override;
    Iterator<baseBlock, std::list<ptrBase>>* create_iterator(ptrBase _element) const override;

    void set_border(unsigned int _size, char _symbol) override;
    void set_chars(char _mask) override;

    void set_margin(unsigned int _top, unsigned int _right, unsigned int _bottom, unsigned int _left) override;
    void set_margin(indents _indent) override;

    void set_padding(unsigned int _top, unsigned int _right, unsigned int _bottom, unsigned int _left) override;
    void set_padding(indents _indent) override;

    unsigned get_height() const override;
    unsigned get_width() const override;

    coordinates get_end_coords() const override;

    bool isBlock() const override;
    void configureDimensions(const coordinates& _coordinates) override;
    void configureCoordinatesByAlign(unsigned parental_relocation) override;
    void configureTextByMask() override;

    void change_chars_in(ptrBase _element);
    void render() override;
private:
    indents margin { ' ' };
    indents padding{ ' ' };
    indents border{ ' ' };

    std::list<ptrBase> children;
};

class blocksLine : public baseBlock
{
    typedef baseBlock* ptrBase;
public:
    blocksLine();
    blocksLine(int start_x, int start_y);

    virtual ~blocksLine() = default;

    ptrBase clone() const override;

    void deleteConnection() override;

    void set_text(const std::string& _new_text) override;

    void set_chars(char _mask) override;

    bool isBlock() const override;

    void configureDimensions(const coordinates& _coordinates) override;

    void configureCoordinatesByAlign(unsigned parental_relocation) override;

    void render() override;
private:
    std::string text;
};