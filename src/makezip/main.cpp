#include "Makezip.h"
#include "defines.h"
#include "errormessage.h"
#include "zip.h"

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

    + more configs
 */
int main(int argc, char **argv) {
    alltargets T;
    zip_entries_t zip_entries;
    std::stringstream *buffer;

    buffer = getMakeZipContents(null);
    parseMakezip(buffer, T);
    if (argc > 1) {
        if ((strcmp(argv[1], "-t") == 0) ||
            (strcmp(argv[1], "--targets") == 0)) {
            printallTargets(T);
            exit(0);
        } else if ((strcmp(argv[1], "-h") == 0) ||
                   (strcmp(argv[1], "--help") == 0)) {
            print(
                "makezip : Create and update zipfiles using Makezip config\n"
                "makezip options:\n"
                "\t--help         print this message\n"
                "\t--targets      print all targets and exit\n"
                "for more info run {}man makezip{}",
                bold, nocol);
            exit(0);
        } else {
            ERR::exitErr(std::format("invalid option\nrun {}man makezip{} for "
                                     "more information\n",
                                     bold, nocol),
                         1);
        }
    }
    makezip(T, zip_entries);

    return 0;
}
