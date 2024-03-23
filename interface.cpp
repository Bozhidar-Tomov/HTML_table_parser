#include "interface.h"
#include <iostream>
#include "constants.h"
#include "Table.h"
#include "Utils.h"
#include <sstream>
#include <limits>

enum class Option
{
    load,
    addRow,
    removeRow,
    edit,
    save,
    print,
    quit,
    unknown,
};

Option getOption(const char *command)
{
    if (myStrCmp(command, "load"))
        return Option::load;
    if (myStrCmp(command, "addRow"))
        return Option::addRow;
    if (myStrCmp(command, "removeRow"))
        return Option::removeRow;
    if (myStrCmp(command, "edit"))
        return Option::edit;
    if (myStrCmp(command, "save"))
        return Option::save;
    if (myStrCmp(command, "print"))
        return Option::print;
    if (myStrCmp(command, "quit"))
        return Option::quit;

    return Option::unknown;
}

struct Params
{
private:
    char _params[MAX_NUM_OF_PARAMS][MAX_TAG_ARGUMENT_SIZE] = {{'\0'}};
    int _count = 0;
    mutable int _pos = 0;

public:
    void readParameters(std::istream &in)
    {
        _count = 0;
        _pos = 0;

        char line[COMMAND_MAX_SIZE];
        in.getline(line, COMMAND_MAX_SIZE);
        std::stringstream ss(line);

        while (!ss.eof() && _count < MAX_NUM_OF_PARAMS)
            ss.getline(_params[_count++], MAX_TAG_ARGUMENT_SIZE, ' ');
    }

    const char *getNext() const
    {
        if (eof())
            return nullptr;
        return _params[_pos++];
    }

    const char *getCurr() const
    {
        if (eof())
            return nullptr;
        return _params[_pos];
    }

    bool eof() const
    {
        return _pos >= _count;
    }

    void clear()
    {
        _count = 0;
        _pos = 0;
    }

} params;

namespace Handlers
{
    void handleFileOperation(Option option, Table &table, Params &params)
    {
        const char *operation = {(option == Option::load) ? "loaded." : "saved."};
        bool success = (option == Option::load) ? table.loadFromFile(params.getCurr()) : table.saveToFile(params.getCurr());

        if (!success)
            std::cout << "File " << params.getCurr() << " cannot be " << operation << std::endl;
        else
            std::cout << "Table successfully " << operation << std::endl;
    }
};

//=========================================================

void run()
{
    std::cout << "Available commands:\n";

    std::cout << "1) load <file name>\n"
              << "2) addRow <rowNumber> <column> ... <column> (max " << MAX_COLUMN_COUNT << ")\n"
              << "3) removeRow <rowNumber>\n"
              << "4) edit <rowNumber> <colNumber> <newData>\n"
              << "5) save <file name>\n"
              << "6) print\n"
              << "7) quit" << std::endl;

    Table table;

    while (true)
    {
        std::cin.clear();
        fflush(stdin);

        std::cout << "->";
        char option[COMMAND_MAX_SIZE] = {'\0'};
        std::cin >> option;

        params.clear();

        if (std::cin.peek() == DEFAULT_CHAR)
        {
            std::cin.ignore();
            params.readParameters(std::cin);
        }

        switch (getOption(option))
        {
        case Option::load:
        case Option::save:
        {
            if (!params.getCurr())
            {
                std::cout << "Source file not specified!" << std::endl;
                break;
            }

            Handlers::handleFileOperation(getOption(option), table, params);
        }
        break;
        case Option::addRow:
        {
            Cell cells[MAX_COLUMN_COUNT];

            if (params.eof())
            {
                std::cout << "Parameters not specified!" << std::endl;
                break;
            }

            int rowIdx = strToInt(params.getCurr() - 1);
            if (rowIdx != -1)
                params.getNext();

            int idx = 0;

            while (!params.eof())
                cells[idx++] = Cell(params.getNext());

            if (rowIdx == -1)
                table.addRow(Row(cells, idx));
            else
                table.addRow(rowIdx, Row(cells, idx));
        }
        break;
        case Option::print:
            table.print(std::cout);
            break;

        case Option::removeRow:
        {
            if (params.eof())
            {
                if (!table.removeRow())
                    std::cout << "Cannot remove row. Table is empty" << std::endl;
                break;
            }

            int idx = strToInt(params.getCurr() - 1);
            if (idx == -1)
                std::cout << "Invalid index!\n";
            else if (!table.removeRow(idx))
                std::cout << "Cannot remove row at index " << idx << "." << std::endl;
        }
        break;
        case Option::edit:
        {
            int rowIdx = strToInt(params.getNext() - 1);
            int colIdx = strToInt(params.getNext() - 1);

            if (rowIdx == -1 || colIdx == -1)
            {
                std::cout << "Not an index!\n";
                break;
            }

            if (!table.changeCellData(rowIdx, colIdx, params.getNext()))
            {
                std::cout << "Error: unable to change data!\n";
                break;
            }
        }
        break;
        case Option::quit:
            return;
        default:
            std::cout << "Invalid command\n";
            break;
        }
    }
}