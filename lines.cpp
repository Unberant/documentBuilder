#include "components.h"

typedef baseBlock* ptrBase;

blocksLine::blocksLine() : baseBlock()
{
    text = "";
    set_height(1);
    set_width(0);
}
blocksLine::blocksLine(int start_x, int start_y) : baseBlock(start_x, start_y)
{
    set_height(1);
    set_width(0);
}
ptrBase blocksLine::clone() const 
{
    return new blocksLine(*this);
}
void blocksLine::deleteConnection() 
{
    if (getParent() != nullptr)
        getParent()->deleteElement(this);
    delete this;
    setParent(nullptr);
}
void blocksLine::set_text(const std::string & _new_text) 
{
    text = _new_text;
    set_width(text.size());
}
void blocksLine::set_chars(char _mask) 
{
    set_text(std::string(get_width(), _mask));
}
bool blocksLine::isBlock() const 
{
    return false;
}
void blocksLine::configureDimensions(const coordinates & _coordinates) 
{
    set_coordinates(_coordinates);
    set_align_ForIt(getParent()->get_alignment());
}
void blocksLine::configureCoordinatesByAlign(unsigned parental_relocation) 
{
    unsigned parent_width = 0;
    if (getParent() == nullptr)
        parent_width = get_width();
    else
        parent_width = getParent()->get_inside_width();
    switch (alignForIt)
    {
    case LEFT:
    {
        startsCoordinates.set_coordinates(parental_relocation + startsCoordinates.x, startsCoordinates.y);
        break;
    }
    case CENTER:
    {
        startsCoordinates.set_coordinates(parental_relocation + startsCoordinates.x + ((parent_width - get_width()) / 2), startsCoordinates.y);
        break;
    }
    case RIGHT:
    {
        startsCoordinates.set_coordinates(parental_relocation + startsCoordinates.x + parent_width - get_width(), startsCoordinates.y);
        break;
    }
    }
}
void blocksLine::render() 
{
    variables::console.GotoXY(startsCoordinates.x, startsCoordinates.y);
    std::cout << text;
    if (startsCoordinates.y >= variables::console_y)
        variables::console_y = startsCoordinates.y + 1;
}