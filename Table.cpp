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
    if (&out == &std::cout)
    {
        for (int i = 0; i < _rowsCount; ++i)
            printLine(i, out);

        out << std::endl;
        return;
    }
    else
    {
        out << "<table>\n";

        for (int i = 0; i < _rowsCount; ++i)
        {
            out << "\t<tr>\n";

            for (int j = 0; j < _rows[i].getSize(); ++j)
            {
                if (_rows[i].getCells()[j].isHead())
                {
                    out << "\t\t<th>";
                    _rows[i].getCells()[j].print(out);
                    out << "</th>\n";
                }

                else
                {
                    out << "\t\t<td>";
                    _rows[i].getCells()[j].print(out);
                    out << "</td>\n";
                }
            }
            out << "\t</tr>\n";
        }

        out << "</table>";
        return;
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

    for (int i = _rowsCount; i > idx; --i)
        _rows[i] = _rows[i - 1];

    _rows[idx] = row;
    ++_rowsCount;
    setWidth(row);
    return true;
}

bool Table::loadFromFile(const char *fileName)
{
    std::ifstream file(fileName);
    if (!file.is_open())
        return false;

    parser.parse(file, *this);

    file.close();
    return true;
}

bool Table::saveToFile(const char *fileName)
{
    std::ofstream file(fileName);

    if (!file)
    {
        std::cerr << "Unable to open file: " << fileName << std::endl;
        return false;
    }

    print(file);

    file.close();

    return true;
}

bool Table::changeCellData(int rowIdx, int colIdx, const char *data)
{
    if (rowIdx < 0 || rowIdx >= _rowsCount || colIdx >= _colsCount)
        return false;

    bool res = _rows[rowIdx].setCell(colIdx, data);

    if (res)
        setWidth(_rows[rowIdx]);
    return res;
}

void Table::setWidth(const Row &row)
{
    for (int i = 0; i < row.getSize(); ++i)
        if (row.getCells()[i].getSize() + (row.getCells()[i].isHead() ? 4 : 0) > _width)
            _width = row.getCells()[i].getSize() + (row.getCells()[i].isHead() ? 4 : 0);
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

        mySetW(diff + extraSpace + (_rows[rowIdx].getCells()[i].isHead() ? 0 : 2), DEFAULT_CHAR, out);
        _rows[rowIdx].getCells()[i].print(out);

        mySetW(diff + (_rows[rowIdx].getCells()[i].isHead() ? 0 : 2), DEFAULT_CHAR, out);
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
                _inTable = false;
                return;
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
            }
        }
        else if (_inData && dataPtr < BUFF_SIZE)
            data[dataPtr++] = _ch;
    }
}

#include <fstream>

void saveTableToHTML(const Table &table, const std::string &filename)
{
}
