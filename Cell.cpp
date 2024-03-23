#include <iostream>
#include "Cell.h"
#include "Utils.h"

Cell::Cell(const char *str, bool isHead)
{
    _isValid = setStr(str);
    _isHead = isHead;
}

bool Cell::setStr(const char *str)
{
    if (!str)
    {
        _str[0] = '\0';
        _size = 0;
        return false;
    }

    char buff[BUFF_SIZE];
    myStrCpy(buff, str);

    replaceEntityReferences(buff);

    int size = myStrLen(buff);

    if (size > MAX_FIELD_SIZE)
    {
        _str[0] = '\0';
        _size = 0;
        return false;
    }

    myStrCpy(_str, buff);
    _size = size;
    return true;
}

const char *Cell::getStr() const
{
    return _str;
}

void Cell::setIsHead(bool isHead)
{
    _isHead = isHead;
}

bool Cell::isHead() const
{
    return _isHead;
}

int Cell::getSize() const
{
    return _size;
}

bool Cell::isValid() const
{
    return _isValid;
}

void Cell::print(std::ostream &out) const
{
    if (_isHead && &out == &std::cout)
        std::cout << HEAD_OPEN;

    printStr(_str, out);

    if (_isHead && &out == &std::cout)
        std::cout << HEAD_CLOSE;
}

void Cell::replaceEntityReferences(char *str)
{
    int i = 0, j = 0;
    while (str[i] != '\0')
    {
        if (str[i] == '&' && str[i + 1] == '#')
        {
            int val = 0;
            int k = i + 2;
            while (str[k] != ';' && str[k] != '\0')
            {
                val = val * 10 + (str[k] - '0');
                ++k;
            }
            if (str[k] == ';' && val >= 0 && val <= 127)
            {
                str[j++] = (char)(val);
                i = k + 1;
            }
            else
                str[j++] = str[i++];
        }
        else
            str[j++] = str[i++];
    }
    str[j] = '\0';
}
