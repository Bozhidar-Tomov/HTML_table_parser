#pragma once
#include "constants.h"
#include <iostream>

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
    bool isHead() const;
    const char *getStr() const;
    int getSize() const;

    void print(std::ostream &) const;

private:
    void replaceEntityReferences(char *);
};
