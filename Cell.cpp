#include "Cell.h"
#include "Utils.h"

Cell::Cell()
{
    _str[0] = '\0';
    _size = 0;
}

Cell::Cell(const char *str)
{
    setStr(str);
}

void Cell::setStr(const char *str)
{
    if (!_str)
    {
        _str[0] = '\0';
        _size = 0;
        return;
    }

    int size = myStrLen(str);

    if (size > MAX_FIELD_SIZE)
    {
        isValid = false;
        _str[0] = '\0';
        _size = 0;
        return;
    }

    myStrCpy(_str, str);
    _size = size;
    isValid = true;
}

const char *Cell::getStr() const
{
    return _str;
}

int Cell::getSize() const
{
    return _size;
}
