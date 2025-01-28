#include "errormessage.hpp"
// defines
#include "defines.h"

namespace Makezip {

#if __cplusplus <= 202002L
namespace std {
inline void print(std::string str, ...) { std::cout << str; };
inline void print(FILE*, std::string str, ...) { fprintf(stderr, str); };
namespace filesystem {
bool exists(std::string file) { return true; };
}  // namespace filesystem
}  // namespace std
#endif

using fmt::print;
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

/*
 * alltargets alias to std::vector of targets
 */
using alltargets = std::vector<target>;
//
// void syntaxerror(int line, std::string& err, std::string&& extrainfo);
// void printallTargets(alltargets& T);
// void findMakezip();
// std::stringstream* getMakeZipContents(char* buffer, ...);
// const alltargets orderTargets(const alltargets& T, ...);
// void grabTargetsfiles(std::string& line, std::stringstream* buffer,
//                       int& currentlinenumber, std::string& errline, target&
//                       t);
// void removeWhiteSpaceAround(std::string& str);
// void* parseMakezip(std::stringstream* buffer, alltargets& T);
//
/*
 * output each target and it targets
 */

inline void printallTargets(alltargets& T) {
    for (const auto target : T) {
        fmt::print("{}[Target]{} : {}\n", green, nocol, target.targetName);
        if (target.targets.size() == 0) {
            fmt::print(stderr, "\t* {}No targets{}\n", red, nocol);
            return;
        } else {
            for (const auto i : target.targets) {
                fmt::print("\t-> {}\n", i);
            }
        }
    }
};

/*
 * look for a Makezip file, exit if not found
 */
inline void findMakezip() {
    if (!std::filesystem::exists("./Makezip")) {
        ERR::exitErr("Makezip file not found\n", -1);
    }
}

/*
 * returns a stringstream * to the file's buffer
 */
inline std::stringstream* getMakeZipContents(char* buffer, ...) {
    findMakezip();
    std::fstream file =
        std::fstream("./Makezip", std::ios::in | std::ios::out | std::ios::app);

    if (!file.is_open()) {
        ERR::exitErr("Could not open Makezip file in current dir\n");
        exit(-1);
    }

    std::stringstream* ssbuffer = new std::stringstream;
    *ssbuffer << file.rdbuf();
    if (ssbuffer->str().size() <= 0) {
        ERR::exitErr("Makezip is Empty\n", 0);
    }
    return ssbuffer;
}

/*
 * remove White space from all around string str
 */
inline void removeWhiteSpaceAround(std::string& str) {
    while (str.ends_with(' ')) {
        str.pop_back();
    }
    while (str.starts_with(' ')) {
        str.erase(0, 1);
    }
}

/*
 * grab Target files from Target Header
 * + updates currentlinenumber
 * + error handling
 * + syntax error handling
 */
inline void grabTargetsfiles(std::string& line, std::stringstream* buffer,
                             int& currentlinenumber, std::string& errline,
                             target& t) {
    while (std::getline(*buffer, line)) {
        if (line.size() <= 0) break;

        /* looking for syntax errors*/
        if (line.find(':') != std::string::npos) {
            ERR::syntaxerror(currentlinenumber, errline, "");
        }

        // remove trailing whitespace and front ones
        removeWhiteSpaceAround(line);
        if ((line.size() != 0)) {
            t.targets.push_back(line);
        }

        currentlinenumber++;
    }
}

inline alltargets orderTargets(const alltargets& T, ...) { return T; }

/*
 * parses all the targets into a vector of targets
 * @rules
 * + whitespace = new target
 * + a target header must end with a :
 * + targets can have operations i.e add delete
 * + no target header should have no empty targets
 */
inline void* parseMakezip(std::stringstream* buffer, alltargets& T) {
    std::string line;
    std::string errline;
    int currentlinenumber{0};  // holds line number for syntax error helps
                               // grab targetsfiles

    while (std::getline(*buffer, line)) {
        if (line.size() <= 0) {  // if nothing
            continue;
        } else {
            errline = line;                // make copy for errors
            removeWhiteSpaceAround(line);  // remove trailing whitespace
            if (line.size() <= 0 || line == " ") {
#ifdef Debug
                fmt::print("line is empty");
#endif
                continue;
            }
            if (line.starts_with('#')) {
                continue;
            }
            target t;
            /*====================================================
             * parsing
            ====================================================*/
            if (line.at(line.size() - 1) == ':') {
                /*+++++++++++++++++++++++++++++
                 * if  all:
                +++++++++++++++++++++++++++++*/
                line.pop_back();  // remove ':'
                removeWhiteSpaceAround(line);
                t.targetName = line;
                currentlinenumber++;
                line.erase();
                grabTargetsfiles(line, buffer, currentlinenumber, errline, t);
                t.ops = INS_ADD;
            } else if (line.find(':') != std::string::npos) {
                /*+++++++++++++++++++++++++++++
                 * if  all:add || all: add
                +++++++++++++++++++++++++++++*/
                int column = line.find(':');
                string _target = line.substr(0, column);
                removeWhiteSpaceAround(_target);
                t.targetName = _target;
                std::string config;
                column++;

                // checking if the config has extra tokens (only one needed )
                config = line.substr(column);
                removeWhiteSpaceAround(config);

                if (config.find(' ') != std::string::npos) {
                    ERR::syntaxerror(currentlinenumber, errline,
                                     fmt::format("Invalid config for target "
                                                 "Header {}{}{}",
                                                 green, t.targetName, nocol));
                }

#ifdef Debug
                p("target -> {}\nconfig -> {}\n", t.targetName, config);
                fgetc(stdin);
#endif

                // check config
                if ((config == "add") || (config == "a")) {
                    t.ops = INS_ADD;
                } else if ((config == "delete") || (config == "d") ||
                           (config == "del")) {
                    t.ops = INS_DEL;
                } else {
                    ERR::syntaxerror(
                        currentlinenumber, errline,
                        "config in target Header can not be identified");
                }
#ifdef Debug
                fmt::print(
                    "target -> {}\n"
                    "config -> {}\n"
                    "INS -> {}\n",
                    t.targetName, config, (int)t.ops);
#endif

                grabTargetsfiles(line, buffer, currentlinenumber, errline, t);
            } else {
                ERR::syntaxerror(
                    currentlinenumber, errline,
                    "target Header and targets cannnot be distinguished");
            }
            T.push_back(t);
        }
        currentlinenumber++;
    }
    return &T;
};

}  // namespace Makezip
