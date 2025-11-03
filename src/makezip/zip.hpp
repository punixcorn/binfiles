// this is a project to make a zip based on make with the command makezip

#include <zip.h>

#include <cstdint>

#include "includes.hpp"
// hold definitions
#include "Makezip.hpp"
#include "defines.h"

using zip_entries_t = std::map<int, std::string>;

/*================================================*/
namespace zzip {

inline zip_t *init(const char *path, int flags) {
    // check if .zip is ADDED
    if (std::string(path).find(".zip")) {
    }
    int err;
    zip_t *zip;
    if ((zip = zip_open(path, flags, &err)) == null) {
        zip_error_t error;
        zip_error_init_with_code(&error, err);
        fmt::print(stderr, "could not open zip\n{}",
                   zip_error_strerror(&error));
        zip_error_fini(&error);
        exit(-1);
    }
    return zip;
}

inline zip_entries_t getEntries(zip_t *zip, ...) {
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

inline void *readBytes(zip_t *zip, const char *filename, int size,
                       uint64_t readsize, ...) {
    zip_file_t *zipfile = zip_fopen(zip, filename, 0);
    if (zipfile == null) exit(-1);
    char *buffer = new char[size];
    zip_fread(zipfile, buffer, readsize);
    return buffer;
}

inline void printFiles(zip_t *zip, ...) {
    for (int i = 0; i < zip_get_num_entries(zip, 0); i++) {
        const char *name = zip_get_name(zip, i, 0);
        if (name == null) {
            break;
        }
        fmt::print("{} ", name);
    }
    fmt::print("\n");
}

inline void printEntries(zip_t *zip, ...) {
    std::map<int, std::string> mp = getEntries(zip);
    for (auto i : getEntries(zip)) {
        fmt::print("{} : {}\n", i.first, i.second);
    }
    fmt::print("\n");
}

inline bool fileExists(zip_t *zip, const char *filename, ...) {
    for (int i = 0; i < zip_get_num_entries(zip, 0); i++) {
        const char *name = zip_get_name(zip, i, 0);
        if (std::string(name) == std::string(filename)) {
            return true;
        }
    }
    return false;
}

inline void addFile(zip_t *zip, const char *filename,
                    zip_entries_t &zip_entries, ...) {
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

inline void addFile(zip_t *zip, const char *filename, ...) {
    zip_source_t *s = zip_source_file(zip, filename, 0, 0);
    int index = zip_file_add(zip, filename, s, ZIP_FL_OVERWRITE);
    zip_source_close(s);
    if (index == -1) {
        ERR::exitErr(fmt::format("Could not add file {}", filename), index);
    }
}

inline void addDirectory(zip_t *zip, const char *dirname, ...) {
    if (!std::filesystem::is_directory(dirname)) {
        ERR::exitErr(fmt::format("directory {} not found", dirname));
    }

    zip_dir_add(zip, dirname, 0);

    for (const auto &entry :
         std::filesystem::recursive_directory_iterator(dirname)) {
        std::string entry_path = entry.path().string();
        std::string relative_path =
            std::filesystem::relative(entry.path(), dirname).string();
#ifdef Debug
        fmt::print("entry : {}\nrelative: {}", entry_path, relative_path);
#endif
        if (entry.is_directory()) {
            // Add directory entry to the zip
            std::string dir_name_in_zip =
                relative_path + "/";  // Ensure it ends with a slash
            if (zip_dir_add(zip, dir_name_in_zip.c_str(), ZIP_FL_OVERWRITE) <
                0) {
                ERR::exitErr(fmt::format("Failed to add directory to zip: {}",
                                         zip_strerror(zip)));
            }
        } else if (entry.is_regular_file()) {
            // Add file to the zip
            addFile(zip, entry_path.c_str(), relative_path.c_str());
        }
    }
}

inline void add(zip_t *zip, const char *name, ...) {
    if (std::filesystem::is_directory(name)) {
        addDirectory(zip, name, null);
    } else {
        addFile(zip, name, null);
    }
}

inline bool startsWith(const std::string &str, const std::string &prefix) {
    return str.compare(0, prefix.size(), prefix) == 0;
}

inline bool deleteFile(zip_t *z, const char *filename) {
    zip_int64_t num_entries = zip_get_num_entries(z, 0);
    if (num_entries == -1) {
        return false;
    }

    std::string target(filename);
    // removes ./  to help add or remove dirs
    if (target.starts_with("./")) {
        target.erase(0, 2);
    }

    if (!target.empty() && target.back() != '/') {
        target += '/';  // Ensure directory ends with '/'
    }

    std::vector<int> indices_to_delete;

    // Collect all entries to delete (files and sub-files if directory)
    for (zip_int64_t i = 0; i < num_entries; ++i) {
        const char *entry_name = zip_get_name(z, i, 0);
        if (!entry_name) {
            continue;
        }

        std::string entry(entry_name);
        if (entry == filename || startsWith(entry, target)) {
            indices_to_delete.push_back(i);
        }
    }

    if (indices_to_delete.empty()) {
        return false;  // No matching file or directory found
    }

    // Delete entries in reverse order to avoid invalidating indices
    for (auto it = indices_to_delete.rbegin(); it != indices_to_delete.rend();
         ++it) {
        if (zip_delete(z, *it) != 0) {
            return false;  // Deletion failed
        }
    }

    return true;
}

inline bool _deleteFile(zip_t *z, const char *filename, ...) {
    zip_entries_t entries = getEntries(z);
    int index = -1;
    for (const auto &i : entries) {
        if (i.second == filename) {
            index = i.first;
            break;
        }
    }
    if (index == -1) {
        return false;
#ifdef debug
        fmt::print(stderr, "file {} not found in {}", name, zipname);
#endif
    }

    zip_delete(z, index);
    return true;
};

// if modified
inline time_t getFileModifiedTime(zip_t *zip, const char *filename, ...) {
    if (!fileExists(zip, filename)) {
        return (time_t)0;
    }
    zip_stat_t *stat = new zip_stat_t;
    if (zip_stat(zip, filename, 0, stat) == -1) {
        return (time_t)0;
    }
    return (time_t)stat->mtime;
}

inline void printChangedFiles(const Makezip::alltargets &tempT, ...) {
    const Makezip::alltargets T = orderTargets(tempT);
    for (const auto &target : T) {
        zip_t *z = init(target.targetName.c_str(), ZIP_CREATE);
        std::string INS = target.ops == Makezip::INS::INS_ADD
                              ? fmt::format("{}{}+a{}", bold, green, nocol)
                              : fmt::format("{}{}-d{}", bold, red, nocol);
        fmt::print("{}[archive]{} : {} {}\n", bold, nocol, target.targetName,
                   INS);
        for (const auto &files : target.targets) {
            struct stat filestat;
            stat(files.c_str(), &filestat);
            time_t zipFilemtime = getFileModifiedTime(z, files.c_str());
            const double TimeDiff = difftime(filestat.st_mtime, zipFilemtime);

            if (TimeDiff <= 5) {
                fmt::print("\t{}{}*{} {} {}[no changes]{}\n", bold, yellow,
                           nocol, files, bold, nocol);
            } else {
                if (target.ops == Makezip::INS_ADD) {
                    fmt::print("\t{}{}A{} {}\n", bold, green, nocol, files);
                } else if (target.ops == Makezip::INS_DEL) {
                    fmt::print("\t{}{}D{} {}\n", bold, red, nocol, files);
                }
            }
        }
    }
}

// file checking is done here before any argument is passed into functions
inline void makezip(const Makezip::alltargets &tempT, ...) {
    if (tempT.size() <= 0) {
        exit(2);
    }
    const Makezip::alltargets T = orderTargets(tempT);

    for (const auto &target : T) {
        zip_t *z = init(target.targetName.c_str(), ZIP_CREATE);

        //  add a '-D' or '+A' to show if archive should be deleteing or adding
        std::string INS = target.ops == Makezip::INS::INS_ADD
                              ? fmt::format("{}{}+a{}", bold, green, nocol)
                              : fmt::format("{}{}-d{}", bold, red, nocol);
        fmt::print("{}[archive]{} : {} {}\n", bold, nocol, target.targetName,
                   INS);

        for (const auto &files : target.targets) {
            if (target.ops == Makezip::INS::INS_ADD) {  // adding file
                if (!std::filesystem::exists(fmt::format("{}", files))) {
                    ERR::exitErr(fmt::format("{} not found\n", files), 2);
                };
                // check modified time before adding file
                struct stat filestat;
                stat(files.c_str(), &filestat);

                time_t zipFilemtime = getFileModifiedTime(z, files.c_str());
                const double TimeDiff =
                    difftime(filestat.st_mtime, zipFilemtime);

#ifdef Debug
                fmt::print("{} -> {} secs\n", files, TimeDiff);
#endif
                if (TimeDiff <= 5) {
                    fmt::print("\t{}{}*{} {} {}[no changes]{}\n", bold, yellow,
                               nocol, files, bold, nocol);
                } else {
                    add(z, files.c_str());
                    fmt::print("\t{}{}A{} {}\n", bold, green, nocol, files);
                }

            } else if (target.ops == Makezip::INS::INS_DEL) {  // deleteing file
                // check if zip to delete from exists
                if (!std::filesystem::exists(
                        fmt::format("{}", target.targetName))) {
                    ERR::exitErr(
                        fmt::format("zip {} not found\nCannot delete from zip",
                                    target.targetName),
                        2);
                };
                // checking if file is in zip before delete
                if (deleteFile(z, files.c_str())) {
                    fmt::print("\t{}{}D{} {}\n", bold, red, nocol, files);
                } else {
                    fmt::print("\t{}{}??{} {} {}[not found]{}\n", bold, yellow,
                               nocol, files, bold, nocol);
                }
            }
        }
        fmt::print("\n");
        zip_close(z);
    }
}

}  // namespace zzip
