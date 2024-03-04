#include <fmt/core.h>
#include <fmt/format.h>
#include <unistd.h>

#include <stdexcept>
#include <string>
#include <vector>
using namespace std;
using namespace fmt;

#include "engine.hpp"
// -p <place> -d <disk>
// place disk
// -p place <disk>
// -d <disk> <place>

class Parser : public Engine
{
  private:
    int Argc;
    char **Argv;

  public:
    Parser(int argc, char **argv) : Argc(argc), Argv(argv)
    {
        if (argc <= 1)
        {
            fmt::print("Error Invalid arguments passed\n");
            exit(1);
        }
        parse();
    };

    ~Parser()
    {
        delete this;
    }

    void parse()
    {
        bool disk{false}, place{false};
        std::string disk_p{}, place_p{};
        int nonParamCount{0};
        switch (Argc)
        {
        case 2:
            if (isOptionFound(string(Argv[1]), "--help"))
            {
                print("{}", "-h,--help");
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
                if (isOptionFound(Argv[i], "--place"))
                {
                    ++i;
                    check_next(Argv[i], "--place needs an argument, where to mount "
                                        "the disk");
                    place = true;
                    try
                    {
                        place_p = std::string(Argv[i] == NULL ? throw : Argv[i]);
                    }
                    catch (const std::logic_error &e)
                    {
                        fmt::print("{}", e.what());
                    }
                }
                else if (isOptionFound(Argv[i], "--disk"))
                {
                    ++i;
                    check_next(Argv[i], "--disk needs an argument, which disk to mount");
                    disk = true;
                    disk_p = std::string(Argv[i]);
                }
                else
                {
                    nonParamCount++;
                }
                i++;
            }; // while

            break;
        };

        if (nonParamCount == 0)
        {
            if (disk && place)
            {
                print("{}\n", "disk && place passed");
            }
            else
            {
                ERR("Invalid args! at disk && place nonParamCount == 0");
            }
        }
        else if (nonParamCount == 1)
        {
            if (disk)
            {
                print("{}\n", "disk passed, one arg");
            }
            else if (place)
            {
                print("{}\n", "place passed, one arg");
            }
            else
            {
                ERR("Invalid args! at disk || place nonParamCount == 1");
            }
        }
        else if (nonParamCount == 2)
        {
            print("{}\n", "2 params");
        }
        else
        {
            ERR("ALL FAILED");
        }
    }
};

int main(int argc, char **argv)
{
    return 0;
}
