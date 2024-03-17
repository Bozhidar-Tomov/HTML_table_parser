#pragma once
#include <istream>

class Parser
{
private:
    bool _inTable = false;
    bool _inRow = false;
    bool _inData = false;
    bool _inHeader = false;
    char _ch = '\0';

public:
    Parser();
    void parse(std::istream &);
    void parseRow(std::istream &);
    void parseCell(std::istream &);
    void parseTag(std::istream &in);
    void parseData(std::istream &in);
};