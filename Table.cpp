#include "Table.h"
#include <fstream>
#include <sstream>
#include "Utils.h"

Table::Table() : _rowsCount(0), _width(0), _colsCount(0) //
{
    _rows[0] = Row();
}

bool Table::removeRow(int idx)
{
    if (idx < 0 || idx >= _rowsCount)
        return false;

    for (int i = idx; i < _rowsCount - 1; ++i)
    {
        _rows[i] = _rows[i + 1];
    }
    --_rowsCount;
    // TODO: update width and _colsCount
    return true;
}

void Table::print(std::ostream &out) const
{
    for (int i = 0; i < _rowsCount; ++i)
        printLine(i, out);

    out << std::endl;
}

void Table::addRow(const Row &row)
{
    if (_rowsCount >= MAX_ROW_COUNT)
        return;

    _rows[_rowsCount++] = row;
    setWidth(row);

    _colsCount = row.getSize() > _colsCount ? row.getSize() : _colsCount;
}

bool Table::addRow(int idx, const Row &row)
{
    if (_rowsCount >= MAX_ROW_COUNT || idx < 0 || idx >= _rowsCount)
        return false;

    _rows[idx] = row;
    ++_rowsCount;
    setWidth(row);
    return true;
}

bool Table::loadFromFIle(const char *fileName)
{
    std::ifstream file(fileName);
    if (!file.is_open())
        return false;

    parser.parse(file, *this);

    file.close();
    return true;
}

bool Table::changeCellData(int rowIdx, int colIdx, const char *data)
{
    if (rowIdx < 0 || rowIdx >= _rowsCount)
        return false;

    bool res = _rows[rowIdx].setCell(colIdx, data);

    if (res)
        setWidth(_rows[rowIdx]);
    return res;
}

void Table::setWidth(const Row &row)
{
    for (int i = 0; i < row.getSize(); ++i)
        if (row.getCells()[i].getSize() > _width)
            _width = row.getCells()[i].getSize();
}

void Table::printLine(int rowIdx, std::ostream &out) const
{
    for (int i = 0; i < _colsCount; ++i)
    {
        out << VERTICAL_DELIM;

        int valueSize = _rows[rowIdx].getCells()[i].getSize();

        int diff = (_width - valueSize + 2);

        bool extraSpace = false;

        if (diff % 2 != 0)
        {
            extraSpace = true;
            --diff;
        }

        diff /= 2;

        if (diff == 0)
            diff = 2;

        mySetW(diff + extraSpace, DEFAULT_CHAR, out);
        _rows[rowIdx].getCells()[i].print(out);

        mySetW(diff, DEFAULT_CHAR, out);
    }
    out << VERTICAL_DELIM << std::endl;
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
                _inTable = true;
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
                _inData = true;
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
                data[dataPtr++] = _ch;
            break;
        }
    }
}
