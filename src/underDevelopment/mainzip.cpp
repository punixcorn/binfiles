// this is a project to make a zip based on make with the command makezip
#include <zzip/info.h>
#include <zzip/zzip.h>

#include "engine.hpp"

int main(int argc, char **argv) {
    struct zzip_dirent *zip_dir =
        (struct zzip_dirent *)malloc(sizeof(struct zzip_dirent));

    if (!zip_dir) {
        return -1;
    }
    return 0;
}
