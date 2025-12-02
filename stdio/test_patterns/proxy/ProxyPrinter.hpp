#pragma once
#include "Printer.hpp"
#include <memory>
#include <iostream>

class ProxyPrinter : public Printer {
private:
    std::unique_ptr<Printer> realPrinter;
    bool enabled = true;

public:
    ProxyPrinter(std::unique_ptr<Printer> real)
        : realPrinter(std::move(real)) {}

    void setEnabled(bool e) { enabled = e; }

    void print(const std::string& msg) override {
        if (!enabled) {
            std::cout << "[PROXY] Blocked: " << msg << std::endl;
            return;
        }

        std::cout << "[PROXY] Forwarding..." << std::endl;
        realPrinter->print(msg);
    }
};
