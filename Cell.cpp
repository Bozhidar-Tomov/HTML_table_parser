#include "Cell.h"
#include "Utils.h"

Cell::Cell()
{
    _str[0] = '\0';
    _size = 0;
}

Cell::Cell(const char *str, bool isHead)
{
    setStr(str);
    _isHead = isHead;
}

void Cell::setStr(const char *str)
{
    char buff[BUFF_SIZE];
    myStrCpy(buff, str);

    if (!buff)
    {
        _str[0] = '\0';
        _size = 0;
        return;
    }

    replaceEntityReferences(buff);

    int size = myStrLen(buff);

    if (size > MAX_FIELD_SIZE)
    {
        _isValid = false;
        _str[0] = '\0';
        _size = 0;
        return;
    }

    myStrCpy(_str, buff);
    _size = size;
    _isValid = true;
}

void Cell::setIsHead(bool isHead)
{
    _isHead = isHead;
}

const char *Cell::getStr() const
{
    return _str;
}

int Cell::getSize() const
{
    return _size;
}

bool Cell::getIsHead() const
{
    return _isHead;
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
