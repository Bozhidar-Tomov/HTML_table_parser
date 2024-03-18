#pragma once
#include "constants.h"

class Cell
{
private:
    char _str[MAX_FIELD_SIZE + 1] = {'\0'};
    int _size = 0;
    bool _isValid = true;
    bool _isHead = false;

public:
    Cell();
    Cell(const char *, bool = false);

    bool setStr(const char *);
    void setIsHead(bool);
    const char *getStr() const;
    int getSize() const;
    bool getIsHead() const;

private:
    void replaceEntityReferences(char *);
};
