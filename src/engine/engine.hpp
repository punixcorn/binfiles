#include <fmt/core.h>
#include <fmt/format.h>
#include <fmt/std.h>

#include <algorithm>
#include <string>
#include <vector>

using fmt::print, fmt::format, std::string, std::vector;

// this class is inherited and void parse(){} is developed in other files
struct Engine {
   public:
    int Argc;
    char **Argv;
    vector<string> debug_info;

    Engine(int argc, char **argv) : Argc(argc), Argv(argv) {
        if (argc <= 1) {
            fmt::print("Error Invalid arguments passed\n");
            exit(1);
        }
    };

    Engine(Engine &e) = default;
    Engine(Engine &&e) = default;
    Engine(Engine *e);

    // debug outputs argc, and argv
    void dedug_argc_argv() {
        fmt::print("argc {}\n", Argc);

        fmt::print("Argv\n");

        auto collect = [](char **a) {
            vector<string> t{};
            int i = 0;
            while (a[i] != NULL) {
                t.push_back(a[i]);
                i++;
            }
            return t.size() > 0 ? t : throw;
        };
        vector<std::string> t = collect(Argv);
        for (const auto s : t) {
            fmt::print("- {}\n", s);
        }
    }

    /* template <typename T, typename... Args> void debug(T arg, Args... args)
     */
    /* { */

    /*     fmt::print("{}\n", arg); */
    /*     debug(args...); */
    /* } */

    // terminates progream with str as error messsage and status as return value
    void ERR(string str, int status = 1) {
        fmt::print(stderr, "{} : [ERR] {}\n", program_invocation_name, str);
        exit(status);
    }

    // checks is an option compared against the current Argv is found
    // checkshort will search for a short option eg --help  will be -h
    [[nodiscard("return true if an option was found")]] bool isOptionFound(
        std::string current_argv, std::string compare_option,
        bool checkShort = true) {
        if (current_argv == compare_option) return true;
        if (checkShort)
            if (current_argv == compare_option.substr(1, 2)) return true;
        return false;
    };

    // change string into lowerCase
    void lowerCase(std::string &data) {
        if (data.size() == 0) return;
        std::transform(data.begin(), data.end(), data.begin(),
                       [](unsigned char c) { return std::tolower(c); });
    }

    // checks if the str starts with a '-'
    // checks if str is a parameter by finding the first char to be '-'
    void validate_option_arg(std::string str, std::string err_message) {
        if (str.size() != 0) {
            if (str[0] != '-') {
                return;
            }
        }
        ERR(err_message);
    }

    // custom funtion to parse Argv and Argc
    void parse(void) {};

};  // void Parse
