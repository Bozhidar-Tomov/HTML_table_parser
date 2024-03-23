#pragma once
#include "Cell.h"
#include "constants.h"

struct Row
{
private:
    Cell _cells[MAX_COLUMN_COUNT];
    int _size = 0;
    int _isValid = true;

public:
    Row() = default;
    Row(const Cell *, int);

    int getSize() const;
    const Cell *getCells() const;

    bool setRow(const Cell *, int);
    bool setCell(int, const char *);
    bool setCellFullSpan(int, const char *);

    bool addCell(const Cell &);
    bool addCell(const Cell &, int);

    bool removeCell();
    bool removeCell(int);

    bool isEmpty() const;
    bool isValid() const;
};
