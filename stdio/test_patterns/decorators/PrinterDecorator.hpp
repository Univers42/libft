#pragma once
#include <string>
#include <memory>
#include <utility>
#include <type_traits>

class Printer
{
public:
    virtual ~Printer() = default;
    virtual void print(const std::string &msg) = 0;
};

class PrinterDecorator : public Printer
{
protected:
    std::unique_ptr<Printer> wrapped;

public:
    // Accept unique_ptr<T> for any T derived from Printer so nested make_unique(...) chains work.
    template <typename T, typename = std::enable_if_t<std::is_base_of<Printer, T>::value>>
    explicit PrinterDecorator(std::unique_ptr<T> printer)
        : wrapped(std::move(printer)) {}
    ~PrinterDecorator() override = default;
};
