#pragma once

constexpr int MAX_COLUMN_COUNT = 30; //
constexpr int MAX_ROW_COUNT = 300;   //
constexpr int MAX_FIELD_SIZE = 50;   //

constexpr char OPENING_TAG = '<'; //
constexpr char CLOSING_TAG = '>'; //

constexpr int CHAR_ENTITY_SIZE = 7; //
constexpr int MAX_TAG_ARGUMENT_SIZE = MAX_FIELD_SIZE * CHAR_ENTITY_SIZE;
constexpr int MAX_TAG_SIZE = 5;
constexpr int BUFF_SIZE = MAX_TAG_SIZE + MAX_TAG_ARGUMENT_SIZE + 2 + 1;

constexpr int COMMAND_MAX_SIZE = MAX_TAG_SIZE + (MAX_TAG_ARGUMENT_SIZE + 1) * MAX_COLUMN_COUNT;
constexpr int PARAMETERS_MAX_SIZE = 512;
constexpr char VERTICAL_DELIM = '|';
constexpr char HORIZONTAL_DELIM = '-';
constexpr char DEFAULT_CHAR = ' ';
constexpr int MAX_NUM_OF_PARAMS = 30;
