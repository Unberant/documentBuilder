#include "documentReBuilder.h"

int main()
{
    std::ifstream myFile("docs.txt");

    document doc(myFile);

    doc.print();


    //blocksBorders bord (3, 1);
    //
    //bord.set_border(3, '%');
    //bord.set_margin(3, 2, 1, 4);
    //bord.set_padding(5, 0, 2, 1);
    //bord.set_width(12);
    //bord.set_height(6);

    //bord.render();


    //blocksLine line("12345", bord.get_coordinates());
    //line.render();



    //TConsole ConsolE;
    //global::console.GotoXY(0, 40);
    return 0;
}

