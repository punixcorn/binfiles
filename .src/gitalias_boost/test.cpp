/*test.c*/
// #include <boost/program_options.hpp>
#include <iostream>
#include <fstream>
#include <cstdlib>/* c++ remake of stdlib*/
#include <cstdio> /* c++ remake of stdio*/
#include <string>
#include <vector>
// #include <boost/program_options/errors.hpp>
// #include <boost/algorithm/string.hpp>
/*C includes*/
#include <dirent.h>
#include <errno.h>
#include <unistd.h>

std::string reponame, repodes;
bool mode;

auto errorT2(std::string e) -> void
{
    std::cout << program_invocation_name << ": " << e;
    exit(1);
}

auto getGitInfo() -> bool
{
    FILE *fd;
    size_t len = 0, read;
    char *temp = new (std::nothrow) char[1024];
    std::string newTemp;
    /* apparently fopen will bug out if there is and error hence 2>&1 is needed */
    fd = popen("git status 2>&1 ", "r");
    if (fd == NULL || temp == NULL)
    {
        std::cerr << "Program Crashed...\n";
        exit(1);
    }
    char x[1024];
    fscanf(fd, " %1023s", temp);
    newTemp = temp;

    std::cout << " Temp = " << newTemp << std::endl;
    if (newTemp == "fatal:")
        return 0;
    return 1;
}

int main(int argc, char **argv)
{
    bool food = getGitInfo();
    std::cout << "Git repo : " << food;
}