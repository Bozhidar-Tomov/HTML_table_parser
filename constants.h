#pragma once

const int MAX_COLUMN_COUNT = 30; //
const int MAX_ROW_COUNT = 300;   //
const int MAX_FIELD_SIZE = 50;   //

const char OPENING_TAG = '<';
const char CLOSING_TAG = '>';

const int CHAR_ENTITY_SIZE = 7;
const int MAX_TAG_ARGUMENT_SIZE = 50 * CHAR_ENTITY_SIZE;
const int MAX_TAG_SIZE = 5;
const int BUFF_SIZE = MAX_TAG_SIZE + MAX_TAG_ARGUMENT_SIZE + 2 + 1;

const int COMMAND_MAX_SIZE = 512;
const char VERTICAL_DELIM = '|';
const char HORIZONTAL_DELIM = '-';
const char INDENT_DELIM = ':';
const char DEFAULT_CHAR = ' ';
