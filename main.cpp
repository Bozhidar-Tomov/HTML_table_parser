#include "Table.h"
#include <iostream>

int main()
{
    Table tb;
    tb.loadFromFIle("test.html");
    tb.print(std::cout);
    tb.changeCellData(0, 0, "hello         world        !");
    tb.print(std::cout);
    int r = 2;
    r++;
}