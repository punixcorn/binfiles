#include <fmt/core.h>
#include <fmt/format.h>

#include <algorithm>
#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <utility>
#include <vector>

using fmt::print, fmt::format, std::string, std::vector, std::map, std::pair,
    std::make_pair;

// -h,--help | this shows the help message
namespace Engine {

struct option {
    std::function<bool()> func;
    std::string option_name;
    std::map<string, string> Options;
};

struct Parser {
   private:
    char **Args;
    int Argc;
    std::map<string, option> opt;

   public:
    Parser(int argc, char **argv, std::map<string, option> options)
        : Args(argv), Argc(argc), opt(options){};

    void ERR(string str, int status = 1) {
        fmt::print(stderr, "{} : [ERR] {}\n", program_invocation_name, str);
        exit(status);
    }  // ERR

   private:
    // check if needle is found in haystack
    // needle(0,2) is found in haystack(0..2) if checkShort if enabled
    bool isOptionFound(std::string haystack, std::string needle,
                       bool checkShort = true) {
        if (haystack == needle) return true;
        if (checkShort)
            if (haystack == needle.substr(1, 2)) return true;
        return false;
    };

    void lowerCase(std::string &data) {
        if (data.size() == 0) return;
        std::transform(data.begin(), data.end(), data.begin(),
                       [](unsigned char c) { return std::tolower(c); });
    }

    // checks if the str has a '-' in it
    void check_next(std::string str, std::string err_message) {
        if (str.size() != 0) {
            if (str[0] != '-') {
                return;
            }
        }
        ERR(err_message);

    }  // check_next

   public:
    void parse() {
        switch (Argc) {
            case 2:

                if (isOptionFound(string(Args[1]), "--help")) {
                    print("{}", "-h,--help");
                    exit(0);
                } else {
                    ERR("Invalid arguments passed", 1);
                }
                break;

            default:
                int i{1};
                while (i < Argc) {
                    if (isOptionFound(Args[i], "--language")) {
                        ++i;
                        check_next(Args[i],
                                   "-l / --language needs an argument");
                    } else if (isOptionFound(Args[i], "--project")) {
                        ++i;
                        check_next(Args[i],
                                   "--standard / -std needs an argument");
                    } else if (isOptionFound(Args[i], "--filename")) {
                        ++i;
                        check_next(Args[i],
                                   "--standard / -std needs an argument");
                    } else if (isOptionFound(Args[i], "--compiler")) {
                        ++i;
                        check_next(Args[i],
                                   "--standard / -std needs an argument");
                    } else if (isOptionFound(Args[i], "--standard", false) ||
                               isOptionFound(Args[i], "-std", false)) {
                        ++i;
                        check_next(Args[i],
                                   "--standard / -std needs an argument");
                    } else if (isOptionFound(Args[i], "--template")) {
                    }
                    i++;
                };
                break;
        };

        // ==== ====== ====== checks ====== ====== =====//

    }  // void Parse
};     // Engine

}  // namespace Engine
