#pragma once
#include "constants.h"

class Cell
{
private:
    char _str[MAX_FIELD_SIZE + 1] = {'\0'};
    int _size = 0;
    bool isValid = true;

public:
    Cell();
    Cell(const char *);

    void setStr(const char *);
    const char *getStr() const;
    int getSize() const;
};
