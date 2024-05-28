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
#include <filesystem>
#include <format>
#include <map>
#include <print>
#include <string>
#include <utility>

#include "Makezip.h"
#include "defines.h"

using zip_entries_t = std::map<int, std::string>;

/*================================================*/

zip_t *init_zip(const char *path, int flags = ZIP_RDONLY);

/* info on zip file */
void *readbytesfromzip(zip_t *zip, const char *filename, int size,
                       uint64_t readsize, ...);
std::map<int, std::string> getfilesandindexinzip(zip_t *zip, ...);
void printZipInfo(zip_t *zip, ...);
void printZipFiles(zip_t *zip, ...);
void printZip(zip_t *zip, ...);
time_t getfileModifiedTime(zip_t *zip, const char *filename, ...);

/* modify zip */
void addFileToZip(zip_t *zip, const char *filename, zip_entries_t &zip_entries,
                  ...);
void addFiletoZip(zip_t *zip, const char *filename, ...);
bool deleteFilefromZip(const char *name, ...);
void makezip(const alltargets &T, zip_entries_t &zip_entries);
/*================================================*/
#endif
