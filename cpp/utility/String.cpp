#include "String.h"
#include <cstring>

shell::String::String()
             : _data(new char[1])
{
    _data[0] = '\0';
}

shell::String::String(const char* str) 
             : _data(0)
{
    _init(str);
}

shell::String::String(const String& str)
{
    _data = new char[strlen(str._data) + 1];
    strcpy(_data, str._data);
}

shell::String::~String()
{
    delete [] _data;
}

shell::String& shell::String::operator=(const String& str)
{
    if (&this != str) {
        delete [] _data;
        _init(str);
    } 
    return *this;
}

void shell::String::_init(const String& str)
{
    _data = new char[strlen(str) + 1];
    strcpy(_data, s);
}
