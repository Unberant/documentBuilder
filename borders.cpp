#include "components.h"

typedef baseBlock* ptrBase;
blocksBorders::blocksBorders() : baseBlock()
{
    margin.set_indents(0, 0, 0, 0);
    padding.set_indents(0, 0, 0, 0);
    border.set_indents(0, 0, 0, 0);
}
blocksBorders::blocksBorders(int start_x, int start_y) : baseBlock(start_x, start_y)
{
    margin.set_indents(0, 0, 0, 0);
    padding.set_indents(0, 0, 0, 0);
    border.set_indents(1, 1, 1, 1);
}
ptrBase blocksBorders::clone() const 
{
    return new blocksBorders(*this);
}
void blocksBorders::addConnection(ptrBase _component) 
{
    this->children.push_back(_component);
    _component->setParent(this);
}
void blocksBorders::deleteConnection()
{
    Iterator<baseBlock, std::list<ptrBase>>* it = create_iterator();
    for (it->first(); !it->isDone(); it->next())
        it->current()->deleteConnection();
    children.clear();
    if (getParent() != nullptr)
        getParent()->deleteElement(this);
    delete this;
    setParent(nullptr);
}
void blocksBorders::deleteElement(ptrBase component) 
{
    children.remove(component);
}
std::list<ptrBase> blocksBorders::getChildren() const
{
    return children;
}
void blocksBorders::addElementAfter(ptrBase component, ptrBase new_component)
{
    std::list<ptrBase>::iterator it = children.begin();
    size_t index = 0;
    for (auto var : children)
    {
        if (var == component)
        {
            advance(it, index + 1);
            children.insert(it, new_component);

        }
        ++index;
    }
    new_component->setParent(this);
}
Iterator<baseBlock, std::list<ptrBase>>* blocksBorders::create_iterator() const  
{
    return new Iterator<baseBlock, std::list<baseBlock*>>(this->children);
}
Iterator<baseBlock, std::list<ptrBase>>* blocksBorders::create_iterator(ptrBase _element) const  
{
    return new Iterator<baseBlock, std::list<baseBlock*>>(_element->getChildren());
}
void blocksBorders::set_border(unsigned int _size, char _symbol)  
{
    border.set_indents(_size, _size, _size, _size);
    border.set_symbol(_symbol);
}
void blocksBorders::set_chars(char _mask)  
{
    border.set_symbol(_mask);
}
void blocksBorders::set_margin(unsigned int _top, unsigned int _right, unsigned int _bottom, unsigned int _left)  
{
    margin.set_indents(_top, _right, _bottom, _left);
}
void blocksBorders::set_margin(indents _indent)  
{
    char s = margin.symbol;
    margin = _indent;
    margin.set_symbol(s);
}
void blocksBorders::set_padding(unsigned int _top, unsigned int _right, unsigned int _bottom, unsigned int _left)  
{
    padding.set_indents(_top, _right, _bottom, _left);
}
void blocksBorders::set_padding(indents _indent)  
{
    char s = padding.symbol;
    padding = _indent;
    padding.set_symbol(s);

}
unsigned blocksBorders::get_height() const  
{
    return margin.top + border.top + padding.top + height + margin.bottom + border.bottom + padding.bottom;
}
unsigned blocksBorders::get_width() const  
{
    return margin.left + border.left + padding.left + get_inside_width() + margin.right + border.right + padding.right;
}
coordinates blocksBorders::get_end_coords() const  
{
    return coordinates(startsCoordinates.x + margin.left + border.left + padding.left,
        startsCoordinates.y + margin.top + border.top + padding.top);
}
bool blocksBorders::isBlock() const  
{
    return true;
}
void blocksBorders::configureDimensions(const coordinates & _coordinates)  
{
    unsigned _width = 0;
    unsigned _height = 0;

    set_coordinates(_coordinates);
    coordinates _coords(_coordinates.x + margin.left + border.left + padding.left,
        _coordinates.y + margin.top + border.top + padding.top);

    if (getParent() != nullptr)
        set_align_ForIt(getParent()->get_alignment());
    Iterator<baseBlock, std::list<ptrBase>>* it = create_iterator();
    for (it->first(); !it->isDone(); it->next())
    {
        it->current()->configureDimensions(_coords);

        _coords = it->current()->get_coordinates();
        _coords.set_coordinates(_coords.x, _coords.y + it->current()->get_height());

        _width = max(it->current()->get_width(), _width);
        _height += it->current()->get_height();
    }
    set_width(max(get_inside_width(), _width));
    set_height(_height);
}
void blocksBorders::configureCoordinatesByAlign(unsigned parental_relocation)  
{
    unsigned parent_width = 0;
    if (getParent() == nullptr)
        parent_width = get_width();
    else
        parent_width = getParent()->get_inside_width();
    unsigned relocation = 0;
    switch (alignForIt)
    {
    case LEFT:
    {
        startsCoordinates.set_coordinates(parental_relocation + startsCoordinates.x, startsCoordinates.y);
        break;
    }
    case CENTER:
    {
        relocation = (parent_width - get_width()) / 2;
        startsCoordinates.set_coordinates(parental_relocation + startsCoordinates.x + relocation,
            startsCoordinates.y);
        break;
    }
    case RIGHT:
    {
        relocation = parent_width - get_width();
        startsCoordinates.set_coordinates(parental_relocation + startsCoordinates.x + relocation,
            startsCoordinates.y);
        break;
    }
    }
    Iterator<baseBlock, std::list<ptrBase>>* it = create_iterator();
    for (it->first(); !it->isDone(); it->next())
    {
        it->current()->configureCoordinatesByAlign(parental_relocation + relocation);
    }
}
void blocksBorders::configureTextByMask()  
{
    if (useMask == true)
        change_chars_in(this);
    Iterator<baseBlock, std::list<ptrBase>>* it = create_iterator();
    for (it->first(); !it->isDone(); it->next())
    {
        it->current()->configureTextByMask();
    }
}
void blocksBorders::change_chars_in(ptrBase _element)
{
    Iterator<baseBlock, std::list<ptrBase>>* it = create_iterator(_element);
    for (it->first(); !it->isDone(); it->next())
    {
        it->current()->set_chars(get_mask());
        change_chars_in(it->current());
    }
}
void blocksBorders::render()  
{
    coordinates left_top = startsCoordinates;
    coordinates right_top(
        left_top.x + margin.left + border.left + padding.left + width + margin.right + border.right + padding.right,
        left_top.y
                         );
    coordinates left_bottom(
        left_top.x,
        left_top.y + margin.top + border.top + padding.top + height - 1 + margin.bottom + border.bottom + padding.bottom
                            );
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
            functions::draw_horizontal_line(symbol, left_top.x, right_top.x, left_top.y + top_border);
        }
        for (size_t left_border = 0; left_border < left_indent; ++left_border)
        {
            functions::draw_vertical_line(symbol, left_top.y + top_indent, left_bottom.y - bottom_indent, left_top.x + left_border);
        }
        for (size_t right_border = 0; right_border < right_indent; ++right_border)
        {
            functions::draw_vertical_line(symbol, right_top.y + top_indent, right_bottom.y - bottom_indent, right_top.x - right_border);
        }
        for (size_t bottom_border = 0; bottom_border < bottom_indent; ++bottom_border)
        {
            functions::draw_horizontal_line(symbol, left_bottom.x, right_bottom.x, left_bottom.y - bottom_border);
        }

        left_top.set_coordinates(left_top.x + borders.at(border_type).left, left_top.y + borders.at(border_type).top);
        right_top.set_coordinates(right_top.x - borders.at(border_type).right, right_top.y + borders.at(border_type).top);
        left_bottom.set_coordinates(left_bottom.x + borders.at(border_type).left, left_bottom.y - borders.at(border_type).bottom);
        right_bottom.set_coordinates(right_bottom.x - borders.at(border_type).right, right_bottom.y - borders.at(border_type).bottom);

    }
}