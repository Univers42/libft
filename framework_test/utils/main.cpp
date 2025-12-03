/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 13:41:33 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/03 14:22:49 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <cwchar>
#include "internal/string.hpp"

int main()
{
    using testing::internal::String;
    auto yesno = [](bool b)
    { return b ? "yes" : "no"; };

    // CloneCString
    const char *s = "Hello";
    const char *copy = String::CloneCString(s);
    std::cout << "CloneCString: " << (copy ? copy : "(null)") << "\n";
    delete[] copy;

    const char *null_copy = String::CloneCString(nullptr);
    std::cout << "CloneCString(nullptr): " << (null_copy ? null_copy : "(null)") << "\n";
    delete[] null_copy; // safe even if nullptr

    // CStringEquals
    std::cout << "CStringEquals(\"abc\",\"abc\"): " << yesno(String::CStringEquals("abc", "abc")) << "\n";
    std::cout << "CStringEquals(\"abc\",\"ab\"): " << yesno(String::CStringEquals("abc", "ab")) << "\n";
    std::cout << "CStringEquals(nullptr,\"\"): " << yesno(String::CStringEquals(nullptr, "")) << "\n";

    // ShowWideCString
    const wchar_t *w = L"héllø ☃";
    std::string from_w = String::ShowWideCString(w);
    std::cout << "ShowWideCString: " << from_w << "\n";
    std::cout << "ShowWideCString(nullptr): " << String::ShowWideCString(nullptr) << "\n";

    // WideCStringEquals
    std::cout << "WideCStringEquals(L\"abc\",L\"abc\"): "
              << yesno(String::WideCStringEquals(L"abc", L"abc")) << "\n";
    std::cout << "WideCStringEquals(L\"a\",nullptr): "
              << yesno(String::WideCStringEquals(L"a", nullptr)) << "\n";

    // Case-insensitive C string and wide C string comparisons
    std::cout << "CaseInsensitiveCStringEquals(\"AbC\",\"aBc\"): "
              << yesno(String::CaseInsensitiveCStringEquals("AbC", "aBc")) << "\n";
    std::cout << "CaseInsensitiveWideCStringEquals(L\"AbC\",L\"aBc\"): "
              << yesno(String::CaseInsensitiveWideCStringEquals(L"AbC", L"aBc")) << "\n";

    // EndsWithCaseInsensitive
    std::string text = "Example.TXT";
    std::cout << "EndsWithCaseInsensitive(\"" << text << "\",\"txt\"): "
              << yesno(String::EndsWithCaseInsensitive(text, "txt")) << "\n";
    std::cout << "EndsWithCaseInsensitive with empty suffix: "
              << yesno(String::EndsWithCaseInsensitive(text, "")) << "\n";

    // Formatting helpers
    std::cout << "FormatIntWidth2(5): " << String::FormatIntWidth2(5) << "\n";
    std::cout << "FormatIntWidthN(42,5): " << String::FormatIntWidthN(42, 5) << "\n";
    std::cout << "FormatHexInt(255): " << String::FormatHexInt(255) << "\n";
    std::cout << "FormatHexUint32(3735928559): " << String::FormatHexUint32(3735928559u) << "\n";
    std::cout << "FormatByte(255): " << String::FormatByte(255) << "\n";

    // StringStreamToString
    std::stringstream ss;
    ss << "stream test";
    std::cout << "StringStreamToString(&ss): " << testing::internal::StringStreamToString(&ss) << "\n";
    std::cout << "StringStreamToString(nullptr): \"" << testing::internal::StringStreamToString(nullptr) << "\"\n";

    return 0;
}