#include <iostream>

void myStrCpy(char *destination, const char *source)
{
    if (!destination || !source)
    {
        return;
    }

    int i = 0;

    while (source[i] != '\0')
    {
        destination[i] = source[i];
        ++i;
    }
    destination[i] = '\0';
}

int myStrLen(const char *str)
{
    const char *s = str;

    while (*s)
    {
        ++s;
    }

    return (s - str);
}

/*
Here we allocate memory - titleCapacity*sizeof(char) bytes in heap and 4 bytes in the stack for the pointer.
Then we delete the stuff in the heap that title points to.
Then we make title to point to the content that new title points at the heap. now both pointers point to the same memory in heap.
At the end of the scope new title dies and title points to the new arr.
We pass the pointer as reference and not as copy so that after the function call title points to the new memory
*/
void resize(char *&title, const int titleCapacity)
{
    char *newTitle = new char[titleCapacity];

    delete[] title;

    title = newTitle;
}

bool myStrCmp(const char *s1, const char *s2)
{
    if (!s1 || !s2)
    {
        return false;
    }
    while (*s1 && *s1 == *s2)
    {
        ++s1;
        ++s2;
    }
    return *s1 - *s2 == 0;
}

// only positive numbers!
int strToInt(const char *str)
{
    int x = 0;

    while (*str >= '0' && *str <= '9')
    {
        x = 10 * x + *str - '0';
        ++str;
    }

    return *str == '\0' ? x : -1;
}

void printStr(const char *str, std::ostream &oStream)
{
    if (!str)
    {
        return;
    }

    while (*str)
    {
        oStream << *str++;
    }
}

void mySetW(const int w, char ch, std::ostream &oStream)
{
    for (int i = 0; i < w; ++i)
    {
        oStream << ch;
    }
}

// Some external resources are used in the implementation of this function!
void removeExtraSpaces(char *str)
{
    int i = 0;
    int j = 0;

    while (str[i])
    {
        if (str[i] != ' ')
        {
            str[j++] = str[i];
        }

        else if (j > 0 && str[j - 1] != ' ')
        {
            str[j++] = ' ';
        }
        ++i;
    }

    if (j > 0 && str[j - 1] == ' ')
    {
        --j;
    }

    str[j] = '\0';
}

void myStrcat(char *first, const char *second)
{
    if (!first || !second)
        return;

    size_t firstLen = myStrLen(first);
    first += firstLen;
    myStrCpy(first, second);
}