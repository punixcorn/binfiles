/*
Gitalias @ 2023 punixcorn
Gitalias a git alias
*/

/* new todo:
 * - push only one branch instead of all : git push -u origin branch [done]
 * - fix grabbing files from curl []
 * - move to libgit [done another file, not recommended]
 * - suggestive autocompelete [done]
 * - remove --git ( no apparent use, this tool solves the DRY use of git not
 * replace it ) [done]
 * - use default username when cloneing a repo [done]
 * - make use of full c++ , remove more C stuff [done-ish]
 */

/* boost includes */
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/case_conv.hpp>
#include <boost/program_options.hpp>
#include <boost/program_options/errors.hpp>
#include <boost/program_options/value_semantic.hpp>
#include <boost/property_tree/json_parser.hpp>

/* C++ includes */
#include <cctype>
#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <new>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>
/* C includes */
#include <alloca.h>
#include <dirent.h>
#include <errno.h>
// #include <fmt/base.h>
#include <fmt/core.h>
#include <fmt/format.h>
#include <fmt/std.h>
#include <unistd.h>

#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>

/* defines */
#ifndef null
#define null NULL
#endif
#define arg(cmd, type) args[cmd].as<type>()
#define red "\e[42m"
#define green "\e[43m"
#define blue "\e[45m"
#define rs "\e[0m"

namespace opt = boost::program_options;
using std::string, std::vector, std::string_view, std::cout, std::cerr,
    std::nothrow, std::endl, std::cin, std::ifstream;

/* Hold username and token */
struct UserInfo {
    std::string token;
    std::string username;
    std::string defaultMessage;
};

/* handle application error */
struct Error_occured {
    // make this a vector -> handles more error messages
   private:
    std::string message;

   public:
    Error_occured() : error_occured(false), message("") {}
    bool error_occured;

    void trip(std::string &&text) {
        error_occured = true;
        message = text;
    }

    void trip(bool state, std::string &&text) {
        error_occured = state;
        message = text;
    }

    std::string what() {
        if (error_occured && message.size() == 0) {
            return std::format("Unknown, Error occured in the application\n");
        }
        return std::format("Error Occured in the Application:\n{}", message);
    }
};

/* global varibales -> helps prevents extreme function arguments*/
struct Globals {
    Globals() {};
    ~Globals() {};
    string messagebox, addbox, reponame, repodes, subcommand, Resetcommand,
        command = "[ -f /bin/git ] ";
    bool mode; /* user request mode */
    vector<string>(Sreset);
    Error_occured App_error;
};

struct Trips {
    Trips() { memset(this, 0, sizeof(*this)); };
    ~Trips() { delete this; };
    bool commit, message, add, init, branch, switch_, deleteBranch, merge, pull,
        push, origin, log, status, repoName, repoDes, repoMode, verbose, Rreset,
        git;
};

/* ============== function declarations ================*/
/* append into subcommand*/
void Isubcommand(Globals *g, const string_view &s1, const string_view &s2);

/* prints error message e with help message and kills program */
auto exitWithHelp(const string_view &e, int return_val = 1) -> void;

/* prints error message e and kills the program */
auto exitWithoutHelp(const string_view &e, int return_val = 1) -> void;

/* checks if you have a git repo init */
auto getGitInfo() -> bool;

/* check for staged files */
auto checkStaged() -> bool;

/* check is thing exists at place */
[[nodiscard("if file exists in place")]] auto findFile(const string &place,
                                                       const string_view &file)
    -> bool;

/* create an online repository */
auto createOnlineRepo(Globals *g) -> void;

/* runs the command */
auto gitalias_main(Globals *g, bool v) -> void;

/* parses the struct  */
auto parse(Globals *g, Trips *t) -> void;

/*finds config file and returns Json vaules about the user*/
[[nodiscard("retrive UserInfo tokens from json file")]] auto ParseUserInfo()
    -> UserInfo;

/* ============== Main ============*/
int main(int argc, char *argv[]) {
    if (argc < 2) {
        exitWithHelp("No arguments passed...");
    }
    Trips *T = new (nothrow) Trips();
    if (T == null) {
        cerr << program_invocation_name << ": Program Crashed...\n"
             << "Err: Trip could not be allocated\n";
        exit(1);
    }
    Globals *G = new (nothrow) Globals();
    if (G == null) {
        cerr << program_invocation_name << ": Program Crashed...\n"
             << "Err: Globals could not be allocated\n";
        exit(1);
    }
    try {
        opt::options_description desc("gitalias options");
        desc.add_options()("help,h", "print this message")(
            "init,i", "init a repository")("commit,c", "add all and commit")(
            "add,a", opt::value<vector<string>>()->multitoken(),
            "add [files] only")("message,m",
                                opt::value<vector<string>>()->multitoken(),
                                "add a message")(
            "branch,b", opt::value<string>()->implicit_value(""),
            "create a branch")("switch,s", opt::value<string>(),
                               "switch to a branch")(
            "delete,d", opt::value<string>(), "delete a branch")(
            "Merge,M", opt::value<string>(),
            "merge branch [ branch-name ] with current branch")(
            "Pull,P", opt::value<string>()->implicit_value(""),
            "pull from origin")("push,p",
                                opt::value<string>()->implicit_value(""),
                                "push into origin")(
            "Clone,C", opt::value<vector<string>>()->multitoken(),
            "clone a repository with given \"user/repo-name\"")(
            "Request,R", opt::value<string>()->default_value("https"),
            "Protocol to use when cloning, [ https/ssh ]")(
            "verbose,v", "print out parsed code")("log,l", "show log files")(
            "Status,S", "show statuts")("origin,o", opt::value<string>(),
                                        "add an origin")(
            "repo,r", opt::value<string>(&G->reponame),
            "name for creating an online repo * [i]")(
            "Des,D", opt::value<string>(&G->repodes),
            "description for the online repo * [ii]")(
            "type,t", opt::value<bool>(&G->mode)->value_name("bool"),
            "if repo should be private * [iii]")(
            "undo,u", opt::value<vector<string>>()->multitoken(),
            "*Reset back to a commit ,eg gitalias -u "
            "[ hard / soft / mixed ] "
            "[ number of commits to reset back ]")(
            "Grab,G", opt::value<string>(),
            "grab a specific folder from a github repo")(
            "Visibility,V", opt::value<vector<string>>()->multitoken(),
            "Modify the status of a repo[private,public]")(
            "Json,J", "List the Json Options");
        // remove --git
        //("git,g", opt::value<vector<string>>()->multitoken(),"append git
        // commands [ gitalias -g \" git ... \" ]")

        opt::variables_map args;
        opt::store(opt::command_line_parser(argc, argv)
                       .options(desc)
                       .style(opt::command_line_style::default_style |
                              opt::command_line_style::allow_sticky)
                       .run(),
                   args);
        opt::notify(args);
        if (args.count("help")) {
            /* appending more info into the string ::
            only needed when theres a command for only shorthands
            stringstream stream;
            stream << desc;
            string helpMsg = stream.str();
            boost::algorithm::replace_all(helpMsg, "--", "-");
            boost::algorithm::replace_all(helpMsg, "[ -", "[ --");
            cout << helpMsg << "\n";
            */
            cout << desc << "\n"
                 << "for more info try\e[1m man gitalias\e[0m\n";
            exit(0);
        }
        /* Check for trips */
        if (args.count("commit")) {
            T->commit = true;
        }

        if (args.count("message")) {
            T->message = true;
            for (vector<string>::const_iterator i =
                     args["message"].as<vector<string>>().begin();
                 i != args["message"].as<vector<string>>().end(); i++) {
                G->messagebox += " -m '";
                G->messagebox += *i;
                G->messagebox += "' ";
            }
        }

        if (args.count("init")) {
            T->init = true;
        }
        if (args.count("add")) {
            T->add = true;
            G->addbox += " && git add ";
            cout << "Files added: ";
            for (auto i : args["add"].as<vector<string>>()) {
                if (i == ".") {
                    cout << "all ";
                } else {
                    cout << i << " ";
                }
                G->addbox += i;
                G->addbox += " ";
            }
            cout << endl;
        }
        if (args.count("branch")) {
            T->branch = true;
            Isubcommand(G, " && git branch ", "");
            if (args["branch"].as<string>().length() > 0) {
                G->subcommand += args["branch"].as<string>();
            }
        }
        if (args.count("switch")) {
            T->switch_ = true;
            Isubcommand(G, " && git switch ", args["switch"].as<string>());
        }
        if (args.count("delete")) {
            T->deleteBranch = true;
            Isubcommand(G, " && git branch -D ", args["delete"].as<string>());
        }
        if (args.count("Merge")) {
            T->merge = true;
            Isubcommand(G, " && git merge ", args["Merge"].as<string>());
        }
        /* work on a script to jump to every branch and pull */
        if (args.count("Pull")) {
            T->pull = true;
            if (args["Pull"].as<string>() == "all") {
                cout << "Pulling all remote branches..\n";
                Isubcommand(G, " && git pull --all ", "");
            } else {
                Isubcommand(G, " && git pull ", args["Pull"].as<string>());
            }
        }
        if (args.count("push")) {
            // will push all if --all is given , will push to -u origin, if a
            // specific branch is given or will just do a normal push in your
            // branch
            T->push = true;

            if (arg("push", string).size() == 0) {
                Isubcommand(G, " && git push ", "");
            }

            if (arg("push", string).size() > 0) {
                if (arg("push", string) == "all") {
                    cout << "Pushing all branches...\n";
                    Isubcommand(G, " && git push --all ", "");
                } else {
                    Isubcommand(G, " && git push -u origin ",
                                arg("push", string));
                }
            }
        }

        if (args.count("Visibility")) {
            if (arg("Visibility", vector<string>).size() != 2) {
                exitWithoutHelp(
                    "1Invalid arguments\nUsage: --Visibility <repoName> "
                    "<private/public>");
            }

            std::string_view viewType = arg("Visibility", vector<string>)[1];
            std::string_view repoName = arg("Visibility", vector<string>)[0];

            if ((viewType != "private") && (viewType != "public")) {
                exitWithoutHelp(
                    "2Invalid arguments\nUsage: --Visibility <repoName> "
                    "<private/public>");
            }

            if (!findFile("/bin", "curl"))
                exitWithoutHelp(
                    "Could not find dependency : curl..."
                    "\nTerminating...\n");

            UserInfo User = ParseUserInfo();
            if (User.username == "_" || User.token == "_") {
                exitWithoutHelp(
                    "Username and Token not set in config "
                    "file:\n~/.config/gitalias/githubuserinfo.json\n");
            }

            std::string Vstr{};
            Vstr = fmt::format(
                "curl -u {}:{} --data \"{{\\\"private\\\": \\\"{}\\\"}}\" "
                "--request PATCH https://api.github.com/repos/{}/{} | tee "
                "/dev/null",
                User.username, User.token,
                viewType == "private" ? "true" : "false", User.username,
                repoName);

            FILE *instance = popen(Vstr.c_str(), "r");
            char buffer[1000];

            while (fgets(buffer, 999, instance) != null) {
                if (strstr(buffer, "Bad credentials") != null) {
                    exitWithoutHelp(
                        "Bad credentials : Please update your token, it may "
                        "have "
                        "expired");
                } else if (strstr(buffer,
                                  "Could not resolve host: api.github.com") !=
                           null) {
                    exitWithoutHelp(
                        "Could not connect to api.github.com\n"
                        "check internet connection");
                }
            }
            fmt::print("{}/{} has been made {} succesfully\n", User.username,
                       arg("Visibility", vector<string>)[0],
                       arg("Visibility", vector<string>)[1]);
        }

        if (args.count("Clone")) {
            // will clone in 3 ways
            // 1. you pass in the whole link
            // 2. you pass in username/repo-name -R ssh or https ( default is
            // https)
            // 3. you pass in repo-name, it will clone with your
            // username/repo-name

            string tempstr{}, dir{};
            tempstr = arg("Clone", vector<string>).front();
            size_t arg_count = arg("Clone", vector<string>).size();
            if (arg_count == 2) {
                dir = arg("Clone", vector<string>).back();
            } else if (arg_count > 2) {
                exitWithoutHelp(
                    "--Clone / -C takes in only 2 args max, The folder to "
                    "clone "
                    "and the dir to clone into");
            }

            // finding github url in the string
            if (tempstr.find("https://github.com") != string::npos ||
                tempstr.find("git@github") != string::npos) {
                tempstr += " ";
                tempstr += dir;
                tempstr += " ";
                Isubcommand(G, " && git clone ", tempstr);
            } else {
                // didn't find github link
                string clonestr{"&& git clone "};

                // didnt find username/reponame so fixing it
                if (tempstr.find("/") == std::string::npos) {
                    UserInfo currentuser = ParseUserInfo();
                    if (currentuser.username == "_") {
                        G->App_error.trip(
                            "Username Unknown, populate "
                            "~/.config/gitalias/githubuserinfo.json with your "
                            "username\n"
                            "A file should have already been created for you");
                    }
                    // append username to the front
                    tempstr.insert(0, "/");
                    tempstr.insert(0, currentuser.username);
                }

                // found username/reponame
                if (arg("Request", string) == "ssh") {
                    cout << "Cloning Protocol : ssh\n";
                    clonestr += "git@github.com:";
                    clonestr += tempstr;
                    clonestr += ".git ";
                } else {
                    cout << "Cloning Protocol: https\n";
                    clonestr += "https://github.com/";
                    clonestr += tempstr;
                    clonestr += " ";
                }
                clonestr += dir;
                Isubcommand(G, clonestr, " ");
            }
        }
        if (args.count("verbose")) {
            T->verbose = true;
        }
        if (args.count("log")) {
            T->log = true;
            Isubcommand(G, " && git log ", "");
        }
        if (args.count("Status")) {
            T->status = true;
            Isubcommand(G, " && git status ", "");
        }
        if (args.count("origin")) {
            // work in 2 ways
            // 1. pass in username/repo
            // 2. pass in repo will use default username

            std::string final = arg("origin", string);
            if (final.find("/") == std::string::npos) {
                final.insert(0, "/");
                final.insert(0, ParseUserInfo().username);
            }
            T->origin = true;
            std::string otemp{fmt::format(
                " && git remote add origin git@github.com:{}.git && git "
                "branch -M "
                "main && git push -u origin main ",
                final)};
            Isubcommand(G, otemp, "");
        }
        if (args.count("repo")) {
            T->repoName = true;
        }
        if (args.count("Des")) {
            T->repoDes = true;
        }
        if (args.count("type")) {
            T->repoMode = true;
        }

        if (args.count("undo")) {
            T->Rreset = true;
            int trip = int(0);
            string Sreset_temp;
            for (auto str : arg("undo", vector<string>)) {
                if (str == "soft" || str == "s") {
                    if (trip == 0) {
                        Sreset_temp += "--soft HEAD~";
                        trip += 1;
                    }
                } else if (str == "hard" || str == "h") {
                    if (trip == 0) {
                        Sreset_temp += "--hard HEAD~";
                        trip += 1;
                    }
                } else if (str == "mixed" || str == "m") {
                    if (trip == 0) {
                        Sreset_temp += "--mixed HEAD~";
                        trip += 1;
                    }
                } else if (atoi(str.c_str()) != 0) {
                    if (Sreset_temp.length() > 0) Sreset_temp += str;
                }
            }
            if (Sreset_temp.length() <= 0)
                exitWithoutHelp(
                    "Invalid parameters passed to --undo options are [ "
                    "type of "
                    "reset (hard/mixed/soft) ] [ number of commits back ] ");

            G->Resetcommand += " && git reset ";
            G->Resetcommand += Sreset_temp;
            Isubcommand(G, G->Resetcommand, "");
        }
        if (args.count("git")) {
            string Git_temp;
            for (auto s : args["git"].as<vector<string>>()) {
                Git_temp += " && git ";
                Git_temp += s;
            }
            Isubcommand(G, Git_temp, "");
        }
        if (args.count("Grab")) {
            bool lcheck = findFile("/bin", "svn");
            if (!lcheck)
                exitWithoutHelp(
                    "Could not find dependency : Svn... Terminating");
            string grabstr("svn export ");
            grabstr.append(arg("Grab", string));
            int initlength = grabstr.length();

            // first checks for a folder:
            //  1: checks for "tree/main"
            boost::algorithm::replace_all(grabstr, "tree/main", "trunk");
            // 2: if not found, then your most likely exporting a branch
            boost::algorithm::replace_all(grabstr, "tree", "branches");

            // second checks for a file:
            //  1: checks for /blob/main
            boost::algorithm::replace_all(grabstr, "blob/main", "trunk");
            // 2: checks for /blob/master
            boost::algorithm::replace_all(grabstr, "blob/master", "trunk");

            // checks if no changes were made, then it could not edit the
            // string
            if (grabstr.length() == initlength)
                exitWithoutHelp("could not resolve github link used\n");
            Isubcommand(G, " && ", grabstr);
        }
        if (args.count("Json")) {
            fmt::print(
                "================================================\n"
                "                JSON options                    \n"
                "username        : your username\n"
                "token           : your gitHub Token\n"
                "default_message : a default commit message\n"
                "================================================\n");
        }
    } catch (const opt::error &ex) {
        cerr << program_invocation_name << ": " << ex.what() << "\n";
        exit(1);
    }

    try {
        parse(G, T);
    } catch (...) {
        cout << program_invocation_name << ": Parsing options failed..."
             << endl;
        exit(1);
    }
    exit(0);
}

auto Isubcommand(Globals *g, const string_view &s1, const string_view &s2)
    -> void {
    g->subcommand += s1;
    if (s2.length() != 0) g->subcommand += s2;
}

auto exitWithoutHelp(const string_view &e, int return_val) -> void {
    fmt::print(stderr, "ERR: {}\n", e);
    exit(return_val);
}

auto exitWithHelp(const string_view &e, int return_val) -> void {
    fmt::print(stderr,
               "ERR: {}\ntry {} --help or \e[1mman gitalias\e[0m for more "
               "information \n",
               e, program_invocation_name);
    exit(return_val);
}

[[nodiscard("grab UserInfo from file")]] auto ParseUserInfo() -> UserInfo {
    UserInfo userinfo;
    std::string filename = getenv("HOME");
    bool fileCreated{false};
    filename.append("/.config/gitalias/githubuserinfo.json");

    std::fstream FileToWorkWith;
    FileToWorkWith.open(filename, std::fstream::in | std::fstream::out);

    // If file does not exist, Create new folder for the file
    if (!FileToWorkWith.is_open()) {
        fileCreated = true;
        fmt::print(
            "Cannot open file, file does not exist. Creating new file..\n");

        // checking if folder exists
        if (!std::filesystem::exists(std::string(
                std::string(getenv("HOME")).append("/.config/gitalias")))) {
            std::filesystem::create_directory(std::string(
                std::string(getenv("HOME")).append("/.config/gitalias")));
        };

        FileToWorkWith.open(filename, std::fstream::in | std::fstream::out |
                                          std::fstream::trunc);
        if (!FileToWorkWith.is_open()) {
            exitWithoutHelp("\nerror creating file\n");
        }

        if (FileToWorkWith.is_open() && !FileToWorkWith.bad()) {
            FileToWorkWith << fmt::format(
                "{{\n"
                "\"username\":\"YOUR_GITHUB_USERNAME\",\n"
                "\"token\" : \"YOUR_GITHUB_TOKEN\",\n"
                "\"default_message\" : \"DEFAULT COMMIT MESSAGE\"\n"
                "}}\n");
        } else {
            fmt::print("error with opened file\n");
            exit(1);
        }
    }  // open/create file

    if (FileToWorkWith.is_open() && !FileToWorkWith.bad() &&
        FileToWorkWith.good()) {
        /* parse Json */
        char *s = new char[2];
        if (FileToWorkWith.getline(s, 1)) {
            if (std::string(s).size() == 0) {
                fmt::print("ERROR with opened file :: EMPTY FILE\n");
            }
            exit(1);
        }
        try {
            boost::property_tree::ptree pt;
            boost::property_tree::read_json(filename, pt);
            userinfo.username = pt.get<std::string>("username");
            userinfo.token = pt.get<std::string>("token");
            // userinfo.defaultMessage is not compulsory
            try {
                userinfo.defaultMessage =
                    pt.get<std::string>("default_message");
            } catch (...) {
                userinfo.defaultMessage = "";
            }
        } catch (const boost::wrapexcept<
                 boost::property_tree::json_parser::json_parser_error> &ex) {
            fmt::print("{}", ex.what());
        }
    }  // read json file

    // if (fileCreated || userinfo.username == "YOUR_GITHUB_USERNAME" ||
    //     userinfo.token == "YOUR_GITHUB_TOKEN") {
    //   exitWithoutHelp(
    //       "~/.config/gitalias/githubuserinfo was generated\n"
    //       "It a default config has been created\n"
    //       "Edit the json file and replace the values with your vaules\n"
    //       "And try again\n"
    //       "This program will terminte\n");
    // }

    if (userinfo.token.size() == 0 || userinfo.username.size() == 0) {
        exitWithoutHelp(
            "Invalid values obtain from config file "
            "~/.config/gitalias/githubuserinfo");
    }  // confirming token size

    if (fileCreated || userinfo.username == "YOUR_GITHUB_USERNAME" ||
        userinfo.token == "YOUR_GITHUB_TOKEN") {
        userinfo.username = "_";
        userinfo.token = "_";
    }

    FileToWorkWith.close();

    if (userinfo.token == "_")
        ;
    else if (userinfo.token.size() > 41) {
        exitWithoutHelp(
            "Invalid values obtain from config file\n"
            "in ~/.config/gitalias/githubuserinfo:\n"
            "Token length > 40 chars");
    }

    return userinfo;
}

/* auto parseGitHubUserFile() -> UserInfo { */
/*     std::string token{}; */
/*     int check = findFile("/usr", "githubToken"); */
/*     if (!check) { */
/*         errorT2( */
/*             "githubUserInfo file not found\ncreate the file 'githubToken'
 * in
 * " */
/*             "/usr " */
/*             "and put only your token inside\nthis is needed to create the
 * "
 */
/*             "online repository\n"); */
/*         exit(1); */
/*     } */

/*     /1* getting the token *1/ */
/*     ifstream tokenfile; */
/*     tokenfile.open("/usr/githubToken"); */
/*     if (!tokenfile) { */
/*         errorT2( */
/*             "Could not read Github token\nit is read from " */
/*             "/usr/githubToken\nplease " */
/*             "create the file and put only the token in...\n"); */
/*     } */
/*     tokenfile >> token; */
/*     tokenfile.close(); */
/*     if (token.size() == 40) */
/*         return token; */
/*     else */
/*         throw; */
/* }; */

auto findFile(const string &place, const string_view &file) -> bool {
    DIR *d;
    struct dirent *dir;
    d = opendir(place.c_str());
    if (d) {
        while ((dir = readdir(d)) != null) {
            string name = dir->d_name;
            if (name == file) {
                closedir(d);
                return 1;
            }
        }
    }
    return 0;
}

auto getGitInfo() -> bool {
    FILE *fd;
    int buffersize = 1024;
    string newTemp;
    char *temp = new (nothrow) char[buffersize];
    bool tempTrip{true};
    /* apparently fopen will bug out if there is and error hence 2>&1 is
        needed */
    fd = popen("git status 2>&1 ", "r");
    if (fd == null || temp == null) exitWithoutHelp("Program Crashed...");
    /* compares the whole string and if it fails to find the string , it
     * will exit and set tempTrip to true hence a second check will be made
     */
    while (fgets(temp, buffersize, fd)) {
        newTemp = temp;
        if (newTemp ==
            "fatal: not a git repository (or any of the parent "
            "directories): .git\n") {
            return !tempTrip;
        }
    }
    /* second check lol */
    fscanf(fd, " %1024s", temp);
    newTemp = temp;
    if (newTemp == "fatal:") return !tempTrip;
    return tempTrip;
}

/* this still has issues
 * rets true if there is an add, false if not
 * this should work lmao
 * */
auto checkStaged() -> bool {
    /* when there is no add we get the strings pasted below */
    FILE *fd = popen(" git status ", "r");
    char *temp = new (nothrow) char[1024];
    if (fd == null || temp == null) exitWithoutHelp("Program Crahsed...");
    while (fgets(temp, 1023, fd)) {
        string_view tempcmp{temp};
        if (tempcmp == "Changes not staged for commit:\n" ||
            tempcmp == "No commits yet\n" ||
            tempcmp ==
                "nothing added to commit but untracked files present (use "
                "\"git add\" to track)\n" ||
            tempcmp == "Changes to be committed\n") {
            /* if string match is found we return false cus we are checking
             * for no add
             */
            fclose(fd);
            return false;
        }
    };
    fclose(fd);
    return true;
}

auto createOnlineRepo(Globals *g) -> void {
    /* variables we need */
    string token = ParseUserInfo().token;
    char *crepo = new (nothrow) char[500];

    if (token == "_") {
        exitWithoutHelp(
            "Token value not set\nEdit "
            "~/.config/gitalias/githubuserinfo.json\n");
    }

    if (crepo == null)
        exitWithoutHelp("Program Crashed... unable to alloated string memory");
    char confirm{};

    /* consent lol */
    fmt::print(
        "Online repository details:\n"
        "\tName: {}\n"
        "\tDescription : {}\n"
        "\tVisibility: {}\n"
        "\n\n"
        "Create repository [y,N] : ",
        g->reponame, g->repodes, (g->mode == true ? "Private" : "Public"));

    cin >> confirm;
    if ((char)tolower(confirm) != 'y')
        exitWithoutHelp("User has stopped the process");

    if (!findFile("/bin", "curl")) {
        exitWithoutHelp(
            "Dependency curl not found\nProgram curl is needed for this "
            "operation\nDownload curl to use this feature");
    }

    snprintf(crepo, 500,
             "curl -X POST -H \"Authorization: Bearer %s\""
             " https://api.github.com/user/repos -d "
             "'{\"name\":\"%s\",\"description\":\"%s\",\"homepage\":\"https:"
             "//github.com\",\"private\":%s}' 2>&1",
             token.c_str(), g->reponame.c_str(), g->repodes.c_str(),
             (g->mode == true ? "true" : "false"));

    /* running it here instead of passing it to run */
    FILE *instance = popen(crepo, "r");
    char buffer[1000];
    while (fgets(buffer, 999, instance) != null) {
        if (strstr(buffer, "Bad credentials") != null) {
            exitWithoutHelp(
                "Bad credentials : Please update your token, it may have "
                "expired");
        } else if (strstr(buffer, "Could not resolve host: api.github.com") !=
                   null) {
            exitWithoutHelp(
                "Could not connect to api.github.com\n"
                "check internet connection");
        }
    }
    cout << program_invocation_name << ": " << g->reponame
         << " repository created succesfully\n";
}

auto gitalias_main(Globals *g, bool v) -> void {
    if (g->subcommand.length()) g->command += g->subcommand;
    if (v) {
        fmt::print("\ngitalias V2.6.6-dev\nRunning Command: {} \n", g->command);
        fmt::print("Press any key to continue, Ctrl+c to quit...\n");
        getchar();
    }
    if (g->App_error.error_occured) {
        exitWithoutHelp(g->App_error.what());
    }

    system(g->command.c_str());
    exit(0);
}

auto parse(Globals *g, Trips *t) -> void {
    string parseCommand;
    bool isInit = getGitInfo();
    /* only when its needed any not for everything */
    /* might remove this in the future */
    if ((t->commit || t->add || t->branch || t->deleteBranch || t->init ||
         t->log || t->switch_ || t->merge || t->status || t->push || t->pull) ==
        true) {
        char chrInit;
        if (isInit == 0) {
            if (t->init) {
                parseCommand += " && git init ";
            } else {
                cout << "Git repository not found\nDo you want to "
                        "initialize a "
                        "git "
                        "repository here?[y,N]: ";
                cin >> chrInit;
                if ((char)tolower(chrInit) != 'y') {
                    t->init = false;
                    exitWithoutHelp("No git repository found, exiting..");
                    exit(1);
                } else
                    parseCommand += " && git init ";
            }
        } else {
            if (t->init) cout << "Git repo already exits skipping...\n";
            t->init = false;
        }
    }
    /* === User repo creation ===*/

    if (t->repoMode && t->repoName && t->repoDes) {
        createOnlineRepo(g);
    } else if (t->repoMode || t->repoDes || t->repoName) {
        cerr << program_invocation_name
             << ": User cannot create online repository, not enough "
                "information\n"
             << "Must fill fields --repo <string> , --Des <string> and "
                "--type "
                "<bool> \n";
    }

    /*if a commit trip is made all these rules apply */
    if (t->commit) {
        /*check g trip */
        if (strstr(g->subcommand.c_str(), "commit") != null)
            exitWithoutHelp("Err: used commit in --git and --commit together");
        /* if not message trip  */
        if (!t->message) {
            std::string commitMessage = ParseUserInfo().defaultMessage;
            if (commitMessage == "DEFAULT COMMIT MESSAGE")
                commitMessage.clear();

            g->messagebox = commitMessage.size() == 0
                                ? " -m 'made some changes' "
                                : fmt::format("-m '{}' ", commitMessage);
        }
        /* if not add trip */
        if (!t->add) {
            /* if no added files */
            if (!checkStaged()) g->addbox += " && git add . ";  // add all
        }

        /* commit trip  occured */
        parseCommand += g->addbox;
        parseCommand += " && git commit ";
        parseCommand += g->messagebox;
    } else {
        if (t->message)
            exitWithoutHelp("Cannot use option --message without a commit");
        if (t->add) parseCommand += g->addbox;
    }
    if (parseCommand.length() > 0) g->command += parseCommand;

    gitalias_main(g, t->verbose);
}
