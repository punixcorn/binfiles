/* Copyright 2023 potato@c137 */
/* Gitalias re-writeen in cpp for long options as get_optlong did not meet requirements*/
/* C++ includes */
#include <boost/program_options.hpp>
#include <iostream>
#include <fstream>
#include <cstdlib> /* c++ remake of stdlib*/
#include <cstdio>  /* c++ remake of stdio*/
#include <string>
#include <vector>
#include <boost/program_options/errors.hpp>
#include <boost/algorithm/string.hpp>
/*C includes*/
#include <dirent.h>
#include <errno.h>
#include <unistd.h>

namespace opt = boost::program_options;

/*=============== global varibales ================= */
std::string messagebox, addbox, reponame, repodes, subcommand, command = "[[ -f /bin/git || -f /usr/bin/git ]] ";
bool mode; // for the user request;
struct Trips
{
  bool commit, message, add, init, branch, switch_, deleteBranch, merge, pull, push, origin, log, status, repoName, repoDes, repoMode, verbose;
};

/* ============== function declarations ================*/
/* for debug purposes, prints the string e */
auto debugg(std::string e) -> void;
/*append into subcommand*/
void Isubcommand(std::string s1, std::string s2);
/* prints error message e and kills the program */
auto errorT1(std::string e) -> void;
/* prints error message e and kills the program */
auto errorT2(std::string e) -> void;
/* checks if you have a git repo init */
auto getGitInfo() -> bool;
/* check is something exists at place */
auto getdir(std::string place, std::string thing) -> bool;
/* create an online repository */
auto createOnlineReop(bool check) -> std::string;
/* runs the command */
auto run(bool v) -> void;
/* parses the struct  */
auto parse(Trips *t) -> void;

/* ============== Main ============*/
int main(int argc, char *argv[])
{

  if (argc < 2)
  {
    errorT1((std::string) "No arguments passed...");
  }

  Trips *T = new (std::nothrow) Trips;
  if (T == NULL)
  {
    std::cerr << "Program Crashed...\n";
    exit(1);
  }
  try
  {
    memset(T, 0, sizeof(T));
  }
  catch (...)
  {
    std::cerr << "Program Crashed...\n";
    exit(1);
  }
  try
  {
    /*
    ---- some info ----

    with notifier() we can call directly to functions
    add implicit_values() as default values causes Trips if called or not , implicit only when called

    -_-_-_-_-_-_-_-_- what to do ? -_-_-_-_-_-_-_-_-_-

    i think implementation of this should be that one could do multiple things at once?
    say :
    ` gitalias -a file1 file2 -c -m "added 2 files" --repo "files" --des "my new files" --mode true --origin "user/files" `
    this would add some files , then it would commit with the message "added 2 files" then create an online repo called "files" then add it to this offline one..
    1. loads of strings to generate {
      could fix this if we used the default generated one? => vm["option"].as<type>()
    }

    2. parsing , as how they are translated into git language and passed into the terminal ? {
      could fix this is we just made every `&& git <command> `
      execpt a few
      -m -c => as m depends on commit and not the other way round
      to fix the -m and -c we will make them the first thing checked
      as they will be parsed first for any checks and errors
    }

    */
    opt::options_description desc(std::string(*argv).append(" options"));
    desc.add_options()("help,h", "print this message")("init,i", "init a repository")("commit,c", "add all and commit")("add,a", opt::value<std::vector<std::string>>()->multitoken(), "add [files] only")("message,m", opt::value<std::vector<std::string>>()->multitoken(), "add a message")("branch,b", opt::value<std::string>()->implicit_value(""), "create a branch")("switch,s", opt::value<std::string>(), "switch to a branch")("delete,d", opt::value<std::string>(), "delete a branch")("Merge,M", opt::value<std::string>(), "merge branch [ branch-name ] with current branch")("Pull,P", opt::value<std::string>(), "pull from origin")("push,p", opt::value<std::string>(), "push into origin")("verbose,v", "print out parsed code")("log,g", "show log files")("Status,S", "show statuts")("origin,o", opt::value<std::string>(), "add an origin")("repo,r", opt::value<std::string>(&reponame), "name for creating an online repo * [i]")("Des,D", opt::value<std::string>(&repodes), "description for the online repo * [ii]")("type,t", opt::value<bool>(&mode)->value_name("bool"), "bool : true/false | if repo should be private *[iii]");

    opt::variables_map args;
    opt::store(opt::command_line_parser(argc, argv).options(desc).style(opt::command_line_style::default_style | opt::command_line_style::allow_long_disguise | opt::command_line_style::allow_sticky | opt::command_line_style::allow_guessing).run(), args);
    opt::notify(args);
    if (args.count("help"))
    { /* fixing the stream */
      std::stringstream stream;
      stream << desc;
      std::string helpMsg = stream.str();
      boost::algorithm::replace_all(helpMsg, "--", "-");
      boost::algorithm::replace_all(helpMsg, "[ -", "[ --");
      /* maybe not */
      std::cout << helpMsg << "\n";
      exit(0);
    }
    /* Check for trips */
    if (args.count("commit"))
    {
      debugg("c");
      T->commit = true;
    }

    if (args.count("message"))
    {
      debugg("m");
      T->message = true;
      for (std::vector<std::string>::const_iterator i = args["message"].as<std::vector<std::string>>().begin(); i != args["message"].as<std::vector<std::string>>().end(); i++)
      {
        messagebox += *i;
        messagebox += "-m ";
      }
    }

    if (args.count("init"))
    {
      debugg("i");
      T->init = true;
    }
    if (args.count("add"))
    {
      debugg("a");
      T->add = true;
      addbox += " && git add ";
      for (std::vector<std::string>::const_iterator i = args["add"].as<std::vector<std::string>>().begin(); i != args["add"].as<std::vector<std::string>>().end(); i++)
      {
        addbox += *i;
        addbox += " ";
      }
    }
    if (args.count("branch"))
    {
      debugg("b");
      T->branch = true;
      Isubcommand(" && git branch ", "");
      if (args["branch"].as<std::string>().length() > 0)
      {
        subcommand += args["branch"].as<std::string>();
      }
    }
    if (args.count("switch"))
    {
      debugg("s");
      T->switch_ = true;
      Isubcommand(" && git switch ", args["switch"].as<std::string>());
    }
    if (args.count("delete"))
    {
      debugg("d");
      T->deleteBranch = true;
      Isubcommand(" && git branch -d ", args["delete"].as<std::string>());
    }
    if (args.count("Merge"))
    {
      debugg("e");
      T->merge = true;
      Isubcommand(" && git merge ", args["Merge"].as<std::string>());
    }
    if (args.count("pull"))
    {
      debugg("l");
      T->pull = true;
      Isubcommand(" && git pull ", "");
    }
    if (args.count("push"))
    {
      debugg("p");
      T->push = true;
      Isubcommand(" && git push ", "");
    }
    if (args.count("verbose"))
    {
      debugg("v");
      T->verbose = true;
    }
    if (args.count("log"))
    {
      debugg("g");
      T->log = true;
      Isubcommand(" && git log ", "");
    }
    if (args.count("status"))
    {
      debugg("t");
      T->status = true;
      Isubcommand(" && git status ", "");
    }
    if (args.count("origin"))
    {

      char *otemp = new (std::nothrow) char[256];
      debugg("o");
      T->origin = true;
      sprintf(otemp,
              " && git remote add origin git@github.com:%s && git branch -M "
              "main && git push -u origin main",
              args["origin"].as<std::string>().c_str());
      Isubcommand(otemp, "");
    }
    if (args.count("repo"))
    {
      debugg("r");
      T->repoName = true;
    }
    if (args.count("des"))
    {
      debugg("y");
      T->repoDes = true;
    }
    if (args.count("mode"))
    {
      debugg("u");
      T->repoMode = true;
    }
  }
  catch (const opt::error &ex)
  {
    std::cerr << program_invocation_name << ": " << ex.what() << "\n";
    exit(1);
  }

  parse(T);
  exit(0);
}

/*===== End of Main =======*/

auto debugg(std::string e) -> void
{
  std::cout << e << " was called \n";
}

auto Isubcommand(std::string s1, std::string s2) -> void
{
  subcommand += s1;
  if (s2.length() <= 0)
    ;
  else
    subcommand += s2;
}

auto errorT2(std::string e) -> void
{
  std::cerr << program_invocation_name << ": " << e;
  exit(1);
}
auto errorT1(std::string e) -> void
{
  std::cerr << program_invocation_name << ": " << e << "\n try \" " << program_invocation_name << " --help \" for more information " << std::endl;
  exit(1);
}

auto getdir(std::string place, std::string thing) -> bool
{
  DIR *d;
  struct dirent *dir;
  d = opendir(place.c_str());
  if (d)
  {
    while ((dir = readdir(d)) != NULL)
    {
      std::string name = dir->d_name;
      if (name == thing)
      {
        closedir(d);
        return 1;
      }
    }
  }
  return 0;
}

auto getGitInfo() -> bool
{
  FILE *fd;
  size_t len = 0, read;
  char *temp = new (std::nothrow) char[1024];
  std::string newTemp;
  /* apparently fopen will bug out if there is and error hence 2>&1 is needed */
  fd = popen("git status 2>&1 ", "r");
  if (fd == NULL || temp == NULL)
  {
    std::cerr << "Program Crashed...\n";
    exit(1);
  }
  fscanf(fd, " %1023s", temp);
  newTemp = temp;

  if (newTemp == "fatal:")
    return 0;
  return 1;
}

auto createOnlineReop() -> std::string
{
  /* variables we need */
  std::string token, strMode;
  char *crepo = new (std::nothrow) char[500];
  if (crepo == NULL)
    errorT2("Program Crashed...\n");
  char confirm;

  /*checking if the file to read the github token from is alive */
  int check = getdir("/usr", "githubToken");
  if (!check)
  {
    errorT2("githubToken file not found\ncreate the file 'githubToken' in /usr and put only your token inside\nthis is needed to create the online repository\n");
    exit(1);
  }

  /* getting the token */
  std::ifstream tokenfile;
  tokenfile.open("/usr/githubToken");
  if (!tokenfile)
  {
    errorT2("Could not read Github token\nit is read from /usr/githubToken\nplease create the file and put only the token in...\n");
  }
  tokenfile >> token;
  tokenfile.close();
  /* checking if the token's length is 40 like every other */
  if (token.length() > 41)
    errorT2("Was expecting a Token of char[40]\nFix: remove any whitespace in /usr/githubToken\n");

  /*converting bool mode to strings */
  if (mode)
    strMode = "true";
  else
    strMode = "false";
  /* consent lol */
  std::cout << " You are about to create an online repository with the following "
               "data :\n"
               "\tName of repository : "
            << reponame << "\n"
                           "\tDescription : "
            << repodes << "\n"
                          "\tPrivate: "
            << strMode << "\n"
                          "continue with creation of the online repository [Y,n] : ";
  std::cin >> confirm;
  if (confirm == 'N' || confirm == 'n')
    return "User has stopped process ";
  else if (confirm == 'Y' || confirm == 'y')
    ;
  else
    return "Invalid input, option not found ";

  check = getdir("/bin", "curl");
  if (!check)
    errorT2("curl not found\nProgram curl is needed for this operation\nDownload curl to use this feature\n");
  sprintf(crepo,
          " && curl -X POST -H \"Authorization: Bearer %s\""
          " https:\/\/api.github.com\/user\/repos -d "
          "'{\"name\":\"%s\",\"description\":\"%s\",\"homepage\":\"https:"
          "\/\/github.com\",\"private\":%s}'",
          token.c_str(), reponame.c_str(), repodes.c_str(), strMode.c_str());

  return crepo;
}

auto run(bool v) -> void
{
  command += subcommand;
  if (v)
    std::cout << command << std::endl;
  std::system(command.c_str());
  exit(0);
}

auto parse(Trips *t) -> void
{
  std::string parseCommand;
  /* checks for a initialized repository */
  bool isInit = getGitInfo();
  std::cout << isInit;
  char chrInit;
  if (isInit == 0)
  {
    std::cout << "Git repository not found\nDo you want to initialize a git repository here?[y,N]: ";
    std::cin >> chrInit;
    if (chrInit == 'Y' || chrInit == 'y')
      t->init = true;
    else if (chrInit == 'N' || chrInit == 'n')
      t->init = false;
    else
      t->init = false;
  }
  else
  {
    if (t->init)
      std::cout << "Git repo already exits skipping...\n";
    t->init = false;
  }

  /* === User repo creation ===*/
  if (t->repoMode && t->repoName && t->repoDes)
  {
    std::string repoStr;
    repoStr = createOnlineReop();
    std::string::const_iterator i = repoStr.begin();
    if (*(i) == 'U')
      errorT2(repoStr);
    else
      subcommand += repoStr;
  }

  /* ----- end of single checks ------ */

  /* init trip */
  if (t->init)
  {
    parseCommand += " && git init ";
  }
  /* if not message trip  */
  if (!t->message)
  {
    messagebox = "commit made";
  }
  /* if not add trip */
  if (!t->add)
  {
    addbox += "&& git add . ";
  }

  /* commit trip  occured */
  if (t->commit)
    parseCommand += " && git commit ";

  parseCommand += addbox;
  parseCommand += messagebox;

  run(t->verbose);
}
