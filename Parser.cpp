#include "Parser.h"
#include "Utils.h"
#include "constants.h"
#include "Cell.h"

Parser::Parser() : _inTable(false), _inRow(false), _inData(false), _inHeader(false), _ch('\0') {}

void Parser::parse(std::istream &in)
{
    char data[MAX_FIELD_SIZE];
    int dataPtr = 0;

    while (in.get(_ch))
    {
        switch (_ch)
        {
        case '<':
            char tag[MAX_TAG_SIZE + 1];

            in.get(tag, MAX_TAG_SIZE + 1, '>');
            in.get(_ch); // consume '>'
            if (myStrCmp(tag, "table"))
            {
                _inTable = true;
            }

            else if (myStrCmp(tag, "/table"))
            {
                _inTable = false;
            }
            else if (_inTable && myStrCmp(tag, "tr"))
            {
                parseRow(in);
            }
            else if (_inTable && myStrCmp(tag, "/tr"))
            {
                _inRow = false;
                // Row();
                // std::cout << "\n";
            }
            else if (_inRow && myStrCmp(tag, "td"))
            {
                _inData = true;
                parseCell(in);
            }
            else if (_inRow && myStrCmp(tag, "/td"))
            {
                _inData = false;
                Cell(data);

                // create cell;
            }
            break;
        default:
            if (_inData)
            {
                data[dataPtr++] = _ch;
            }
            break;
        }
    }
}

void Parser::parseRow(std::istream &)
{
}

void Parser::parseCell(std::istream &)
{
}
