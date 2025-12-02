#pragma once
#include <string>

class Printer
{
public:
    virtual ~Printer() = default;
    virtual void print(const std::string &msg) = 0;
};
