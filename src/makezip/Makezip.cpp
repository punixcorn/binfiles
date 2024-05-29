#include "Makezip.h"

#include "errormessage.h"

/*
 * output each target and it targets
 */
void printallTargets(alltargets& T) {
    for (const auto target : T) {
        std::print("{}[Target]{} : {}\n", green, nocol, target.targetName);
        if (target.targets.size() == 0) {
            std::print(stderr, "\t* {}No targets{}\n", red, nocol);
            return;
        } else {
            for (const auto i : target.targets) {
                std::print("\t-> {}\n", i);
            }
        }
        std::print("\n");
    }
};

/*
 * look for a Makezip file, exit if not found
 */
void findMakezip() {
    if (!std::filesystem::exists("./Makezip")) {
        ERR::exitErr("Makezip file not found\n", -1);
    }
}

/*
 * returns a stringstream * to the file's buffer
 */
std::stringstream* getMakeZipContents(char* buffer, ...) {
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
void removeWhiteSpaceAround(std::string& str) {
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
void grabTargetsfiles(std::string& line, std::stringstream* buffer,
                      int& currentlinenumber, std::string& errline, target& t) {
    while (std::getline(*buffer, line)) {
        if (line.size() <= 0) break;

        /* looking for syntax errors*/
        if (line.find(':') != std::string::npos) {
            ERR::syntaxerror(currentlinenumber, errline);
        }

        // remove trailing whitespace and front ones
        removeWhiteSpaceAround(line);
        if ((line.size() != 0)) {
            t.targets.push_back(line);
        }

        currentlinenumber++;
    }
}

/*
 * parses all the targets into a vector of targets
 * @rules
 * + whitespace = new target
 * + a target header must end with a :
 * + no target header should have no empty targets
 */
void* parseMakezip(std::stringstream* buffer, alltargets& T) {
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
                /* p("line is empty"); */
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
                t.targetName = line;
                currentlinenumber++;

                /* grab targetsfiles
                while (std::getline(*buffer, line)) {
                    if (line.size() <= 0) break;

                    /* looking for syntax errors
                    if (line.at(line.size() - 1) == ':' ||
                        line.find(':') != std::string::npos) {
                        syntaxerror(currentlinenumber, errline);
                    }
                    // remove trailing whitespace and front ones
                    removeWhiteSpaceAround(line);
                    t.targets.push_back(line);
                    currentlinenumber++;
                }
                */

                grabTargetsfiles(line, buffer, currentlinenumber, errline, t);
                t.ops = INS_ADD;
            } else if (line.find(':') != std::string::npos) {
                /*+++++++++++++++++++++++++++++
                 * if  all:add || all: add
                +++++++++++++++++++++++++++++*/
                int column = line.find(':');
                t.targetName = line.substr(0, column);
                std::string config;
                column++;

                // checking if the config has extra tokens (only one needed )
                config = line.substr(column);
                removeWhiteSpaceAround(config);

                if (config.find(' ') != std::string::npos) {
                    ERR::syntaxerror(currentlinenumber, errline,
                                     std::format("Invalid config for target "
                                                 "Header {}{}{}",
                                                 green, t.targetName, nocol));
                }

                /*
                p("target -> {}\nconfig -> {}\n", t.targetName, config);
                fgetc(stdin);
                */

                // check config
                if ((config == "add") || (config == "a")) {
                    t.ops = INS_ADD;
                } else if ((config == "delete") || (config == "d")) {
                    t.ops = INS_DEL;
                } else {
                    ERR::syntaxerror(
                        currentlinenumber, errline,
                        "config in target Header can not be identified");
                }

                /*
                p("target -> {}\n"
                  "config -> {}\n"
                  "INS -> {}\n",
                  t.targetName, config, (int)t.ops);
                */

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

/*
    while (std::getline(*buffer, line)) {
        if (line.size() <= 0) {  // if nothing
            continue;
        } else if (line.at(line.size() - 1) == ':') {  // if all:
            line.pop_back();                           // remove :
            target t;
            t.targetName = line;
            currentline++;

            // grab targetsfiles
            while (std::getline(*buffer, line)) {
                if (line.size() <= 0) break;

                /* looking for syntax errors
                {
                    if (line.at(line.size() - 1) == ':') {
                        syntaxerror(currentline, line);
                    }

                    if (line.size()) {
                    }
                }
                t.targets.push_back(line);
                currentline++;
            }
            T.push_back(t);
        } else if (line.find(':') != line.at(line.size() - 1)) {
            syntaxerror(currentline, line,
                        "target Header and targets cannnot be distinguished");
        } else {
            syntaxerror(currentline, line, "unidentified token");
        }
        currentline++;
    }
 */
