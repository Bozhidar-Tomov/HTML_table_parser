#pragma once
#include <iostream>
#include "constants.h"
#include "Row.h"

class Table
{
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
        HtmlTableParser() = default;
        void parse(std::istream &, Table &);
    } _parser;

public:
    Table() = default;

    bool addRow(const Row &);
    bool addRow(int, const Row &);
    bool loadFromFile(const char *);
    bool saveToFile(const char *);

    bool changeCellData(int, int, const char *);

    const Row *getRows() const;
    int getColsCount() const;
    int getRowsCount() const;

    bool removeRow();
    bool removeRow(int);

    void print(std::ostream &) const;

private:
    void setWidth(const Row &);
    void printLine(const int, std::ostream &) const;

    void updateDimensions();
};