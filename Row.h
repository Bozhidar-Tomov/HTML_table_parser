#pragma once
#include "constants.h"
#include "Cell.h"

struct Row
{
private:
    Cell _cells[MAX_COLUMN_COUNT];
    int _size;

public:
    Row();
    Row(const Cell *, int);

    int getSize() const;
    const Row &getRow() const;
    const Cell *getCells() const;

    void setRow(const Cell *, int);
    void setCell(const char *, const int);

    void addCell(const Cell &);
    void addCell(const Cell &, int);

    void removeCell();
    void removeCell(int);
};
