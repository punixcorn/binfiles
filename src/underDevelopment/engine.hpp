#include <fmt/core.h>
#include <fmt/format.h>

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using fmt::print, fmt::format, std::string, std::vector;

// -h,--help | this shows the help message

struct Engine {
   public:
    int Argc;
    char **Argv;

    Engine(int argc, char **argv) : Argc(argc), Argv(argv) {
        if (argc <= 1) {
            fmt::print("Error Invalid arguments passed\n");
            exit(1);
        }
        parse();
    };

    Engine(Engine &e) = default;
    Engine(Engine &&e) = default;
    Engine(Engine *e);

    ~Engine() { delete this; }

    //
    void ERR(string str, int status = 1) {
        fmt::print(stderr, "{} : [ERR] {}\n", program_invocation_name, str);
        exit(status);
    }

    //
    [[nodiscard("returns true if the str starts with a -")]] bool Is_param(
        const char *str) {
        return false;
    }

    //
    bool isOptionFound(std::string haystack, std::string needle,
                       bool checkShort = true) {
        if (haystack == needle) return true;
        if (checkShort)
            if (haystack == needle.substr(1, 2)) return true;
        return false;
    };

    // change string into lowerCase
    void lowerCase(std::string &data) {
        if (data.size() == 0) return;
        std::transform(data.begin(), data.end(), data.begin(),
                       [](unsigned char c) { return std::tolower(c); });
    }

    // checks if the str starts with a '-'
    void check_next(std::string str, std::string err_message) {
        if (str.size() != 0) {
            if (str[0] != '-') {
                return;
            }
        }
        ERR(err_message);
    }

    void parse(){};

};  // void Parse
