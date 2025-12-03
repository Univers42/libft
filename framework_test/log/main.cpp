#include <iostream>
#include <iomanip>
#include <thread>
#include "message.hpp"
#include "colors.hpp"

// forward declaration; logger.cpp should be compiled/linked separately
void run_logger_demo();

int main()
{
    using namespace tester;
    using namespace tester::colors;

    // header
    std::cout << BOLD << "=== Tester Message + Logger Demo ===" << RESET << "\n\n";

    // show standard palette
    std::cout << UNDERLINE << "Standard colors:" << RESET << "\n";
    std::cout << BLACK << " BLACK " << RESET << " "
              << RED << " RED " << RESET << " "
              << GREEN << " GREEN " << RESET << " "
              << YELLOW << " YELLOW " << RESET << " "
              << BLUE << " BLUE " << RESET << " "
              << MAGENTA << " MAGENTA " << RESET << " "
              << CYAN << " CYAN " << RESET << " "
              << WHITE << " WHITE " << RESET << "\n\n";

    std::cout << UNDERLINE << "Bright colors:" << RESET << "\n";
    std::cout << BRIGHT_BLACK << " BRIGHT_BLACK " << RESET << " "
              << BRIGHT_RED << " BRIGHT_RED " << RESET << " "
              << BRIGHT_GREEN << " BRIGHT_GREEN " << RESET << " "
              << BRIGHT_YELLOW << " BRIGHT_YELLOW " << RESET << " "
              << BRIGHT_BLUE << " BRIGHT_BLUE " << RESET << " "
              << BRIGHT_MAGENTA << " BRIGHT_MAGENTA " << RESET << " "
              << BRIGHT_CYAN << " BRIGHT_CYAN " << RESET << " "
              << BRIGHT_WHITE << " BRIGHT_WHITE " << RESET << "\n\n";

    // 256-color gradient (first 36 colors for compact demo)
    std::cout << UNDERLINE << "256-color gradient (sample):" << RESET << "\n";
    for (int i = 16; i < 52; ++i)
    {
        std::cout << fg256(i) << std::setw(4) << i << RESET;
        if ((i - 16 + 1) % 12 == 0)
            std::cout << "\n";
    }
    std::cout << "\n\n";

    // truecolor examples
    std::cout << UNDERLINE << "Truecolor samples:" << RESET << "\n";
    std::cout << fg_rgb(255, 120, 0) << " orange-fg " << RESET << " ";
    std::cout << bg_rgb(10, 10, 60) << fg_rgb(200, 220, 255) << " bg-dark-blue-with-light-text " << RESET << "\n\n";

    // Message usage examples (colored whole-message and inline fragments)
    {
        Message info;
        info.SetColor(GREEN) << "Service started" << " port=" << 8080 << " env=prod";
        std::cout << info << "\n";
    }

    {
        Message warn;
        warn.SetColor(YELLOW) << "Low disk space" << " remaining=" << 512 << "MB";
        std::cout << warn << "\n";
    }

    {
        Message err;
        // inline fragment color + whole-message background demonstration
        err << "Failed to open " << "/etc/passwd" << " - ";
        err << BRIGHT_RED << "permission denied" << RESET;
        std::cout << err << "\n";
    }

    // diff-like output (green/red lines)
    {
        Message diff;
        diff << GREEN << "+ expected: foobar()" << RESET << "\n";
        diff << RED << "- actual:   foobaz()" << RESET << "\n";
        std::cout << diff << "\n";
    }

    // memory report (compact)
    {
        Message mem;
        mem << "mem_report: total=" << 8 * 1024 << "KB used=" << 3 * 1024 << "KB free=" << 5 * 1024 << "KB";
        std::cout << DIM << mem << RESET << "\n\n";
    }

    // assertion (use background color to emphasize)
    {
        Message assertion;
        assertion.SetColor(BG_BRIGHT_RED);
        assertion << "ASSERTION FAILED: expected=" << 42 << " got=" << 41;
        // printing via operator<< will append RESET
        std::cout << assertion << "\n\n";
    }

    // Small animated spinner to show asynchronous logger demo starting (visual flourish)
    {
        std::cout << ITALIC << "Starting logger pipeline demo";
        for (int i = 0; i < 6; ++i)
        {
            std::cout << "." << std::flush;
            std::this_thread::sleep_for(std::chrono::milliseconds(120));
        }
        std::cout << RESET << "\n\n";
    }

    // Run the full logger pipeline demo (formatting, color, threading, file sink)
    run_logger_demo();

    std::cout << "\n"
              << BOLD << "=== Demo complete ===" << RESET << "\n";
    std::cout << "Check app.log for persisted entries and observe colored output above.\n";

    return 0;
}
