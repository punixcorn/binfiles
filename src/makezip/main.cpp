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
    - but can skipp adding

    + more configs
 */

int main(int argc, char **argv) {
    alltargets T;
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
            ERR::exitErr(fmt::format("invalid option\nrun {}man makezip{} for "
                                     "more information\n",
                                     bold, nocol),
                         1);
        }
    }
    zzip::makezip(T);

    /* zip_t *z = zzip::init("z.zip"); */
    /* time_t t = zzip::getFileModifiedTime(z, "defines.h"); */

    /* if (t == 0) { */
    /*     return -1; */
    /* } */

    /* fmt::print("main.cpp -> {}\n", ctime(&t)); */
    /* printf(" File access time %s", ctime(&filestat.st_atime)); */
    /* printf(" File modify time %s", ctime(&filestat.st_mtime)); */
    /* printf("File changed time %s", ctime(&filestat.st_ctime)); */
    /* return 0; */
}
