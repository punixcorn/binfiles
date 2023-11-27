#include <algorithm>
#include <fmt/core.h>
#include <fmt/format.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
using namespace fmt;

struct Engine
{
  private:
    char **Args;
    int Argc;

  public:
    Engine(char **argv, int argc) : Args(argv), Argc(argc){};

    void ERR(string str, int status = 1)
    {
        fmt::print(stderr, "{} : [ERR] {}\n", program_invocation_name, str);
        exit(status);
    } // ERR

  private:
    // check if needle is found in haystack
    // needle(0,2) is found in haystack(0..2) if checkShort if enabled
    bool isOptionFound(std::string haystack, std::string needle, bool checkShort = true)
    {
        if (haystack == needle)
            return true;

        if (checkShort)
            if (haystack == needle.substr(1, 2))
                return true;

        return false;
    };

    void lowerCase(std::string &data)
    {
        if (data.size() == 0)
            return;
        std::transform(data.begin(), data.end(), data.begin(), [](unsigned char c) { return std::tolower(c); });
    }

    // checks if the str has a '-' in it
    void check_next(std::string str, std::string err_message)
    {
        if (str.size() != 0)
        {
            if (str[0] != '-')
            {
                return;
            }
        }
        ERR(err_message);

    } // check_next

  public:
    // parser -l -c -h -std -f -s
    void parse()
    {
        switch (Argc)
        {
        case 2:
            if (isOptionFound(string(Args[1]), "--help"))
            {
                fmt::print("{} : create a simple project startup \n"
                           "-h    --help                       output this "
                           "message\n"
                           "-l    --language   [c,cpp]         language of the "
                           "project*\n"
                           "-p    --project     name           name of the project*\n"
                           "-c    --complier   [cmake,make]    compiler Script  ( "
                           "default: "
                           "make )\n"
                           "-f    --filename   [filename]      custom file name ( "
                           "default: "
                           "main.[c,cpp] )\n"
                           "-std  --standard   [standard]      set a standard   ( "
                           "default: "
                           "-std=c2- )\n"
                           "-t   --template                        copy template files\n"
                           "\n"
                           "NB: -l and -p is compulsory\n"
                           "default files or \"template\" files will be looked for to be copied\n"
                           "Makefile       :   ~/.Makefile\n"
                           "CMakeLists.txt :   ~/.CMakeLists.txt\n"
                           "cpp files      :   ~/.template.cpp\n"
                           "c files        :   ~/.template.c\n"
                           "\n"
                           "Use --template / -t to use template files\n"
                           "But you will have to edit them mannually\n",
                           program_invocation_name);

                exit(0);
            }
            else
            {
                ERR("Invalid arguments passed", 1);
            }
            break;
        default:
            int i{1};
            while (i < Argc)
            {
                if (isOptionFound(Args[i], "--language"))
                {
                    ++i;
                    check_next(Args[i], "-l / --language needs an argument");
                }
                else if (isOptionFound(Args[i], "--project"))
                {
                    ++i;
                    check_next(Args[i], "--standard / -std needs an argument");
                }
                else if (isOptionFound(Args[i], "--filename"))
                {
                    ++i;
                    check_next(Args[i], "--standard / -std needs an argument");
                }
                else if (isOptionFound(Args[i], "--compiler"))
                {
                    ++i;
                    check_next(Args[i], "--standard / -std needs an argument");
                }
                else if (isOptionFound(Args[i], "--standard", false) || isOptionFound(Args[i], "-std", false))
                {
                    ++i;
                    check_next(Args[i], "--standard / -std needs an argument");
                }
                else if (isOptionFound(Args[i], "--template"))
                {
                }
                i++;
            };
            break;
        };

        // ==== ====== ====== checks ====== ====== =====//

    } // void Parse
};    // Engine
int main(void)
{
    return 0;
}
