#include "zip.h"

#include <filesystem>
#include <string>

#include "Makezip.h"
#include "defines.h"
#include "errormessage.h"

zip_t *zzip::init(const char *path, int flags) {
    // check if .zip is ADDED
    if (std::string(path).find(".zip")) {
    }
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

void *zzip::readBytes(zip_t *zip, const char *filename, int size,
                      uint64_t readsize, ...) {
    zip_file_t *zipfile = zip_fopen(zip, filename, 0);
    if (zipfile == null) exit(-1);
    char *buffer = new char[size];
    zip_fread(zipfile, buffer, readsize);
    return buffer;
}

void zzip::printFiles(zip_t *zip, ...) {
    for (int i = 0; i < zip_get_num_entries(zip, 0); i++) {
        const char *name = zip_get_name(zip, i, 0);
        if (name == null) {
            break;
        }
        std::print("{} ", name);
    }
    std::print("\n");
}

void zzip::printEntries(zip_t *zip, ...) {
    std::map<int, std::string> mp = zzip::getEntries(zip);
    for (auto i : zzip::getEntries(zip)) {
        std::print("{} : {}\n", i.first, i.second);
    }
    std::print("\n");
}

bool zzip::fileExists(zip_t *zip, const char *filename, ...) {
    for (int i = 0; i < zip_get_num_entries(zip, 0); i++) {
        const char *name = zip_get_name(zip, i, 0);
        if (std::string(name) == std::string(filename)) {
            return true;
        }
    }
    return false;
}

zip_entries_t zzip::getEntries(zip_t *zip, ...) {
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

void zzip::addDirectory(zip_t *zip, const char *dirname, ...) {
    if (!std::filesystem::is_directory(dirname)) {
        ERR::exitErr(std::format("directory {} not found", dirname));
    }

    zip_dir_add(zip, dirname, 0);
    // open dir/ add files
    for (const auto &entry : std::filesystem::directory_iterator(dirname)) {
        std::print("{}\n", entry.path().string());
    }

    exit(0);

    zzip::addFile(zip, std::format("{}/{}", dirname, dirname).c_str());
}

void zzip::addFile(zip_t *zip, const char *filename, zip_entries_t &zip_entries,
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

void zzip::addFile(zip_t *zip, const char *filename, ...) {
    zip_source_t *s = zip_source_file(zip, filename, 0, 0);
    int index = zip_file_add(zip, filename, s, ZIP_FL_OVERWRITE);
    zip_source_close(s);
    if (index == -1) {
        ERR::exitErr(std::format("Could not add file {}", filename), index);
    }
}

void zzip::add(zip_t *zip, const char *name, ...) {
    if (std::filesystem::is_directory(name)) {
        zzip::addDirectory(zip, name, null);
    } else {
        // zzip::addFile(zip, name, null);
    }
}

bool zzip::deleteFile(zip_t *z, const char *name, ...) {
    zip_entries_t entries = zzip::getEntries(z);
    int index = -1;
    for (const auto &i : entries) {
        if (i.second == name) {
            index = i.first;
            break;
        }
    }
    if (index == -1) {
        return false;
#ifdef debug
        std::print(stderr, "file {} not found in {}", name, zipname);
#endif
    }

    zip_delete(z, index);
    return true;
};

// if modified
time_t zzip::getFileModifiedTime(zip_t *zip, const char *filename, ...) {
    if (!zzip::fileExists(zip, filename)) {
        return (time_t)0;
    }
    zip_stat_t *stat = new zip_stat_t;
    if (zip_stat(zip, filename, 0, stat) == -1) {
        return (time_t)0;
    }
    return (time_t)stat->mtime;
}

// file checking is done here before any argument is passed into functions
void zzip::makezip(const alltargets &tempT, ...) {
    if (tempT.size() <= 0) {
        exit(2);
    }
    const alltargets T = orderTargets(tempT);

    for (const auto &target : T) {
        zip_t *z = zzip::init(target.targetName.c_str(), ZIP_CREATE);

        //  add a '*D' or '*A' to show if archive should be deleteing or adding
        std::string INS = target.ops == INS_ADD
                              ? std::format("{}{}*A{}", bold, green, nocol)
                              : std::format("{}{}*D{}", bold, red, nocol);
        std::print("{}[archive]{} : {} {}\n", bold, nocol, target.targetName,
                   INS);

        for (const auto &files : target.targets) {
            if (target.ops == INS_ADD) {  // adding file
                if (!std::filesystem::exists(std::format("./{}", files))) {
                    ERR::exitErr(std::format("{} not found\n", files), 2);
                };
                // check modified time before adding file
                struct stat filestat;
                stat(files.c_str(), &filestat);

                time_t zipFilemtime =
                    zzip::getFileModifiedTime(z, files.c_str());
                const double TimeDiff =
                    difftime(filestat.st_mtime, zipFilemtime);

#ifdef debug
                print("{} -> {} secs\n", files, TimeDiff);
                continue;
#endif

                if (TimeDiff <= 5) {
                    std::print("\t{}{}*{} {} {}[no changes]{}\n", bold, yellow,
                               nocol, files, bold, nocol);
                } else {
                    zzip::add(z, files.c_str());
                    std::print("\t{}{}+{} {}\n", bold, green, nocol, files);
                }

            } else if (target.ops == INS_DEL) {  // deleteing file
                // check if zip to delete from exists
                if (!std::filesystem::exists(
                        std::format("./{}", target.targetName))) {
                    ERR::exitErr(
                        std::format("zip {} not found\n", target.targetName),
                        2);
                };
                // checking if file is in zip before delete
                if (zzip::deleteFile(z, files.c_str())) {
                    std::print("\t{}{}-{} {}\n", bold, red, nocol, files);
                } else {
                    std::print("\t{}{}*{} {} {}[not found]{}\n", bold, yellow,
                               nocol, files, bold, nocol);
                }
            }
        }
        print("\n");
        zip_close(z);
    }
}
