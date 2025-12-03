/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 13:41:33 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/03 14:19:25 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal/string.hpp"

#include <locale>
#include <codecvt>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cctype>

namespace testing
{
    namespace internal
    {

        const char *String::CloneCString(const char *c_str)
        {
            if (!c_str)
                return nullptr;
            size_t len = std::strlen(c_str);
            char *copy = new char[len + 1];
            std::memcpy(copy, c_str, len + 1);
            return copy;
        }

        bool String::CStringEquals(const char *lhs, const char *rhs)
        {
            if (lhs == rhs)
                return true; // covers both nullptr
            if (!lhs || !rhs)
                return false;
            return std::strcmp(lhs, rhs) == 0;
        }

        std::string String::ShowWideCString(const wchar_t *wide_c_str)
        {
            if (!wide_c_str)
                return std::string("(null)");
#if defined(__cpp_lib_codecvt) || defined(_MSC_VER) || 1
            try
            {
                std::wstring ws(wide_c_str);
                // use codecvt to convert wchar_t -> UTF-8 (portable enough)
                std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> conv;
                return conv.to_bytes(ws);
            }
            catch (...)
            {
                return std::string("(failed to convert from wide string)");
            }
#else
            // Fallback: narrow each wchar via wctomb (locale dependent)
            std::string out;
            std::mbstate_t state = std::mbstate_t();
            const wchar_t *src = wide_c_str;
            size_t len = std::wcslen(wide_c_str);
            std::vector<char> buf(len * MB_CUR_MAX + 1);
            const wchar_t *p = src;
            char *q = buf.data();
            size_t ret = std::wcsrtombs(q, &p, buf.size(), &state);
            if (ret == static_cast<size_t>(-1))
                return "(failed to convert from wide string)";
            return std::string(q, ret);
#endif
        }

        bool String::WideCStringEquals(const wchar_t *lhs, const wchar_t *rhs)
        {
            if (lhs == rhs)
                return true;
            if (!lhs || !rhs)
                return false;
            return std::wcscmp(lhs, rhs) == 0;
        }

        bool String::CaseInsensitiveCStringEquals(const char *lhs, const char *rhs)
        {
            if (lhs == rhs)
                return true;
            if (!lhs || !rhs)
                return false;
            while (*lhs && *rhs)
            {
                unsigned char a = static_cast<unsigned char>(*lhs);
                unsigned char b = static_cast<unsigned char>(*rhs);
                if (std::tolower(a) != std::tolower(b))
                    return false;
                ++lhs;
                ++rhs;
            }
            return *lhs == *rhs;
        }

        bool String::CaseInsensitiveWideCStringEquals(const wchar_t *lhs, const wchar_t *rhs)
        {
            if (lhs == rhs)
                return true;
            if (!lhs || !rhs)
                return false;
            while (*lhs && *rhs)
            {
                if (std::towlower(*lhs) != std::towlower(*rhs))
                    return false;
                ++lhs;
                ++rhs;
            }
            return *lhs == *rhs;
        }

        bool String::EndsWithCaseInsensitive(const std::string &str, const std::string &suffix)
        {
            if (suffix.size() > str.size())
                return false;
            size_t offset = str.size() - suffix.size();
            for (size_t i = 0; i < suffix.size(); ++i)
            {
                char a = std::tolower(static_cast<unsigned char>(str[offset + i]));
                char b = std::tolower(static_cast<unsigned char>(suffix[i]));
                if (a != b)
                    return false;
            }
            return true;
        }

        std::string String::FormatIntWidth2(int val)
        {
            std::ostringstream ss;
            ss << std::setw(2) << std::setfill('0') << val;
            return ss.str();
        }

        std::string String::FormatIntWidthN(int val, int width)
        {
            std::ostringstream ss;
            if (width < 0)
                width = 0;
            ss << std::setw(width) << std::setfill('0') << val;
            return ss.str();
        }

        std::string String::FormatHexInt(int val)
        {
            std::ostringstream ss;
            ss << std::uppercase << std::hex << val;
            return ss.str();
        }

        std::string String::FormatHexUint32(uint32_t val)
        {
            std::ostringstream ss;
            ss << std::uppercase << std::hex << val;
            return ss.str();
        }

        std::string String::FormatByte(unsigned char value)
        {
            std::ostringstream ss;
            ss << std::uppercase << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(value);
            return ss.str();
        }

        std::string StringStreamToString(::std::stringstream *stream)
        {
            if (!stream)
                return std::string();
            return stream->str();
        }

    } // namespace internal
} // namespace testing
