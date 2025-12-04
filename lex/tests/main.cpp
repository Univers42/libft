#include <iostream>
#include <string>
#include <vector>

extern "C"
{
#include "lexer.h"
#include "ds.h"
#include "ft_stdio.h"
#include "ft_stdlib.h"
#include "ft_math.h"
#include "ft_debug.h"
#include "ft_ctype.h"
    /* Minimal forward declarations to ensure C++ test file sees the C API
       in case header declarations differ or are hidden by conditional macros. */
    char *tokenizer(char *str, t_deque *ret);
    void print_tokens(t_deque *tokens);
    void print_token_summary(t_deque *tokens);
    void deque_destroy(t_deque *ret);
    int deque_init(t_deque *ret, size_t initial_cap, size_t elem_size, void *ctx);
}

void test_tokenizer(const char *input)
{
    t_deque tokens;
    char *prompt;

    std::cout << "\n========================================\n";
    std::cout << "Testing input: \"" << input << "\"\n";
    std::cout << "========================================\n";

    // Initialize deque for tokens (provide capacity, elem size, ctx=NULL)
    deque_init(&tokens, 8, sizeof(t_token), NULL);

    // Tokenize
    prompt = tokenizer((char *)input, &tokens);

    if (prompt)
    {
        std::cout << "Incomplete input, prompt: " << prompt << "\n";
    }

    // Print tokens
    print_tokens(&tokens);

    // Print summary
    print_token_summary(&tokens);

    // Cleanup
    deque_destroy(&tokens);
}

void run_test_suite()
{
    std::vector<std::string> test_cases = {
        // Basic commands
        "ls -la",
        "echo hello world",
        "cat file.txt",

        // Pipes and redirections
        "ls | grep test",
        "cat < input.txt > output.txt",
        "echo test >> file.txt",
        "cat << EOF",

        // Logical operators
        "command1 && command2",
        "command1 || command2",
        "command1; command2",

        // Quotes
        "echo 'single quoted'",
        "echo \"double quoted\"",
        "echo `backtick`",

        // Variable expansion
        "echo $HOME",
        "echo ${VAR}",
        "echo $?",
        "echo $$",

        // Complex combinations
        "ls -la | grep test > output.txt",
        "if [ -f file ]; then echo exists; fi",
        "(cd /tmp && ls)",

        // Special characters
        "echo test*",
        "ls file?.txt",
        "echo [abc]",

        // Incomplete (should return prompt)
        "echo 'incomplete",
        "echo \"incomplete",
    };

    for (const auto &test : test_cases)
    {
        test_tokenizer(test.c_str());
    }
}

void interactive_mode()
{
    std::string input;
    t_deque tokens;

    deque_init(&tokens, 8, sizeof(t_token), NULL);

    std::cout << "\n=== Interactive Tokenizer ===\n";
    std::cout << "Enter commands to tokenize (Ctrl+D to exit):\n\n";

    while (true)
    {
        std::cout << "> ";
        if (!std::getline(std::cin, input))
            break;

        if (input.empty())
            continue;

        if (input == "exit" || input == "quit")
            break;

        char *prompt = tokenizer((char *)input.c_str(), &tokens);

        if (prompt)
        {
            std::cout << "Incomplete: " << prompt << "\n";
        }
        else
        {
            print_tokens(&tokens);
        }

        deque_clear(&tokens);
    }

    deque_destroy(&tokens);
    std::cout << "\nGoodbye!\n";
}

int main(int argc, char **argv)
{
    if (argc > 1)
    {
        // Test mode with command-line argument
        if (std::string(argv[1]) == "--test" || std::string(argv[1]) == "-t")
        {
            run_test_suite();
        }
        else if (std::string(argv[1]) == "--interactive" || std::string(argv[1]) == "-i")
        {
            interactive_mode();
        }
        else
        {
            // Tokenize single argument
            test_tokenizer(argv[1]);
        }
    }
    else
    {
        std::cout << "Usage:\n";
        std::cout << "  " << argv[0] << " <command>          - Tokenize a single command\n";
        std::cout << "  " << argv[0] << " --test|-t          - Run test suite\n";
        std::cout << "  " << argv[0] << " --interactive|-i   - Interactive mode\n";
        return 1;
    }

    return 0;
}
