#include "Row.h"
#include "Utils.h"
#include "constants.h"

Row::Row() : _size(0)
{
    _cells[0] = Cell();
}

Row::Row(const Cell *values, int size)
{
    setRow(values, size);
}

int Row::getSize() const
{
    return _size;
}

const Row &Row::getRow() const
{
    return *this;
}

const Cell *Row::getCells() const
{
    return _cells;
}

void Row::setRow(const Cell *cells, int size)
{
    if (cells == nullptr || size >= MAX_COLUMN_COUNT)
    {
        return;
    }

    _size = size;

    for (int i = 0; i < size; ++i)
    {
        _cells[i] = cells[i];
    }
}

bool Row::setCell(int idx, const char *str)
{
    if (idx < 0 || idx >= MAX_COLUMN_COUNT)
        return false;

    _size = idx + 1 > _size ? idx + 1 : _size;

    return _cells[idx].setStr(str);
}

// append
void Row::addCell(const Cell &cell)
{
    if (_size >= MAX_COLUMN_COUNT)
    {
        return;
    }

    _cells[_size++] = cell;
}

// at index
void Row::addCell(const Cell &cell, int idx)
{
    if (idx < 0 || idx >= _size || _size >= MAX_COLUMN_COUNT)
    {
        return;
    }

    for (int i = _size; i > idx; --i)
    {
        _cells[i] = _cells[i - 1];
    }

    _cells[idx] = cell;
    ++_size;
}

// at end
void Row::removeCell()
{
    if (_size > 0)
        --_size;
}

// at index
void Row::removeCell(int idx)
{
    if (idx < 0 || idx >= _size)
    {
        return;
    }

    for (int i = idx; i < _size - 1; ++i)
    {
        _cells[i] = _cells[i + 1];
    }

    --_size;
}

bool Row::isEmpty() const
{
    return _size == 0;
}
