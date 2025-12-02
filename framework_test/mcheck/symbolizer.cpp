#include "symbolizer.hpp"
#include <cstdio>
#include <sstream>
#include <iomanip>
#include <cstdint>
#include <string>
#include <vector>
#include <dlfcn.h>

namespace symbolizer
{

    static std::string run_addr2line_for(const char *objpath, uintptr_t offset)
    {
        if (!objpath)
            return {};
        std::ostringstream cmd;
        // demangle (-C), function names (-f) and pretty printing (-p)
        cmd << "addr2line -e " << objpath << " -f -p 0x" << std::hex << offset;
        FILE *fp = popen(cmd.str().c_str(), "r");
        if (!fp)
            return {};
        char buf[2048];
        std::string out;
        if (fgets(buf, sizeof(buf), fp))
            out = buf;
        pclose(fp);
        if (!out.empty() && out.back() == '\n')
            out.pop_back();
        return out;
    }

    static std::string symbolize_addr(void *addr)
    {
        if (!addr)
            return std::string();
        Dl_info info;
        if (dladdr(addr, &info) && info.dli_fname)
        {
            uintptr_t base = (uintptr_t)info.dli_fbase;
            uintptr_t a = (uintptr_t)addr;
            uintptr_t offset = (a >= base) ? (a - base) : a;
            std::string res = run_addr2line_for(info.dli_fname, offset);
            if (!res.empty())
            {
                // prefix with object name for clarity
                std::ostringstream ss;
                ss << "[" << info.dli_fname << "] " << res;
                return ss.str();
            }
        }
        // fallback: try /proc/self/exe with absolute address
        return run_addr2line_for("/proc/self/exe", (uintptr_t)addr);
    }

    std::string symbolize(void *addr)
    {
        auto s = symbolize_addr(addr);
        if (!s.empty())
            return s;
        std::ostringstream ss;
        ss << "addr:0x" << std::hex << (uintptr_t)addr;
        return ss.str();
    }

    std::vector<std::string> symbolize_many(const std::vector<void *> &addrs, int skip)
    {
        std::vector<std::string> out;
        if (addrs.empty())
            return out;
        size_t s = skip < 0 ? 0 : (size_t)skip;
        for (size_t i = s; i < addrs.size(); ++i)
            out.push_back(symbolize_addr(addrs[i]));
        return out;
    }

} // namespace symbolizer
