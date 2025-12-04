#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <filesystem>
#include <cstdlib>
#include <unistd.h>
#include <fstream>

#include "../include/filepath.hpp"

namespace fs = std::filesystem;

static std::vector<std::string> split_tokens(const std::string &line)
{
    std::istringstream iss(line);
    std::vector<std::string> toks;
    std::string t;
    while (iss >> t)
        toks.push_back(t);
    return toks;
}

static void print_help()
{
    std::cout << "Commands:\n"
                 "  help                         Show this help\n"
                 "  exit                         Quit\n"
                 "  pwd                          Show current directory (FilePath::GetCurrentDir)\n"
                 "  cd <path>                    Change current directory (uses chdir)\n"
                 "  ls [path]                    List directory (std::filesystem)\n"
                 "  mkdir <path>                 Create single directory (FilePath::CreateFolder)\n"
                 "  mkdirs <path>                Create directories recursively (FilePath::CreateDirectoriesRecursively)\n"
                 "  touch <file>                 Create empty file\n"
                 "  rm <path>                    Remove (recursive) using std::filesystem::remove_all\n"
                 "  sysrm <path>                 Remove using system(\"rm -rf ...\") to demonstrate system call\n"
                 "  exists <path>                Check existence (FilePath::FileOrDirectoryExists)\n"
                 "  isdir <path>                 Check if path is directory (FilePath::DirectoryExists)\n"
                 "  basename <path>              Show file name (RemoveDirectoryName)\n"
                 "  dirname <path>               Show directory (RemoveFileName)\n"
                 "  removeext <path> <ext>       Remove extension from filename (RemoveExtension)\n"
                 "  normalize <path>             Normalize path via FilePath ctor\n"
                 "  concat <dir> <rel>           Concat paths (ConcatPaths)\n"
                 "  genunique <dir> <base> <ext> Generate unique filename (GenerateUniqueFileName)\n";
}

static void RunSelfTests()
{
    using testing::internal::FilePath;
    namespace fs = std::filesystem;

    FilePath cwd = FilePath::GetCurrentDir();
    FilePath test_root = FilePath::ConcatPaths(cwd, FilePath(".fp_test_tmp"));

    // cleanup any previous run
    try
    {
        fs::remove_all(test_root.string());
    }
    catch (...)
    {
    }

    int passed = 0, total = 0;
    auto check = [&](bool cond, const char *name)
    {
        ++total;
        if (cond)
        {
            ++passed;
            std::cout << "[PASS] " << name << "\n";
        }
        else
            std::cout << "[FAIL] " << name << "\n";
    };

    // 1) Create nested directories recursively
    FilePath nested = FilePath::ConcatPaths(test_root, FilePath("a/b/c"));
    bool created = nested.CreateDirectoriesRecursively();
    check(created, "CreateDirectoriesRecursively");

    // 2) DirectoryExists on deepest dir
    check(FilePath::ConcatPaths(test_root, FilePath("a/b/c")).DirectoryExists(), "DirectoryExists");

    // 3) Create a file inside deepest dir and check FileOrDirectoryExists
    FilePath file = FilePath::ConcatPaths(FilePath::ConcatPaths(test_root, FilePath("a/b/c")), FilePath("hello.txt"));
    {
        std::ofstream ofs(file.string(), std::ios::trunc);
        if (ofs)
            ofs << "hi\n";
    }
    check(file.FileOrDirectoryExists(), "FileOrDirectoryExists (file)");

    // 4) RemoveExtension
    FilePath with_ext("myfile.TXT");
    FilePath without = with_ext.RemoveExtension("txt");
    check(without.string() == "myfile", "RemoveExtension (case-insensitive)");

    // 5) MakeFilename + ConcatPaths
    FilePath base("log");
    FilePath fname = FilePath::MakeFilename(test_root, base, 2, "log");
    // expected ends with "/log_2.log"
    check(fname.string().find("log_2.log") != std::string::npos, "MakeFilename / ConcatPaths");

    // 6) GenerateUniqueFileName: ensure unique when file exists
    // create example.log
    FilePath ex = FilePath::ConcatPaths(test_root, FilePath("example.log"));
    {
        std::ofstream ofs(ex.string(), std::ios::trunc);
        if (ofs)
            ofs << "x\n";
    }
    FilePath unique = FilePath::GenerateUniqueFileName(test_root, FilePath("example"), "log");
    check(unique.string() != ex.string(), "GenerateUniqueFileName avoids existing");

    // 7) RemoveDirectoryName / RemoveFileName
    FilePath some("/path/to/file.txt");
    check(some.RemoveDirectoryName().string() == "file.txt", "RemoveDirectoryName");
    check(some.RemoveFileName().string().find("/path/to/") != std::string::npos || some.RemoveFileName().string() == "./", "RemoveFileName");

    // 8) Normalize (constructor already normalizes), and RemoveTrailingPathSeparator
    FilePath messy("//a//b///c//");
    check(messy.string().find("//") == std::string::npos, "Normalize collapses separators");
    FilePath dirslash("/tmp/");
    check(dirslash.RemoveTrailingPathSeparator().string() == "/tmp", "RemoveTrailingPathSeparator");

    // 9) IsAbsolutePath / IsRootDirectory
    check(FilePath("/").IsRootDirectory(), "IsRootDirectory");
    check(FilePath("/etc").IsAbsolutePath(), "IsAbsolutePath");

    // Cleanup
    bool cleaned = false;
    try
    {
        fs::remove_all(test_root.string());
        cleaned = true;
    }
    catch (...)
    {
        cleaned = false;
    }
    check(cleaned, "Cleanup test directory");

    std::cout << "Self-tests: " << passed << " / " << total << " passed.\n";
}

int main()
{
    using testing::internal::FilePath;

    std::string line;
    print_help();

    while (true)
    {
        std::cout << "repl> " << std::flush;
        if (!std::getline(std::cin, line))
            break;
        auto toks = split_tokens(line);
        if (toks.empty())
            continue;

        const std::string &cmd = toks[0];

        if (cmd == "help")
            print_help();
        else if (cmd == "exit" || cmd == "quit")
            break;
        else if (cmd == "pwd")
        {
            FilePath cwd = FilePath::GetCurrentDir();
            std::cout << cwd.string() << "\n";
        }
        else if (cmd == "cd")
        {
            if (toks.size() < 2)
            {
                std::cout << "usage: cd <path>\n";
                continue;
            }
            const FilePath dest(toks[1]);
            if (chdir(dest.c_str()) == 0)
                std::cout << "cd -> " << FilePath::GetCurrentDir().string() << "\n";
            else
                std::cout << "cd failed\n";
        }
        else if (cmd == "ls")
        {
            FilePath target = (toks.size() >= 2) ? FilePath(toks[1]) : FilePath::GetCurrentDir();
            try
            {
                for (auto &p : fs::directory_iterator(target.string()))
                    std::cout << (p.is_directory() ? "d " : "f ") << p.path().filename().string() << "\n";
            }
            catch (const std::exception &e)
            {
                std::cout << "ls error: " << e.what() << "\n";
            }
        }
        else if (cmd == "mkdir")
        {
            if (toks.size() < 2)
            {
                std::cout << "usage: mkdir <path>\n";
                continue;
            }
            FilePath p(toks[1]);
            if (p.CreateFolder())
                std::cout << "created: " << p.string() << "\n";
            else
                std::cout << "failed\n";
        }
        else if (cmd == "mkdirs")
        {
            if (toks.size() < 2)
            {
                std::cout << "usage: mkdirs <path>\n";
                continue;
            }
            FilePath p(toks[1]);
            if (p.CreateDirectoriesRecursively())
                std::cout << "created (rec): " << p.string() << "\n";
            else
                std::cout << "failed\n";
        }
        else if (cmd == "touch")
        {
            if (toks.size() < 2)
            {
                std::cout << "usage: touch <file>\n";
                continue;
            }
            FilePath f(toks[1]);
            std::ofstream ofs(f.string(), std::ios::app);
            if (ofs)
                std::cout << "touched: " << f.string() << "\n";
            else
                std::cout << "touch failed\n";
        }
        else if (cmd == "rm")
        {
            if (toks.size() < 2)
            {
                std::cout << "usage: rm <path>\n";
                continue;
            }
            FilePath target(toks[1]);
            std::error_code ec;
            auto n = fs::remove_all(target.string(), ec);
            if (ec)
                std::cout << "rm error: " << ec.message() << "\n";
            else
                std::cout << "removed entries: " << n << "\n";
        }
        else if (cmd == "sysrm")
        {
            if (toks.size() < 2)
            {
                std::cout << "usage: sysrm <path>\n";
                continue;
            }
            // Warning: uses system rm -rf (demonstration)
            // Simple shell-escaping: wrap in single quotes and escape existing single quotes.
            std::string path = toks[1];
            std::string safe;
            for (char c : path)
            {
                if (c == '\'')
                    safe += "'\"'\"'";
                else
                    safe.push_back(c);
            }
            std::string cmdline = "rm -rf '" + safe + "'";
            int r = std::system(cmdline.c_str());
            std::cout << "sysrm returned: " << r << "\n";
        }
        else if (cmd == "exists")
        {
            if (toks.size() < 2)
            {
                std::cout << "usage: exists <path>\n";
                continue;
            }
            FilePath p(toks[1]);
            std::cout << (p.FileOrDirectoryExists() ? "yes\n" : "no\n");
        }
        else if (cmd == "isdir")
        {
            if (toks.size() < 2)
            {
                std::cout << "usage: isdir <path>\n";
                continue;
            }
            FilePath p(toks[1]);
            std::cout << (p.DirectoryExists() ? "yes\n" : "no\n");
        }
        else if (cmd == "basename")
        {
            if (toks.size() < 2)
            {
                std::cout << "usage: basename <path>\n";
                continue;
            }
            FilePath p(toks[1]);
            std::cout << p.RemoveDirectoryName().string() << "\n";
        }
        else if (cmd == "dirname")
        {
            if (toks.size() < 2)
            {
                std::cout << "usage: dirname <path>\n";
                continue;
            }
            FilePath p(toks[1]);
            std::cout << p.RemoveFileName().string() << "\n";
        }
        else if (cmd == "removeext")
        {
            if (toks.size() < 3)
            {
                std::cout << "usage: removeext <path> <ext>\n";
                continue;
            }
            FilePath p(toks[1]);
            std::cout << p.RemoveExtension(toks[2].c_str()).string() << "\n";
        }
        else if (cmd == "normalize")
        {
            if (toks.size() < 2)
            {
                std::cout << "usage: normalize <path>\n";
                continue;
            }
            FilePath p(toks[1]);
            std::cout << p.string() << "\n";
        }
        else if (cmd == "concat")
        {
            if (toks.size() < 3)
            {
                std::cout << "usage: concat <dir> <rel>\n";
                continue;
            }
            FilePath a(toks[1]), b(toks[2]);
            std::cout << FilePath::ConcatPaths(a, b).string() << "\n";
        }
        else if (cmd == "genunique")
        {
            if (toks.size() < 4)
            {
                std::cout << "usage: genunique <dir> <base> <ext>\n";
                continue;
            }
            FilePath dir(toks[1]), base(toks[2]);
            std::cout << FilePath::GenerateUniqueFileName(dir, base, toks[3].c_str()).string() << "\n";
        }
        else if (cmd == "runtests" || cmd == "selftest")
        {
            RunSelfTests();
        }
        else
            std::cout << "unknown command (help for list)\n";
    }

    return 0;
}
