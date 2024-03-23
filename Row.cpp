#include "Row.h"
#include "Utils.h"
#include "constants.h"

Row::Row(const Cell *values, int size)
{
    _isValid = setRow(values, size);
}

int Row::getSize() const
{
    return _size;
}

const Cell *Row::getCells() const
{
    return _cells;
}

bool Row::setRow(const Cell *cells, int size)
{
    if (cells == nullptr || size >= MAX_COLUMN_COUNT)
        return false;

    _size = size;

    for (int i = 0; i < size; ++i)
    {
        if (!cells[i].isValid())
            return false;

        _cells[i] = cells[i];
    }

    return true;
}

bool Row::setCell(int idx, const char *str)
{
    if (idx < 0 || idx >= _size)
        return false;

    ++_size;

    return _cells[idx].setStr(str);
}

bool Row::setCellFullSpan(int idx, const char *str)
{
    if (idx < 0 || idx >= MAX_COLUMN_COUNT)
        return false;

    _size = idx + 1 > _size ? idx + 1 : _size;

    return _cells[idx].setStr(str);
}

// append
bool Row::addCell(const Cell &cell)
{
    if (_size >= MAX_COLUMN_COUNT)
        return false;

    _cells[_size++] = cell;
    return true;
}

// at index
bool Row::addCell(const Cell &cell, int idx)
{
    if (idx < 0 || idx >= _size || _size >= MAX_COLUMN_COUNT)
        return false;

    for (int i = _size; i > idx; --i)
        _cells[i] = _cells[i - 1];

    _cells[idx] = cell;
    ++_size;

    return true;
}

// at end
bool Row::removeCell()
{
    if (_size < 1)
        return false;

    --_size;
    return true;
}

// at index
bool Row::removeCell(int idx)
{
    if (idx < 0 || idx >= _size)
        return false;

    for (int i = idx; i < _size - 1; ++i)
        _cells[i] = _cells[i + 1];

    --_size;
    return true;
}

bool Row::isEmpty() const
{
    return _size == 0;
}

bool Row::isValid() const
{
    return _isValid;
}
