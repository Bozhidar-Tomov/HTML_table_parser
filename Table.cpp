#include "Table.h"
#include <fstream>
#include <sstream>
#include "Utils.h"
#include "Parser.h"

Table::Table()
{
    _atributes = Row();
    _rows[0] = Row();
    _rowsCount = 0;
    _width = 0;
}

void Table::addRow(const Row &row)
{
    if (_rowsCount >= MAX_ROW_COUNT)
    {
        return;
    }

    _rows[_rowsCount++] = row;
}

void Table::addAtributes(const Row &atributes)
{

    if (_atributes.getSize() != 0)
    {
        return;
    }

    _atributes = atributes;
}

bool Table::loadFromFIle(const char *fileName)
{
    std::ifstream file(fileName);
    if (!file.is_open())
    {
        return false;
    }

    Parser parser;
    parser.parse(file);

    file.close();
    return true;
}

int main()
{
    Table tb;
    tb.loadFromFIle("test.html");
}