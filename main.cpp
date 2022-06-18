#include "documentBuilder.h"

int main()
{
   std::ifstream myFile("docs.txt");
   std::ifstream new_block("new_block.txt");
   
   document doc;
   
   doc.load(myFile);
   
   doc.update(10, new_block);
   
   doc.render();
   
   doc.remove(2); // << std::endl;
   std::cout << "Count of elements:" << doc.count() << std::endl;
   std::cout << "is document empty:" << doc.empty() << std::endl;

   //global::console.GotoXY(0, 100); // possible
   
   return 0;
}

