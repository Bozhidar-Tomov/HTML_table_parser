#include "Table.h"
#include <fstream>
#include <sstream>
#include "Utils.h"

enum class Tag
{
    table,
    th,
    tr,
    td,
    _table,
    _th,
    _tr,
    _td,
    unknown,
};

Tag getTag(const char *tag)
{
    if (myStrCmp(tag, "table"))
        return Tag::table;
    if (myStrCmp(tag, "/table"))
        return Tag::_table;
    if (myStrCmp(tag, "th"))
        return Tag::th;
    if (myStrCmp(tag, "/th"))
        return Tag::_th;
    if (myStrCmp(tag, "tr"))
        return Tag::tr;
    if (myStrCmp(tag, "/tr"))
        return Tag::_tr;
    if (myStrCmp(tag, "td"))
        return Tag::td;
    if (myStrCmp(tag, "/td"))
        return Tag::_td;

    return Tag::unknown;
}

bool Table::removeRow()
{
    if (_rowsCount < 1)
        return false;

    --_rowsCount;

    updateDimensions();
    return true;
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

    updateDimensions();
    return true;
}

void Table::print(std::ostream &out) const
{
    if (&out == &std::cout)
    {
        for (int i = 0; i < _rowsCount; ++i)
            printLine(i, out);

        out << std::endl;
    }
    else
    {
        out << "<table>\n";
        for (int i = 0; i < _rowsCount; ++i)
        {
            out << "\t<tr>\n";

            for (int j = 0; j < _rows[i].getSize(); ++j)
            {
                const char *o_tag = {(_rows[i].getCells()[j].isHead() ? "\t\t<th>" : "\t\t<td>")};
                const char *c_tag = {(_rows[i].getCells()[j].isHead() ? "</th>\n" : "</td>\n")};

                out << o_tag;
                _rows[i].getCells()[j].print(out);
                out << c_tag;
            }

            out << "\t</tr>\n";
        }
        out << "</table>";
    }
}

const Row *Table::getRows() const
{
    return _rows;
}

int Table::getColsCount() const
{
    return _colsCount;
}

int Table::getRowsCount() const
{
    return _colsCount;
}

bool Table::addRow(const Row &row)
{
    if (_rowsCount >= MAX_ROW_COUNT)
        return false;

    _rows[_rowsCount++] = row;

    setWidth(row);
    _colsCount = row.getSize() > _colsCount ? row.getSize() : _colsCount;
    return true;
}

bool Table::addRow(int idx, const Row &row)
{
    if (_rowsCount >= MAX_ROW_COUNT || idx < 0 || idx >= _rowsCount)
        return false;

    for (int i = _rowsCount; i > idx; --i)
        _rows[i] = _rows[i - 1];

    _rows[idx] = row;
    ++_rowsCount;

    setWidth(row);
    _colsCount = row.getSize() > _colsCount ? row.getSize() : _colsCount;
    return true;
}

bool Table::loadFromFile(const char *fileName)
{
    std::ifstream file(fileName, std::ios::in);
    if (!file.is_open())
        return false;

    _parser.parse(file, *this);

    file.close();
    return true;
}

bool Table::saveToFile(const char *fileName)
{
    std::ofstream file(fileName, std::ios::out);

    if (!file)
        return false;

    print(file);

    file.close();

    return true;
}

bool Table::changeCellData(int rowIdx, int colIdx, const char *data)
{
    if (rowIdx < 0 || rowIdx >= _rowsCount || colIdx < 0 || colIdx >= _colsCount)
        return false;

    bool res = _rows[rowIdx].setCell(colIdx, data);

    if (res)
        setWidth(_rows[rowIdx]);
    return res;
}

void Table::setWidth(const Row &row)
{
    for (int i = 0; i < row.getSize(); ++i)
    {
        int currWidth = row.getCells()[i].getSize() +
                        (row.getCells()[i].isHead() ? myStrLen(HEAD_OPEN) + myStrLen(HEAD_CLOSE) : 0);

        if (currWidth > _width)
            _width = currWidth;
    }
}

void Table::printLine(int rowIdx, std::ostream &out) const
{
    for (int i = 0; i < _colsCount; ++i)
    {
        const Cell &cell = _rows[rowIdx].getCells()[i];

        int diff = (_width - cell.getSize() + 2);
        bool extraSpace = false;

        if (diff % 2 != 0)
        {
            extraSpace = true;
            --diff;
        }

        diff /= 2;
        diff = !diff ? 2 : diff;

        out << VERTICAL_DELIM;
        mySetW(diff + extraSpace + (cell.isHead() ? 0 : myStrLen(HEAD_OPEN)), DEFAULT_CHAR, out);
        cell.print(out);
        mySetW(diff + (cell.isHead() ? 0 : myStrLen(HEAD_CLOSE)), DEFAULT_CHAR, out);
    }
    out << VERTICAL_DELIM << std::endl;
}

void Table::updateDimensions()
{
    _colsCount = 0;
    _width = 0;

    for (int i = 0; i < _rowsCount; ++i)
    {
        setWidth(_rows[i]);
        _colsCount = _colsCount < _rows[i].getSize() ? _rows[i].getSize() : _colsCount;
    }
}

void Table::HtmlTableParser::parse(std::istream &in, Table &table)
{
    int getIdx = in.tellg();
    char data[BUFF_SIZE + 1] = {'\0'};
    int dataPtr = 0;
    Row row;

    while (in.get(_ch))
    {
        if (_ch == OPENING_TAG)
        {
            char tag[MAX_TAG_SIZE + 1];
            in.get(tag, MAX_TAG_SIZE + 1, CLOSING_TAG);
            in.get(_ch); // skipping '>'

            switch (getTag(tag))
            {
            case Tag::table:
                _inTable = true;
                break;
            case Tag::_table:
            {
                _inTable = false;
                in.seekg(getIdx);
                return;
            }

            case Tag::tr:
                if (_inTable)
                {
                    _inRow = true;
                    row = Row();
                }
                break;
            case Tag::_tr:
                if (_inTable)
                {
                    _inRow = false;
                    table.addRow(row);
                }
                break;
            case Tag::td:
            case Tag::th:
                if (_inRow)
                    _inData = true;
                break;
            case Tag::_td:
            case Tag::_th:
                if (_inRow)
                {
                    _inData = false;
                    data[dataPtr] = '\0';
                    dataPtr = 0;

                    Cell cell(data);
                    if (getTag(tag) == Tag::_th)
                        cell.setIsHead(true);
                    row.addCell(cell);
                }
                break;
            default:
                if (_inData && dataPtr < BUFF_SIZE)
                    data[dataPtr++] = _ch;
            }
        }
        else if (_inData && dataPtr < BUFF_SIZE)
            data[dataPtr++] = _ch;
    }
}
