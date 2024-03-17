#pragma one
#include "constants.h"
#include "Row.h"

class Table
{
private:
    Row _atributes;
    Row _rows[MAX_ROW_COUNT];
    int _rowsCount = 0;
    int _width = 0;

public:
    Table();

    void addRow(const Row &);
    void addAtributes(const Row &);

    bool loadFromFIle(const char *);
    // bool saveToFile();
};