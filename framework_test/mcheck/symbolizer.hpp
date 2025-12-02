#pragma once
#include <string>
#include <vector>

namespace symbolizer
{
    // symbolize a single address (returns one-line "func at file:line" or raw hex if unavailable)
    std::string symbolize(void *addr);

    // symbolize multiple addresses (skip 'skip' frames)
    std::vector<std::string> symbolize_many(const std::vector<void *> &addrs, int skip = 0);
}
