#include <pthread.h>
#include <unistd.h>

#include <cctype>
#include <cerrno>
#include <cstdarg>
#include <cstdio>
#include <cstring>

// === c++ === //
#include <assert.h>

#include <algorithm>
#include <cctype>
#include <filesystem>
#include <format>
#include <fstream>
#include <print>
#include <sstream>
#include <string>

using std::string, std::stringstream;

/* [ name ]: makehere
 * [ purpose ] : create a simple c/c++ project [ status ] : complete
 * [ sample ] : makehere -p project-name -l [cpp/c] -C [make/cmake] -std
 * [11/14/17/20/2a/2x] -f <filename>
 */

// engine will take the argc and argv and parse them.
// parse => check flags and call the respective functions

// check for a short option eg --help => -h

struct Engine {
   private:
    char **Args;
    int Argc;
    bool Templates{true};
    string language;
    string filename;
    string project;
    string compiler;
    string standard;

   public:
    Engine(char **argv, int argc) : Args(argv), Argc(argc) {};

    void ERR(string str, int status = 1) {
        std::print(stderr, "{} : [ERR] {}\n", program_invocation_name, str);
        exit(status);
    }  // ERR

   private:
    // check if needle is found in haystack
    // needle(0,2) is found in haystack(0..2) if checkShort if enabled
    bool isOptionFound(std::string haystack, std::string needle,
                       bool checkShort = true) {
        if (haystack == needle) return true;

        if (checkShort)
            if (haystack == needle.substr(1, 2)) return true;

        return false;
    };

    void createFile(std::string File, std::string fileContents) {
        std::ofstream fileStream(std::format("./{}/{}", project, File));
        fileStream << fileContents;
        fileStream.close();
    }

    void lowerCase(std::string &data) {
        if (data.size() == 0) return;
        std::transform(data.begin(), data.end(), data.begin(),
                       [](unsigned char c) { return std::tolower(c); });
    }

    // checks if the str has a '-' in it
    void check_next(std::string str, std::string err_message) {
        if (str.size() != 0) {
            if (str[0] != '-') {
                return;
            }
        }
        ERR(err_message);

    }  // check_next

    void buildClangd() {
        std::string str = std::format(
            "CompileFlags:\n"
            "\tAdd: [ -std={}{} ]\n",
            (language == "c" ? "c" : "c++"), standard);
        createFile(".clangd", str);
    };  // Buildclangd

    void buildMake() {
        // if template exists
        if (std::filesystem::exists("~/.Makefile") && Templates) {
            std::filesystem::copy("~/.Makefile",
                                  std::format("./{}/Makefile", project));
            return;
        }

        std::string makestr{
            std::format("CC={}\n"
                        "FILE={}\n"
                        "STD=-std={}{}\n"
                        "FLAGS=-g --all-warnings\n"
                        "main:main.o\n"
                        "\t$(CC) main.o ${{STD}} ${{FLAGS}} -o main\n\n"
                        "main.o: ${{FILE}}\n"
                        "\t$(CC) -c ${{FILE}} ${{STD}} ${{FLAGS}} -o main.o\n\n"
                        "run: main\n"
                        "\t@echo '========================================'\n"
                        "\t@./main\n\n"
                        "clean:\n"
                        "\t@touch main.o main\n"
                        "\t@rm main.o main\n"
                        "\t@echo cleaning done",
                        (language == "c" ? "gcc" : "g++"), filename,
                        (language == "c" ? "c" : "c++"), standard)};

        createFile("Makefile", makestr);
    };  // buildMake

    void buildCmake() {
        std::string cmake_standard{};
        if (standard == "2x" || standard == "2a") {
            cmake_standard = "23";
        }

        std::string cmakestr{std::format(
            "cmake_minimum_required(VERSION 3.5)\n"
            "project({0})\n"
            "add_executable(${{PROJECT_NAME}} {1})\n"
            "set_target_properties(${{PROJECT_NAME}} PROPERTIES {2}_STANDARD "
            "{3})\n"
            "set(CMAKE_{2}_STANDARD_REQUIRED true)\n"
            "#find_package(std)\n"
            "#target_link_libraries(${{PROJECT_NAME}} PRIVATE "
            "std::std-header-only -static)\n"
            "set(EXECUTABLE_OUTPUT_PATH ${{CMAKE_SOURCE_DIR}}/bin)\n",
            project, filename, (language == "c" ? "C" : "CXX"),
            cmake_standard)};

        std::string make_for_cmake = std::format(
            "CC=cmake\n"
            "FILE={}\n"
            "main: build\n"
            "\t$(CC) --build build/\n\n"
            "build: ${{FILE}}\n"
            "\t$(CC) -S . -B build/\n\n"
            "run: main\n"
            "\t@echo \"=========================================\"\n"
            "\t@./bin/{}\n\n"
            "clean:\n"
            "\t@rm -rf bin build 2>/dev/null\n",
            filename, project);

        if (Templates) {
            if (std::filesystem::exists("~/.CMakeLists.txt-c") &&
                language == "C") {
                std::filesystem::copy(
                    "~/.CMakeLists.txt-c",
                    std::format("./{}/CMakeLists.txt", project));
            } else if (std::filesystem::exists("~/.CMakeLists.txt-cpp") &&
                       language == "CXX") {
                std::filesystem::copy(
                    "~/.CMakeLists.txt-c",
                    std::format("./{}/CMakeLists.txt", project));
            } else {
                createFile("CMakeLists.txt", cmakestr);
            }
        } else {
            createFile("CMakeLists.txt", cmakestr);
        }
        createFile("Makefile", make_for_cmake);
    }  // Cmake

    void createProjectDir() {
        if (std::filesystem::exists(std::format("./{}", project))) {
            ERR("project name already exists at current directory\n", 1);
        }
        std::filesystem::create_directory(project);
    }

   public:
    // parser -l -c -h -std -f -s
    void parse() {
        switch (Argc) {
            case 2:
                if (isOptionFound(string(Args[1]), "--help")) {
                    std::print(
                        "{} : create a simple project startup \n"
                        "-h    --help                       output this "
                        "message\n"
                        "-l    --language   [c,cpp]         language of the "
                        "project*\n"
                        "-p    --project     name           name of the "
                        "project*\n"
                        "-c    --complier   [cmake,make]    compiler Script  ( "
                        "default: "
                        "make )\n"
                        "-f    --filename   [filename]      custom file name ( "
                        "default: "
                        "main.[c,cpp] )\n"
                        "-std  --standard   [standard]      set a standard   ( "
                        "default: "
                        "-std=c2- )\n"
                        "-t   --template                        copy template "
                        "files\n"
                        "\n"
                        "NB: -l and -p is compulsory\n"
                        "default files or \"template\" files will be looked "
                        "for to be copied\n"
                        "Makefile       :   ~/.Makefile\n"
                        "CMakeLists.txt :   ~/.CMakeLists.txt\n"
                        "cpp files      :   ~/.template.cpp\n"
                        "c files        :   ~/.template.c\n"
                        "\n"
                        "Use --template / -t to use template files\n"
                        "But you will have to edit them mannually\n",
                        program_invocation_name);

                    exit(0);
                } else {
                    ERR("Invalid arguments passed", 1);
                }
                break;
            default:
                int i{1};
                while (i < Argc) {
                    if (isOptionFound(Args[i], "--language")) {
                        ++i;
                        check_next(Args[i],
                                   "-l / --language needs an argument");
                        language = string(Args[i]);
                        lowerCase(language);
                        if ((language != "c") && (language != "cpp") &&
                            (language != "cxx") && (language != "c++")) {
                            ERR("only c or cpp allowed in "
                                "-l/--language",
                                1);
                        }
                    } else if (isOptionFound(Args[i], "--project")) {
                        ++i;
                        check_next(Args[i], "-p / --project needs an argument");
                        project = string(Args[i]);
                    } else if (isOptionFound(Args[i], "--filename")) {
                        ++i;
                        check_next(Args[i],
                                   "-f / --filename needs an argument");
                        filename = string(Args[i]);
                    } else if (isOptionFound(Args[i], "--compiler")) {
                        ++i;
                        check_next(Args[i],
                                   "-c / --compiler needs an argument");
                        compiler = string(Args[i]);
                        lowerCase(compiler);
                    } else if (isOptionFound(Args[i], "--standard", false) ||
                               isOptionFound(Args[i], "-std", false)) {
                        ++i;
                        check_next(Args[i],
                                   "--standard / -std needs an argument");
                        standard = string(Args[i]);
                    } else if (isOptionFound(Args[i], "--template")) {
                        Templates = false;
                    }
                    i++;
                };
                break;
        };

        // ==== ====== ====== checks ====== ====== =====//

        // check if language is set
        if (language.size() == 0) {
            ERR("-l is compulsory , needed to determine the compiler script "
                "and the "
                "filename",
                1);
        }

        // check if project is set
        if (project.size() == 0) {
            ERR("-p / --project is compulsory, needed to created directory", 1);
        }

        // checking standard
        if (standard != "11" || standard != "14" || standard != "17" ||
            standard != "20" || standard != "99" || standard != "23") {
            if (language == "c") {
                standard = "2x";
            } else {
                standard = "2a";
            }
        }

        if (standard == "23") {
            if (language == "c") {
                standard = "2x";
            } else {
                standard = "2a";
            }
        }

        // cheking filename
        if (filename.size() == 0) {
            if (language == "c") {
                filename = "main.c";
            } else {
                filename = "main.cpp";
            }
        } else {
            if (language == "c") {
                if (filename.find(".c") == std::string::npos) {
                    filename += ".c";
                }
            } else {
                if (filename.find(".cpp") == std::string::npos &&
                    filename.find(".cxx") == std::string::npos &&
                    filename.find(".c++") == std::string::npos) {
                    filename += ".cpp";
                }
            }
        }

        // ==== ====== ====== Done ====== ====== =====//

        // creating the project dir
        createProjectDir();

        // create file
        if (language == "c")  //  check for template file for c
        {
            if (std::filesystem::exists("~/.template.c") && Templates) {
                std::filesystem::copy(
                    "~/.template.c", std::format("./{}/{}", project, filename));
            } else {
                createFile(filename,
                           std::format("/*\n"
                                       "\tproject: {} \n"
                                       "\tauthor: \n"
                                       "*/\n"
                                       "#include <stdio.h>\n"
                                       "int main(int argc,char ** argv){{\n"
                                       "printf(\"project : {}\\n\");\n"
                                       "return 0;\n}}\n",
                                       project, project));
            }
        } else  //  check for template file for cpp
        {
            if (std::filesystem::exists("~/.template.cpp") && Templates) {
                std::filesystem::copy(
                    "~/.template.cpp",
                    std::format("./{}/{}", project, filename));
            } else {
                createFile(filename,
                           std::format("/*\n"
                                       "\tproject: {}\n"
                                       "\tauthor: \n"
                                       "*/\n"
                                       "#include <iostream>\n"
                                       "int main(int argc,char ** argv){{\n"
                                       "std::cout << \"project : {}\\n\";\n"
                                       "return 0;\n}}\n",
                                       project, project));
            }
        }

        // creating the compiler script
        if (compiler.size() == 0) {
            compiler = "make";
        }

        if (compiler == "make") {
            buildMake();
        } else if (compiler == "cmake") {
            buildCmake();
        }

        // setting up clangd
        buildClangd();

        std::print(
            "Created a project:{}\n"
            "language        : {}\n"
            "filename        : {}\n"
            "compiler script : {}\n"
            "standard        : {}\n",
            project, language, filename, compiler, standard);
        exit(0);
    }  // void Parse
};  // Engine

int main(int argc, char **argv) {
    Engine e(argv, argc);
    if (argc == 1) {
        e.ERR("no arguments passed\ntry -h for more info\n", 1);
    }
    e.parse();
    return 0;
}
