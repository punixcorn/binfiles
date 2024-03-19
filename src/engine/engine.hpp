#include <assert.h>
#include <fmt/core.h>
#include <fmt/format.h>

#include <algorithm>
#include <string>
#include <vector>

using std::string, fmt::print, fmt::format;
#define assert_if_null(arg) assert(arg != NULL);

namespace binfiles {

// this class is inherited and void parse(){} is developed in other files
class Engine {
   public:
    /*
     template <typename T>
     std::vector<std::string> collect_strings_from_array(T arr) {
         std::vector<std::string> return_vec{};

         if (std::is_compound_v<T> && std::is_array_v<T> && std::is_class_v<T>)
     { for (auto i : arr) { return_vec.push_back(i);
             }
         } else if (std::is_same_v<char **, T>) {
             int i = 0;
             while (arr[i] != NULL) {
                 return_vec.push_back(arr[i]);
                 i++;
             }
         }

         return return_vec.size() > 0 ? return_vec : throw;
     }

     */

    // tranforms char*[] into std::vector<std::string>
    std::vector<std::string> collect_strings_from_array(char **a) {
        assert(a != NULL);
        std::vector<std::string> t{};
        int i = 0;
        while (a[i] != NULL) {
            t.push_back(a[i]);
            i++;
        }
        return t.size() > 0 ? t : throw;
    };

    // debug outputs argc, and argv
    void dedug_argc_argv(int argc, char **argv) {
        fmt::print("argc {}\n", argc);

        fmt::print("Argv\n");

        std::vector<std::string> t = collect_strings_from_array(argv);
        for (const auto s : t) {
            fmt::print("- {}\n", s);
        }
    }

    /* template <typename T, typename... Args> void debug(T arg, Args... args)*/
    /* { */
    /*     fmt::print("{}\n", arg); */
    /*     debug(args...); */
    /* } */

    // terminates progream with str as error messsage and status as return value
    void ERR(std::string str, int status = 1) {
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
    void parse(){};

};  // void Parse

/*
   {
   char ** Argv()
   }
Parser::Parser(int argc, char **argv) : Engine() {
    assert(argv != null);
    assert(argc > 0);
    Argc = argc;
    Argv = argv;
};
*/
}  // namespace binfiles
