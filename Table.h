#pragma once
#include <iostream>
#include "constants.h"
#include "Row.h"

class Table
{
private:
    Row _rows[MAX_ROW_COUNT];
    int _rowsCount = 0;
    int _width = 0;
    int _colsCount = 0;
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

    bool addRow(int, const Row &);
    bool loadFromFIle(const char *);

    bool changeCellData(int, int, const char *);

    bool removeRow(int);

    void print(std::ostream &) const;

private:
    void addRow(const Row &);
    void setWidth(const Row &);
    void printLine(const int, std::ostream &) const;
};