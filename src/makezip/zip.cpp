#include "zip.h"

#include <format>

#include "Makezip.h"
#include "defines.h"
#include "errormessage.h"

zip_t *init_zip(const char *path, int flags) {
    int err;
    zip_t *zip;
    if ((zip = zip_open(path, flags, &err)) == null) {
        zip_error_t error;
        zip_error_init_with_code(&error, err);
        std::print(stderr, "could not open zip\n{}",
                   zip_error_strerror(&error));
        zip_error_fini(&error);
        exit(-1);
    }
    return zip;
}

void *readbytesfromzip(zip_t *zip, const char *filename, int size,
                       uint64_t readsize, ...) {
    zip_file_t *zipfile = zip_fopen(zip, filename, 0);
    if (zipfile == null) exit(-1);
    char *buffer = new char[size];
    zip_fread(zipfile, buffer, readsize);
    return buffer;
}

void printZipFiles(zip_t *zip, ...) {
    for (int i = 0; i < zip_get_num_entries(zip, 0); i++) {
        const char *name = zip_get_name(zip, i, 0);
        if (name == null) {
            break;
        }
        std::print("{} ", name);
    }
    std::print("\n");
}

void printZip(zip_t *zip, ...) {
    std::map<int, std::string> mp = getfilesandindexinzip(zip);
    for (auto i : getfilesandindexinzip(zip)) {
        std::print("{} : {}\n", i.first, i.second);
    }
    std::print("\n");
}

std::map<int, std::string> getfilesandindexinzip(zip_t *zip, ...) {
    zip_entries_t ret;
    for (int i = 0; i < zip_get_num_entries(zip, 0); i++) {
        const char *name = zip_get_name(zip, i, 0);
        if (name == null) {
            break;
        }
        ret.insert(std::pair(i, name));
    }
    return ret;
}

void addFiletoZip(zip_t *zip, const char *filename, zip_entries_t &zip_entries,
                  ...) {
    zip_source_t *s = zip_source_file(zip, filename, 0, 0);
    int index = zip_file_add(zip, filename, s, ZIP_FL_OVERWRITE);
    zip_source_close(s);
    if (index == -1) {
        ERR::exitErr("could not add file {}", -1);
    }

    int override = 0;
    for (auto i : zip_entries) {
        if (index == i.first) {
            override = 1;
            break;
        }
    }
    if (!override) zip_entries.insert(std::make_pair(index, filename));
}

void addFiletoZip(zip_t *zip, const char *filename, ...) {
    zip_source_t *s = zip_source_file(zip, filename, 0, 0);
    int index = zip_file_add(zip, filename, s, ZIP_FL_OVERWRITE);
    zip_source_close(s);
    if (index == -1) {
        ERR::exitErr(std::format("Could not add file {}", filename), index);
    }
}

bool deleteFilefromZip(zip_t *z, const char *name, const char *zipname...) {
    zip_entries_t entries = getfilesandindexinzip(z);
    int index = -1;
    for (const auto &i : entries) {
        if (i.second == name) {
            index = i.first;
            break;
        }
    }
    if (index == -1) {
        std::print(stderr, "file {} not found in {}", name, zipname);
    }
    zip_get_num_entries(z, index);
    return true;
};

// if modified
time_t getfileModifiedTime(zip_t *zip, const char *filename, ...) {
    zip_stat_t *stat;
    if (zip_stat(zip, filename, 0, stat) == -1) {
        return 0;
    }
    return stat->mtime;
}

void makezip(const alltargets &T, zip_entries_t &zip_entries) {
    if (T.size() <= 0) {
        exit(2);
    }

    for (const auto &target : T) {
        zip_t *z = init_zip(target.targetName.c_str(), ZIP_CREATE);

        //  add a '*D' or '*A' to show if archive should be deleteing or adding
        std::string INS = target.ops == INS_ADD
                              ? std::format("{}{}*A{}", bold, green, nocol)
                              : std::format("{}{}*D{}", bold, red, nocol);
        std::print("{}[archive]{} : {} {}\n", bold, nocol, target.targetName,
                   INS);

        for (const auto &files : target.targets) {
            // adding file
            if (target.ops == INS_ADD) {
                if (!std::filesystem::exists(std::format("./{}", files))) {
                    ERR::exitErr(std::format("file {} not found\n", files), 2);
                };
                addFiletoZip(z, files.c_str());
                std::print("\t{}{}+{} {}\n", bold, green, nocol, files);
            } else if (target.ops == INS_DEL) {
                // deleteing file
                if (!std::filesystem::exists(
                        std::format("./{}", target.targetName))) {
                    ERR::exitErr(
                        std::format("zip {} not found\n", target.targetName),
                        2);
                };
                deleteFilefromZip(z, files.c_str(), target.targetName.c_str());
                std::print("\t{}{}-{} {}\n", bold, red, nocol, files);
            }
        }
        print("\n");
        zip_close(z);
    }
}
