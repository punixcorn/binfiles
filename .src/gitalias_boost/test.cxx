#include <boost/program_options.hpp>
#include <iostream>
#include <fstream>
#include <cstdlib> /* c++ remake of stdlib*/
#include <cstdio>  /* c++ remake of stdio*/
#include <string>
#include <vector>
#include <boost/program_options/errors.hpp>
#include <boost/algorithm/string.hpp>
/*C includes*/
#include <dirent.h>
using namespace std;
void errorT2(string e)
{
    cerr << e << endl;
}

auto Checkadd() -> bool
{
    FILE *fd;
    char *temp = new (std::nothrow) char[1024];
    fd = popen("git status", "r");
    if (fd == NULL || temp == NULL)
        errorT2("Program Crahsed...\n");
    while (fscanf(fd, "%1023s", temp) != EOF)
    {
        cout << temp << " <<>> ";
        if (strcmp(temp, "Changes") == 0)
            return true;
    };
    return false;
}

bool checkadd2()
{
    FILE *fd;
    char *temp = new (std::nothrow) char[1024];
    fd = popen("git status", "r");
    if (fd == NULL || temp == NULL)
        errorT2("Program Crahsed...\n");

    while (fgets(temp, 1023, fd))
    {
        string foo = temp;
        cout << temp << " << " << foo.length() << " >> ";
        if (foo == "Changes not staged for commit:\n" || foo == "Changes to be committed:\n")
            return true;
    };
    return false;
}

auto getGitInfo() -> bool
{
    FILE *fd;
    char *temp = new (std::nothrow) char[1024];
    std::string newTemp;
    bool tempTrip{false};
    /* apparently fopen will bug out if there is and error hence 2>&1 is needed */
    fd = popen("git status 2>&1 ", "r");
    if (fd == NULL || temp == NULL)
        errorT2("Program Crashed...\n");
    while (fgets(temp, 1024, fd))
    {
        std::string tempcmp;
        tempcmp = temp;
        if (tempcmp == "fatal: not a git repository (or any of the parent directories): .git\n")
            return tempTrip;
        else
            tempTrip = true;
    }
    /* second check lol */
    if (tempTrip)
    {
        fscanf(fd, " %1023s", temp);
        newTemp = temp;
        if (newTemp == "fatal:")
            return !tempTrip;
    }
    return tempTrip;
}
typedef FILE file;
int main()
{
    bool test = getGitInfo();
    cout << " Test :" << test << endl;
}