#pragma once
#include <ostream>

void myStrCpy(char *, const char *);
int myStrLen(const char *);
void resize(char *&, const int);
bool myStrCmp(const char *, const char *);
int strToInt(const char *);
void printStr(const char *, std::ostream &);
void mySetW(const int, const char, std::ostream &);
void removeExtraSpaces(char *);
void myStrcat(char *, const char *);