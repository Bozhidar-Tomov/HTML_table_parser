#include "Table.h"
#include <fstream>
#include <sstream>
#include "Utils.h"

Table::Table()
{
    _rows[0] = Row();
    _rowsCount = 0;
    _width = 0;
}

void Table::addRow(const Row &row)
{
    if (_rowsCount >= MAX_ROW_COUNT)
    {
        return;
    }

    _rows[_rowsCount++] = row;
}

bool Table::loadFromFIle(const char *fileName)
{
    std::ifstream file(fileName);
    if (!file.is_open())
    {
        return false;
    }

    parser.parse(file, *this);

    file.close();
    return true;
}

void Table::setWidth(const Row &row)
{
    for (int i = 0; i < row.getSize(); ++i)
    {
        if (row.getCells()[i].getSize() > _width)
        {
            _width = row.getCells()[i].getSize();
        }
    }
}

Table::HtmlTableParser::HtmlTableParser() : _inTable(false), _inRow(false), _inData(false), _ch('\0') {}

void Table::HtmlTableParser::parse(std::istream &in, Table &table)
{
    char data[BUFF_SIZE + 1];
    int dataPtr = 0;
    Row row;

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
                return;
            }
            else if (_inTable && myStrCmp(tag, "tr"))
            {
                _inRow = true;
                row = Row();
            }
            else if (_inTable && myStrCmp(tag, "/tr"))
            {
                _inRow = false;
                table.addRow(row);
            }
            else if (_inRow && (myStrCmp(tag, "td") || myStrCmp(tag, "th")))
            {
                _inData = true;
            }
            else if (_inRow && (myStrCmp(tag, "/td") || myStrCmp(tag, "/th")))
            {
                _inData = false;
                data[dataPtr] = '\0';
                dataPtr = 0;

                Cell cell(data);
                if (myStrCmp(tag, "/th"))
                    cell.setIsHead(true);
                row.addCell(cell);
            }
            break;
        default:
            if (_inData && dataPtr < BUFF_SIZE)
            {
                data[dataPtr++] = _ch;
            }
            break;
        }
    }
}
