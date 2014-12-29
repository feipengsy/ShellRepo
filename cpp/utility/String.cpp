#include "String.h"
#include <assert>
#include <cstring>
#include <algorithm>

shell::String::String()
             : _data(new char[1])
{
    _data[0] = '\0';
}

shell::String::String(const char* str) 
{
    assert(str);
    _data = new char[strlen(str) + 1];
    strcpy(_data, str);
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

shell::String& shell::String::operator=(String str)
{
    swap(str);
    return *this;
}

void shell::String::swap(String& other)
{
    std::swap(_data, other._data);
}
