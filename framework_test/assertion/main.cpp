#include <iostream>
#include "../log/message.hpp"
#include "../log/colors.hpp"
#include "assert.hpp"

int main()
{
    using namespace tester;
    using namespace tester::colors;

    // success example
    testing::AssertionResult ok = testing::AssertionSuccess();
    Message m1;
    m1.SetColor(GREEN) << "Quick check passed: " << 1 << " == " << 1;
    std::cout << m1 << "\n";

    // failure example with message composed via Message
    Message mm;
    mm << "Value mismatch: expected=" << 42 << " got=" << 41;
    testing::AssertionResult fail = testing::AssertionFailure(mm);

    // print assertion summary
    if (!fail.success())
    {
        Message out;
        out.SetColor(BG_BRIGHT_RED) << "ASSERTION FAILED: " << fail.message();
        // operator<< will append RESET
        std::cout << out << "\n";
    }
    else
    {
        Message out;
        out.SetColor(GREEN) << "All assertions passed";
        std::cout << out << "\n";
    }

    // show negation and appended messages
    testing::AssertionResult neg = !testing::AssertionSuccess();
    neg << " (negation demo)";
    Message nmsg;
    nmsg.SetColor(YELLOW) << "Negation result success=" << neg.success() << " msg=" << neg.message();
    std::cout << nmsg << "\n";

    return fail.success() ? 0 : 1;
}
