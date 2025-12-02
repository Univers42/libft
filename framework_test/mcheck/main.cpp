#include "leaks.hpp"
#include <iostream>
#include <cstring>

__attribute__((weak))
int main()
{
    // RAII starts memcheck on construction and stops+reports on destruction
    memcheck::RAIICollector rc;

    // allocate two buffers
    char *buf1 = new char[64];
    std::strcpy(buf1, "This buffer is intentionally leaked (C++ test)\n");
    // register allocation with memcheck
    memcheck::onAlloc(buf1, 64);

    char *buf2 = new char[64];
    std::strcpy(buf2, "This buffer will be freed (C++ test)\n");
    memcheck::onAlloc(buf2, 64);

    // use them
    std::cout << buf1;
    std::cout << buf2;

    // free second buffer and unregister it
    memcheck::onFree(buf2);
    delete[] buf2;

    // intentionally do NOT free buf1 to simulate a leak

    // when main returns, rc.~RAIICollector() will stop and print the report
    return 0;
}
