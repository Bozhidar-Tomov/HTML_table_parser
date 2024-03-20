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

struct Parameters
{
    char _params[MAX_NUM_OF_PARAMS][MAX_TAG_ARGUMENT_SIZE] = {{'\0'}};
    int _count = 0;

    void readParameters(std::istream &in)
    {
        _count = 0;

        char line[COMMAND_MAX_SIZE];
        in.getline(line, COMMAND_MAX_SIZE);
        std::stringstream ss(line);

        while (!ss.eof() && _count < MAX_NUM_OF_PARAMS)
            ss.getline(_params[_count++], MAX_TAG_ARGUMENT_SIZE, ' ');
        return;
    }

} params;

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
            if (!params._params[0])
            {
                std::cout << "Source file not specified!" << std::endl;
                break;
            }

            if (getOption(option) == Option::load)
            {
                if (!table.loadFromFile(params._params[0]))
                {
                    std::cout << "Cannot load file " << params._params[0] << std::endl;
                    break;
                }
            }
            else
            {
                if (!table.saveToFile(params._params[0]))
                {
                    std::cout << "Cannot save to file " << params._params[0] << std::endl;
                    break;
                }
            }

            std::cout << "Table loaded successfully." << std::endl;
        }
        break;
        case Option::addRow:
        {
            Cell cells[MAX_COLUMN_COUNT];

            int rowIdx = strToInt(params._params[0]);
            int idx = 0;

            for (int i = 0; i < params._count; ++i)
            {
                cells[idx++] = Cell(params._params[i]);
            }

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
            int idx = strToInt(params._params[0]);

            if (idx == -1)
                break;
            table.removeRow(idx);
        }
        break;
        case Option::edit:
        {
            int rowIdx = strToInt(params._params[0]);
            int colIdx = strToInt(params._params[1]);

            if (rowIdx == -1 || colIdx == -1)
            {
                std::cout << "Not an index!\n";
                break;
            }

            if (!table.changeCellData(rowIdx, colIdx, params._params[2]))
            {
                std::cout << "Error: unable to change data!\n";
                break;
            }
            std::cout << "Operation successful\n";
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