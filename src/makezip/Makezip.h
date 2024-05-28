#ifndef __MAKEFILE_ZIP_H__
#define __MAKEFILE_ZIP_H__
#include <boost/algorithm/string.hpp>
#include <boost/program_options/parsers.hpp>
#include <filesystem>
#include <format>
#include <fstream>
#include <print>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

#include "defines.h"

using std::print;
using std::string;

/*
 *  hold possible instruction for target
 */
enum INS { INS_DEL = 0, INS_ADD = 1 };

/*
 *  hold targetName and all targets below it
 *  + by default all target is INS_ADD
 */
struct target {
    std::string targetName;
    std::vector<std::string> targets;
    INS ops = INS_ADD;
};

// alltargets alias to std::vector of targets
using alltargets = std::vector<target>;

void syntaxerror(int line, std::string& err, std::string&& extrainfo);
void printallTargets(alltargets& T);
void findMakezip();
std::stringstream* getMakeZipContents(char* buffer, ...);
void grabTargetsfiles(std::string& line, std::stringstream* buffer,
                      int& currentlinenumber, std::string& errline, target& t);
void removeWhiteSpaceAround(std::string& str);
void* parseMakezip(std::stringstream* buffer, alltargets& T);

#endif
