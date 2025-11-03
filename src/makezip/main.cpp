#include "zip.hpp"
// holds defines
#include "defines.h"

/*
    Not complete
    + file dependencies eg
        all.zip: add
            a
            b

        newall.zip: all.zip
            all.zip

    + modify zip files if exists not replace
    - no point in modifing, just replace
    - but can skipp adding

    + more configs
 */

int main(int argc, char **argv) {
    Makezip::alltargets T;
    std::stringstream *buffer;

    if (argc == 2) {
        if ((strcmp(argv[1], "-h") == 0) || (strcmp(argv[1], "--help") == 0)) {
            fmt::print(
                "makezip : Create and update zipfiles using Makezip config\n"
                "makezip options:\n"
                "\t--help         print this message\n"
                "\t--targets      print all targets and exit\n"
                "\t--changes      print all targets with changes and exit\n"
                "for more info run {}man makezip{}",
                bold, nocol);
            exit(0);
        }

        if ((strcmp(argv[1], "-t") == 0) ||
            (strcmp(argv[1], "--targets") == 0)) {
            buffer = Makezip::getMakeZipContents(null);
            parseMakezip(buffer, T);
            printallTargets(T);
            exit(0);
        } else if ((strcmp(argv[1], "-c") == 0) ||
                   (strcmp(argv[1], "--changes") == 0)) {
            buffer = Makezip::getMakeZipContents(null);
            parseMakezip(buffer, T);
            zzip::printChangedFiles(T);
            exit(0);
        } else {
            ERR::exitErr(fmt::format("invalid option\nrun {}man makezip{} for "
                                     "more information\n",
                                     bold, nocol),
                         1);
        }
    } else if (argc > 2) {
        ERR::exitErr(fmt::format("invalid option\nrun {}man makezip{} for "
                                 "more information\n",
                                 bold, nocol),
                     1);
    }

    buffer = Makezip::getMakeZipContents(null);
    parseMakezip(buffer, T);
    zzip::makezip(T);
}
