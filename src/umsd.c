#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* [ file name ] : umsd
 * [ file purpose ] : a wrapper umount
 * [ mode ] : stable
 * [ sample ] : umsd /mnt
 * [ sample2 ] : umsd -lsblk
 */

char command[100];
int main(int argc, char** argv) {
    if (argc == 2) {
        if (strcmp("-h", *(argv + 1)) == 0) {
            fprintf(stdout,
                    "Usage : %s [ options... ]\n"
                    "<directory>  un-mount this directory\n"
                    "-h           print this message\n"
                    "-lsblk       run lsblk first\n",
                    *(argv));
            exit(0);
        } else if (strcmp("-lsblk", *(argv + 1)) == 0) {
            FILE* fp;
            fp = popen("lsblk", "r");
            char* buffer = (char*)malloc(sizeof(char));
            if (fp == NULL || buffer == NULL) {
                fprintf(stderr,
                        "%s : ERR: unable to open lsblk and allocate space\n",
                        *(argv));
                pclose(fp);
                exit(1);
            }
            while (fread(buffer, sizeof(*buffer), 1, fp) == 1) {
                fwrite(buffer, sizeof(*buffer), 1, stdout);
            }
            free(buffer);
            pclose(fp);
            printf("un-mount: ");
            scanf("%s", *(argv + 1));
        }

        goto argc2;
    }

    fprintf(stderr,
            "%s : error, invaild arguments passed...\n"
            "Try %s -h for help\n",
            *(argv), *(argv));
    exit(1);
argc2:
    sprintf(command, " sudo umount %s -f -l -v ", *(argv + 1));
    system(command);
    exit(0);
}
