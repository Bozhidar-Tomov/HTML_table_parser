#pragma once
#include <ostream>
#include "constants.h"

class Cell
{
private:
    char _str[MAX_FIELD_SIZE + 1] = {'\0'};
    int _size = 0;
    bool _isValid = true;
    bool _isHead = false;

public:
    Cell() = default;                 //
    Cell(const char *, bool = false); //

    bool setStr(const char *);  //
    const char *getStr() const; //

    void setIsHead(bool); //
    bool isHead() const;  //

    int getSize() const; //
    bool isValid() const;

    void print(std::ostream &) const; //

private:
    void replaceEntityReferences(char *); //
};
