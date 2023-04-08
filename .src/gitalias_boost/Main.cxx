/* Copyright 2023 punicorn
 *
 * Gitalias a git alias
 * move all string copies  to std::string_view
 */

/* boost includes */
#include <boost/algorithm/string.hpp>
#include <boost/program_options.hpp>
#include <boost/program_options/errors.hpp>
#include <boost/program_options/value_semantic.hpp>
/* C++ includes */
#include <array>
#include <fstream>
#include <iostream>
#include <new>
#include <string>
#include <string_view>
#include <vector>
/*C includes*/
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <dirent.h>
#include <errno.h>
#include <unistd.h>

namespace opt = boost::program_options;

/*global varibales -> helps prevents extreme function arguments*/
std::string messagebox, addbox, reponame, repodes, subcommand, Resetcommand,
    command = "[[ -f /bin/git || -f /usr/bin/git ]] ";
bool mode; // for the user request;
std::vector<std::string>(Sreset);
struct Trips {
  bool commit, message, add, init, branch, switch_, deleteBranch, merge, pull,
      push, origin, log, status, repoName, repoDes, repoMode, verbose, Rreset,
      git;
};

/* ============== function declarations ================*/
/* append into subcommand [ ret: void ]*/
void Isubcommand(const std::string_view &s1, const std::string_view &s2);
/* prints error message e with help message and kills the program [ ret: void
 * ]*/
auto errorT1(const std::string_view &e) -> void;
/* prints error message e and kills the program  [ ret: void ]*/
auto errorT2(const std::string_view &e) -> void;
/* check is something exists at place [ ret: bool ]*/
auto getdir(const std::string &place, const std::string_view &thing) -> bool;
/* checks if you have a git repo init [ ret: bool ]*/
auto getGitInfo() -> bool;
/* check for staged files [ ret: bool ]*/
auto Checkadd() -> bool;
/* create an online repository [ ret: void ]*/
auto createOnlineRepo() -> void;
/* runs the command [ ret: void ]*/
auto run(bool v) -> void;
/* parses the struct [ ret: void ] */
auto parse(Trips *t) -> void;

/* ============== Main ============*/
int main(int argc, char *argv[]) {

  if (argc < 2) {
    errorT1("No arguments passed...");
  }

  Trips *T = new (std::nothrow) Trips;
  if (T == NULL) {
    std::cerr << program_invocation_name << ": Program Crashed...\n"
              << "Err: Trip could not be allocated\n";
    exit(1);
  }
  try {
    memset(T, 0, sizeof(Trips));
  } catch (...) {
    std::cerr << program_invocation_name << ": Program Crashed...\n"
              << "Err : Trip reset<memset()>\n";
    exit(1);
  }
  try {
    opt::options_description desc(std::string(*argv).append(" options"));
    desc.add_options()("help,h", "print this message")(
        "init,i", "init a repository")("commit,c", "add all and commit")(
        "add,a", opt::value<std::vector<std::string>>()->multitoken(),
        "add [files] only")(
        "message,m", opt::value<std::vector<std::string>>()->multitoken(),
        "add a message")("branch,b",
                         opt::value<std::string>()->implicit_value(""),
                         "create a branch")(
        "switch,s", opt::value<std::string>(), "switch to a branch")(
        "delete,d", opt::value<std::string>(),
        "delete a branch")("Merge,M", opt::value<std::string>(),
                           "merge branch [ branch-name ] with current branch")(
        "Pull,P", opt::value<std::string>()->implicit_value(""),
        "pull from origin")(
        "push,p", opt::value<std::string>()->implicit_value(""),
        "push into origin")("Clone,C", opt::value<std::string>(),
                            "clone a repository with given \"user/repo-name\"")(
        "Request,R", opt::value<std::string>()->default_value("https"),
        "Protocol to use when cloning, [ https/ssh ]")("verbose,v",
                                                       "print out parsed code")(
        "log,l", "show log files")("Status,S", "show statuts")(
        "origin,o", opt::value<std::string>(),
        "add an origin")("repo,r", opt::value<std::string>(&reponame),
                         "name for creating an online repo * [i]")(
        "Des,D", opt::value<std::string>(&repodes),
        "description for the online repo * [ii]")(
        "type,t", opt::value<bool>(&mode)->value_name("bool"),
        "if repo should be private *[iii]")(
        "undo,u", opt::value<std::vector<std::string>>()->multitoken(),
        "*Reset back to a commit , "
        "1st arg : type of reset ( hard / soft / mixed ) , "
        "2nd arg : number of commits to reset back")(
        "git,g", opt::value<std::vector<std::string>>()->multitoken(),
        "run git command =[  git <arg> ] if you add more commands "
        "append git to it eg:  ga -G \"add .\" \" git commit -m \"foo\" \" ... "
        "]")("Grab,G", opt::value<std::string>(),
             "grab a specific folder from a github repo");

    opt::variables_map args;
    opt::store(opt::command_line_parser(argc, argv)
                   .options(desc)
                   .style(opt::command_line_style::default_style |
                          opt::command_line_style::allow_sticky)
                   .run(),
               args);
    opt::notify(args);
    if (args.count("help")) {
      /* fixing the stream  maybe?
      only needed when theres a command for only shorthands
      std::stringstream stream;
      stream << desc;
      std::string helpMsg = stream.str();
      boost::algorithm::replace_all(helpMsg, "--", "-");
      boost::algorithm::replace_all(helpMsg, "[ -", "[ --");
      std::cout << helpMsg << "\n";
      */
      std::cout << desc << "\n";
      exit(0);
    }
    /* Check for trips */
    if (args.count("commit")) {
      T->commit = true;
    }

    if (args.count("message")) {
      T->message = true;
      for (std::vector<std::string>::const_iterator i =
               args["message"].as<std::vector<std::string>>().begin();
           i != args["message"].as<std::vector<std::string>>().end(); i++) {
        messagebox += " -m '";
        messagebox += *i;
        messagebox += "' ";
      }
    }

    if (args.count("init")) {
      T->init = true;
    }
    if (args.count("add")) {
      T->add = true;
      addbox += " && git add ";
      std::cout << "Files added: ";
      for (auto i : args["add"].as<std::vector<std::string>>()) {
        if (i == ".") {
          std::cout << "all ";
        } else {
          std::cout << i << " ";
        }
        addbox += i;
        addbox += " ";
      }
      std::cout << std::endl;
    }
    if (args.count("branch")) {
      T->branch = true;
      Isubcommand(" && git branch ", "");
      if (args["branch"].as<std::string>().length() > 0) {
        subcommand += args["branch"].as<std::string>();
      }
    }
    if (args.count("switch")) {
      T->switch_ = true;
      Isubcommand(" && git switch ", args["switch"].as<std::string>());
    }
    if (args.count("delete")) {
      T->deleteBranch = true;
      Isubcommand(" && git branch -d ", args["delete"].as<std::string>());
    }
    if (args.count("Merge")) {
      T->merge = true;
      Isubcommand(" && git merge ", args["Merge"].as<std::string>());
    }
    if (args.count("Pull")) {
      T->pull = true;
      if (args["Pull"].as<std::string>() == "all") {
        std::cout << "Pulling all remote branches..\n";
        Isubcommand(" && git push --all ", "");
      } else {
        Isubcommand(" && git pull ", args["Pull"].as<std::string>());
      }
    }
    if (args.count("push")) {
      T->push = true;
      if (args["push"].as<std::string>() == "all") {
        std::cout << "Pushing all branches...\n";
        Isubcommand(" && git push --all ", "");
      } else {
        Isubcommand(" && git push ", args["push"].as<std::string>());
      }
    }
    if (args.count("Clone")) {
      std::string clonestr = " && git clone ";
      if (args["Request"].as<std::string>() == "ssh") {
        clonestr += "git@github.com:";
        clonestr += args["Clone"].as<std::string>();
        clonestr += ".git ";
      } else {
        std::cout << program_invocation_name
                  << ": using default Protocol for cloning : https...\n";
        clonestr += "https://github.com/";
        clonestr += args["Clone"].as<std::string>();
        clonestr += " ";
      }
      Isubcommand(clonestr, "");
    }
    if (args.count("verbose")) {
      T->verbose = true;
    }
    if (args.count("log")) {
      T->log = true;
      Isubcommand(" && git log ", "");
    }
    if (args.count("Status")) {
      T->status = true;
      Isubcommand(" && git status ", "");
    }
    if (args.count("origin")) {
      size_t buffersize = 256;
      char *otemp = new (std::nothrow) char[buffersize];
      T->origin = true;
      snprintf(otemp, buffersize,
               " && git remote add origin git@github.com:%s && git branch -M "
               "main && git push -u origin main",
               args["origin"].as<std::string>().c_str());
      Isubcommand(otemp, "");
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
      std::string Sreset_temp;
      for (auto s : args["undo"].as<std::vector<std::string>>()) {
        if (s == "soft" || s == "s") {
          Sreset_temp += "--soft HEAD~";
        } else if (s == "hard" || s == "h") {
          Sreset_temp += "--hard HEAD~";
        } else if (s == "mixed" || s == "m") {
          Sreset_temp += "--mixed HEAD~";
        } else if (atoi(s.c_str()) != 0) {
          if (Sreset_temp.length() > 0)
            Sreset_temp += s;
        }
      }
      if (Sreset_temp.length() <= 0)
        errorT2("Invalid parameters passed to --undo options are [ type of "
                "reset (hard/soft) ] [ number of commits back ] \n");

      Resetcommand += " && git reset ";
      Resetcommand += Sreset_temp;
      Isubcommand(Resetcommand, "");
    }
    if (args.count("git")) {
      std::string Git_temp;
      for (auto s : args["git"].as<std::vector<std::string>>()) {
        Git_temp += " && git ";
        Git_temp += s;
      }
      Isubcommand(Git_temp, "");
    }
    if (args.count("Grab")) {

      std::string grabstr("svn export ");
      grabstr.append(args["Grab"].as<std::string>());
      int initLenght = grabstr.length();
      // first checks for "tree/main"
      boost::algorithm::replace_all(grabstr, "tree/main", "trunk");
      // if not found, then your most likely exporting a branch
      boost::algorithm::replace_all(grabstr, "tree", "branches");
      if (grabstr.length() == initLenght)
        errorT2("could not resolve github link used\n");
      Isubcommand(" && ", grabstr);
    }

  } catch (const opt::error &ex) {
    std::cerr << program_invocation_name << ": " << ex.what() << "\n";
    exit(1);
  }
  try {
    parse(T);
  } catch (...) {
    std::cout << program_invocation_name << ": Parsing options failed..."
              << std::endl;
    exit(1);
  }
  exit(0);
}
/*===== End of Main =======*/

auto Isubcommand(const std::string_view &s1, const std::string_view &s2)
    -> void {
  subcommand += s1;
  if (s2.length() != 0)
    subcommand += s2;
}

auto errorT2(const std::string_view &e) -> void {
  std::cerr << program_invocation_name << ": " << e;
  exit(1);
}

auto errorT1(const std::string_view &e) -> void {
  std::cerr << program_invocation_name << ": " << e << "\n try \" "
            << program_invocation_name << " --help \" for more information "
            << std::endl;
  exit(1);
}

auto getdir(const std::string &place, const std::string_view &thing) -> bool {
  DIR *d;
  struct dirent *dir;
  d = opendir(place.c_str());
  if (d) {
    while ((dir = readdir(d)) != NULL) {
      std::string name = dir->d_name;
      if (name == thing) {
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
  std::string newTemp;
  char *temp = new (std::nothrow) char[buffersize];
  bool tempTrip{true};
  /* apparently fopen will bug out if there is and error hence 2>&1 is
   * needed */
  fd = popen("git status 2>&1 ", "r");
  if (fd == NULL || temp == NULL)
    errorT2("Program Crashed...\n");
  /* compares the whole string and if it fails to find the string , it will
   * exit and set tempTrip to true hence a second check will be made */
  while (fgets(temp, buffersize, fd)) {
    newTemp = temp;
    if (newTemp == "fatal: not a git repository (or any of the parent "
                   "directories): .git\n") {
      return !tempTrip;
    }
  }
  /* second check lol */
  fscanf(fd, " %1024s", temp);
  newTemp = temp;
  if (newTemp == "fatal:")
    return !tempTrip;
  return tempTrip;
}
/* this still has issues
 * rets true if there is an add, false if not
 * this should work lmao
 * */
auto Checkadd() -> bool {
  /*
   * when there is no add we get the strings pasted below
   */
  FILE *fd = popen(" git status ", "r");
  char *temp = new (std::nothrow) char[1024];
  if (fd == NULL || temp == NULL)
    errorT2("Program Crahsed...\n");
  while (fgets(temp, 1023, fd)) {
    std::string_view tempcmp{temp};
    if (tempcmp == "Changes not staged for commit:\n" ||
        tempcmp == "No commits yet\n" ||
        tempcmp == "nothing added to commit but untracked files present (use "
                   "\"git add\" to track)\n" ||
        tempcmp == "Changes to be committed\n") {
      /*if string match is found we return false cus we are checking for no
       * add
       */
      return false;
    }
  };
  return true;
}

auto createOnlineRepo() -> void {
  /* variables we need */
  std::string token, strMode;
  char *crepo = new (std::nothrow) char[500];
  if (crepo == NULL)
    errorT2("Program Crashed...\n");
  char confirm;
  /*checking if the file to read the github token from is alive */
  int check = getdir("/usr", "githubToken");
  if (!check) {
    errorT2("githubToken file not found\ncreate the file 'githubToken' in /usr "
            "and put only your token inside\nthis is needed to create the "
            "online repository\n");
    exit(1);
  }

  /* getting the token */
  std::ifstream tokenfile;
  tokenfile.open("/usr/githubToken");
  if (!tokenfile) {
    errorT2("Could not read Github token\nit is read from "
            "/usr/githubToken\nplease "
            "create the file and put only the token in...\n");
  }
  tokenfile >> token;
  tokenfile.close();
  /* checking if the token's length is 40 like every other */
  if (token.length() > 41)
    errorT2("Was expecting a Token of char[40]\nFix: remove any whitespace in "
            "/usr/githubToken\n");

  /*converting bool mode to strings */
  if (mode)
    strMode = "true";
  else
    strMode = "false";
  /* consent lol */
  std::cout
      << " You are about to create an online repository with the following "
         "data :\n"
         "\tName of repository : "
      << reponame
      << "\n"
         "\tDescription : "
      << repodes
      << "\n"
         "\tPrivate: "
      << strMode
      << "\n"
         "continue with creation of the online repository [y,N] : ";
  std::cin >> confirm;
  if ((char)tolower(confirm) != 'y')
    errorT2("User has stopped the process\n");
  check = getdir("/bin", "curl");
  if (!check)
    errorT2("curl not found\nProgram curl is needed for this "
            "operation\nDownload curl to use this feature\n");
  snprintf(crepo, 500,
           "curl -X POST -H \"Authorization: Bearer %s\""
           " https://api.github.com/user/repos -d "
           "'{\"name\":\"%s\",\"description\":\"%s\",\"homepage\":\"https:"
           "//github.com\",\"private\":%s}'",
           token.c_str(), reponame.c_str(), repodes.c_str(), strMode.c_str());
  /* running it here instead of passing it to run */
  FILE *instance = popen(crepo, "r");
  char buffer[100];
  while (std::fgets(buffer, 100, instance) != NULL) {
    if (strstr(buffer, "Bad credentials") != NULL) {
      errorT2(
          "Bad credentials : Please update your token, it may have expired\n");
    }
  }
  std::cout << program_invocation_name << ": " << reponame
            << " repository created succesfully\n";
}

auto run(bool v) -> void {
  if (subcommand.length() > 0)
    command += subcommand;
  if (v) {
    std::cout << "Gitalias VERSION 2.2.1\nCommand generated: " << command
              << std::endl;
  }
  std::system(command.c_str());
  exit(0);
}

auto parse(Trips *t) -> void {
  std::string parseCommand;
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
        std::cout
            << "Git repository not found\nDo you want to initialize a git "
               "repository here?[y,N]: ";
        std::cin >> chrInit;
        if ((char)tolower(chrInit) != 'y') {
          t->init = false;
          errorT2("No git repository found, exiting..\n");
          exit(1);
        } else
          parseCommand += " && git init ";
      }
    } else {
      if (t->init)
        std::cout << "Git repo already exits skipping...\n";
      t->init = false;
    }
  }
  /* === User repo creation ===*/

  if (t->repoMode && t->repoName && t->repoDes) {
    createOnlineRepo();
  } else if (t->repoMode || t->repoDes || t->repoName) {
    std::cerr << program_invocation_name
              << ": User cannot create online repository, not enough "
                 "information\n"
              << "Must fill fields --repo <string> , --Des <string> and --type "
                 "<bool> \n";
  }

  /*if a commit trip is made all these rules apply */
  if (t->commit) {
    /*check g trip */
    if (strstr(subcommand.c_str(), "commit") != NULL)
      errorT2("Err: used commit in --git and -c / --commit together");
    /* if not message trip  */
    if (!t->message) {
      messagebox = " -m 'commit made' ";
    }
    /* if not add trip */
    if (!t->add) {
      /* if no added files */
      if (!Checkadd())
        addbox += " && git add . "; // add all
    }

    /* commit trip  occured */
    parseCommand += addbox;
    parseCommand += " && git commit ";
    parseCommand += messagebox;
  } else {
    if (t->message)
      errorT2("Cannot use option --message without a commit\n");
    if (t->add)
      parseCommand += addbox;
  }
  if (parseCommand.length() > 0)
    command += parseCommand;

  run(t->verbose);
}
