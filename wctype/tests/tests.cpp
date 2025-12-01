#include <iostream>
#include <cwchar>
#include <cwctype>
#include <clocale>
#include <vector>
#include <string>
#include <functional>
#include <iomanip>

// include C header with C linkage
extern "C"
{
#include "../ft_wctype.h"
}

struct TestRunner
{
    int pass = 0;
    int fail = 0;

    void expect(bool cond, const std::wstring &name, const std::wstring &detail = L"")
    {
        if (cond)
        {
            ++pass;
            std::wcout << L"[PASS] " << name;
            if (!detail.empty())
                std::wcout << L": " << detail;
            std::wcout << L'\n';
        }
        else
        {
            ++fail;
            std::wcout << L"[FAIL] " << name;
            if (!detail.empty())
                std::wcout << L": " << detail;
            std::wcout << L'\n';
        }
    }

    void summary() const
    {
        std::wcout << L"\nSUMMARY: " << pass << L" passed, " << fail << L" failed\n";
    }
};

// small helper to print a wchar_t in human + hex form
static std::wstring wc_info(wint_t wc)
{
    wchar_t buf[8] = {0};
    if (iswprint(wc))
    {
        std::swprintf(buf, sizeof(buf) / sizeof(wchar_t), L"'%lc' (0x%02X)", (wchar_t)wc, (unsigned)wc);
    }
    else
    {
        std::swprintf(buf, sizeof(buf) / sizeof(wchar_t), L"(0x%02X)", (unsigned)wc);
    }
    return std::wstring(buf);
}

int main()
{
    std::setlocale(LC_ALL, "");

    TestRunner tr;

    // quick smoke checks (retain some of the readable one-off tests)
    tr.expect(ft_iswalpha(L'A'), L"iswalpha('A')");
    tr.expect(!ft_iswalpha(L'1'), L"iswalpha('1') == false");
    tr.expect(ft_iswdigit(L'5'), L"iswdigit('5')");
    tr.expect(ft_iswxdigit(L'A'), L"iswxdigit('A')");
    tr.expect(ft_towlower(L'A') == L'a', L"towlower('A') == 'a'");
    tr.expect(ft_towupper(L'a') == L'A', L"towupper('a') == 'A'");

    // Exhaustive ASCII checks (0..127) comparing to standard isw* family.
    // Table of checks: pair of name and two functions: our ft_ and C std isw*
    struct Check
    {
        std::wstring name;
        std::function<int(wint_t)> mine;
        std::function<int(wint_t)> stdref;
    };
    std::vector<Check> checks = {
        {L"iswalnum", [](wint_t c)
         { return ft_iswalnum(c); }, [](wint_t c)
         { return std::iswalnum(c); }},
        {L"iswalpha", [](wint_t c)
         { return ft_iswalpha(c); }, [](wint_t c)
         { return std::iswalpha(c); }},
        {L"iswblank", [](wint_t c)
         { return ft_iswblank(c); }, [](wint_t c)
         { return std::iswblank(c); }},
        {L"iswcntrl", [](wint_t c)
         { return ft_iswcntrl(c); }, [](wint_t c)
         { return std::iswcntrl(c); }},
        {L"iswdigit", [](wint_t c)
         { return ft_iswdigit(c); }, [](wint_t c)
         { return std::iswdigit(c); }},
        {L"iswgraph", [](wint_t c)
         { return ft_iswgraph(c); }, [](wint_t c)
         { return std::iswgraph(c); }},
        {L"iswlower", [](wint_t c)
         { return ft_iswlower(c); }, [](wint_t c)
         { return std::iswlower(c); }},
        {L"iswprint", [](wint_t c)
         { return ft_iswprint(c); }, [](wint_t c)
         { return std::iswprint(c); }},
        {L"iswpunct", [](wint_t c)
         { return ft_iswpunct(c); }, [](wint_t c)
         { return std::iswpunct(c); }},
        {L"iswspace", [](wint_t c)
         { return ft_iswspace(c); }, [](wint_t c)
         { return std::iswspace(c); }},
        {L"iswupper", [](wint_t c)
         { return ft_iswupper(c); }, [](wint_t c)
         { return std::iswupper(c); }},
        {L"iswxdigit", [](wint_t c)
         { return ft_iswxdigit(c); }, [](wint_t c)
         { return std::iswxdigit(c); }}};

    for (const auto &chk : checks)
    {
        int mismatches_shown = 0;
        for (int i = 0; i < 128; ++i)
        {
            wint_t wc = (wint_t)i;
            bool mine = !!chk.mine(wc);
            bool expected = !!chk.stdref(wc);
            if (mine != expected)
            {
                if (mismatches_shown < 5)
                {
                    tr.expect(false, L"Mismatch " + chk.name, wc_info(wc) + L" -> mine=" + (mine ? L"1" : L"0") + L" expected=" + (expected ? L"1" : L"0"));
                }
                ++mismatches_shown;
            }
        }
        tr.expect(mismatches_shown == 0, L"exhaustive ASCII check: " + chk.name, mismatches_shown ? (std::to_wstring(mismatches_shown) + L" mismatches") : L"");
    }

    // Test ft_wctype name -> enum mapping and ft_iswctype interop.
    std::vector<std::pair<std::wstring, t_wctype>> name_map = {
        {L"alnum", FT_WCTYPE_ALNUM},
        {L"alpha", FT_WCTYPE_ALPHA},
        {L"blank", FT_WCTYPE_BLANK},
        {L"cntrl", FT_WCTYPE_CNTRL},
        {L"digit", FT_WCTYPE_DIGIT},
        {L"graph", FT_WCTYPE_GRAPH},
        {L"lower", FT_WCTYPE_LOWER},
        {L"print", FT_WCTYPE_PRINT},
        {L"punct", FT_WCTYPE_PUNCT},
        {L"space", FT_WCTYPE_SPACE},
        {L"upper", FT_WCTYPE_UPPER},
        {L"xdigit", FT_WCTYPE_XDIGIT},
        {L"unknown", FT_WCTYPE_NONE}};

    // verify mapping values
    for (auto &p : name_map)
    {
        t_wctype got = ft_wctype(p.first.c_str());
        tr.expect(got == p.second, L"ft_wctype(\"" + p.first + L"\")", L"got=" + std::to_wstring((int)got) + L" expected=" + std::to_wstring((int)p.second));
    }

    // reference map for ft_iswctype checks
    std::vector<std::pair<std::wstring, std::function<int(wint_t)>>> refmap = {
        {L"alnum", [](wint_t c)
         { return std::iswalnum(c); }},
        {L"alpha", [](wint_t c)
         { return std::iswalpha(c); }},
        {L"blank", [](wint_t c)
         { return std::iswblank(c); }},
        {L"cntrl", [](wint_t c)
         { return std::iswcntrl(c); }},
        {L"digit", [](wint_t c)
         { return std::iswdigit(c); }},
        {L"graph", [](wint_t c)
         { return std::iswgraph(c); }},
        {L"lower", [](wint_t c)
         { return std::iswlower(c); }},
        {L"print", [](wint_t c)
         { return std::iswprint(c); }},
        {L"punct", [](wint_t c)
         { return std::iswpunct(c); }},
        {L"space", [](wint_t c)
         { return std::iswspace(c); }},
        {L"upper", [](wint_t c)
         { return std::iswupper(c); }},
        {L"xdigit", [](wint_t c)
         { return std::iswxdigit(c); }}};

    for (auto &p : refmap)
    {
        t_wctype typ = ft_wctype(p.first.c_str());
        int mismatch_count = 0;
        for (int i = 0; i < 128; ++i)
        {
            wint_t wc = (wint_t)i;
            bool mine = !!ft_iswctype(wc, typ);
            bool expected = !!p.second(wc);
            if (mine != expected)
            {
                if (mismatch_count < 5)
                    tr.expect(false, L"ft_iswctype mismatch for " + p.first, wc_info(wc) + L" mine=" + (mine ? L"1" : L"0") + L" expected=" + (expected ? L"1" : L"0"));
                ++mismatch_count;
            }
        }
        tr.expect(mismatch_count == 0, L"ft_iswctype exhaustive: " + p.first, mismatch_count ? (std::to_wstring(mismatch_count) + L" mismatches") : L"");
    }

    // conversions: ensure ft_towlower / ft_towupper agree with standard for ASCII letters
    int conv_mismatch = 0;
    for (int i = 0; i < 128; ++i)
    {
        wint_t wc = (wint_t)i;
        wint_t mine_lo = ft_towlower(wc);
        wint_t std_lo = std::towlower(wc);
        if (mine_lo != std_lo)
        {
            if (conv_mismatch < 6)
                tr.expect(false, L"towlower mismatch", wc_info(wc) + L" mine=" + std::to_wstring(mine_lo) + L" std=" + std::to_wstring(std_lo));
            ++conv_mismatch;
        }
        wint_t mine_up = ft_towupper(wc);
        wint_t std_up = std::towupper(wc);
        if (mine_up != std_up)
        {
            if (conv_mismatch < 6)
                tr.expect(false, L"towupper mismatch", wc_info(wc) + L" mine=" + std::to_wstring(mine_up) + L" std=" + std::to_wstring(std_up));
            ++conv_mismatch;
        }
    }
    tr.expect(conv_mismatch == 0, L"conversion functions (towlower/towupper) exhaustive ASCII", conv_mismatch ? (std::to_wstring(conv_mismatch) + L" mismatches") : L"");

    tr.summary();
    return (tr.fail == 0) ? 0 : 1;
}
