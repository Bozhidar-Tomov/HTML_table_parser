#pragma once
#include "constants.h"
#include "Cell.h"
#include <ostream>

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
    bool setCell(int, const char *);

    void addCell(const Cell &);
    void addCell(const Cell &, int);

    void removeCell();
    void removeCell(int);

    bool isEmpty() const;
};
