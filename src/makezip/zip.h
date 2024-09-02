// this is a project to make a zip based on make with the command makezip
#ifndef __MAIN_ZIP_H
#define __MAIN_ZIP_H

#include <zip.h>

/* boost for program options */
#include <boost/program_options.hpp>
// #include <boost/algorithm/string.hpp>
#include <boost/program_options/errors.hpp>
#include <boost/program_options/value_semantic.hpp>
// #include <boost/property_tree/json_parser.hpp>

/* c++ libs */
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
#include <zip.h>

#include <ctime>
#include <filesystem>

#include <map>

#include <string>
#include <type_traits>
#include <utility>

#include "Makezip.h"
#include "defines.h"

using zip_entries_t = std::map<int, std::string>;

/*================================================*/
namespace zzip {

zip_t *init(const char *path, int flags = ZIP_RDONLY);

/* info on zip file */
void *readBytes(zip_t *zip, const char *filename, int size, uint64_t readsize,
                ...);
bool fileExists(zip_t *zip, const char *filename, ...);
zip_entries_t getEntries(zip_t *zip, ...);
void printZipInfo(zip_t *zip, ...);
void printFiles(zip_t *zip, ...);
void printEntries(zip_t *zip, ...);
time_t getFileModifiedTime(zip_t *zip, const char *filename, ...);

/* modify zip */
void addFile(zip_t *zip, const char *filename, zip_entries_t &zip_entries, ...);
void addFile(zip_t *zip, const char *filename, ...);
void addDirectory(zip_t *zip, const char *dirname, ...);
bool deleteFile(zip_t *z, const char *name, ...);
// this will add dir or file
void add(zip_t *zip, const char *name, ...);
void makezip(const alltargets &T, ...);
}  // namespace zzip
/*================================================*/
#endif
