#pragma once
#include <istream>
#include "constants.h"
#include "Row.h"

class Table
{
private:
    Row _rows[MAX_ROW_COUNT];
    int _rowsCount = 0;
    int _width = 0;
    struct HtmlTableParser
    {
    private:
        bool _inTable = false;
        bool _inRow = false;
        bool _inData = false;
        char _ch = '\0';

    public:
        HtmlTableParser();
        void parse(std::istream &, Table &);
    } parser;

public:
    Table();

    void addRow(const Row &);
    bool loadFromFIle(const char *);

private:
    void setWidth(const Row &);
};