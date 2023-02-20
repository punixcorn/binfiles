/* Copyright 2023 potato */
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* global variables */
char command[2048];
char programName[100];
char *optarg, *opt_message, *opt_branch, *opt_origin, *opt_add, *opt_repoName,
    *opt_description, *githubCreate, *opt_view;
struct trip {
  int init, commit, add, message, branch, switch_, merge, pull, push, origin,
      log, status, deleteBranch, repoName, repoDes, mode, verbose;
};

/* ===== functions, order matters ====== */

/* searches for the file 'thing' in the path 'place' */
int getdir(char *place, char *thing);
/* prints the help message and exits the program with the number exit_v */
void print_format(FILE *std, int exit_v, char *exe_name);
void notTogether(char c1, char c2);
/* copys the string in s2 to command */
void input(char *s1);
/* puts the string ss in system to be runned */
void end(char *ss, int v);
/* made for the creation of onlin repository */
void addCmdRepo(int a, int v);
/* copies the string s1 s2 s3 into command */
void runNcheck(char *s1, char *s2, char *s3);
/* copies the string s1 s2 into mainstr and passes mainstr into system() */
void runCheck(char *mainstr, char *s1, char *s2, int v);
/* the logic of this application, makes decisions based on the values of the
 * struct passed in, s is the app name */
void run(struct trip *T, char *s);

/* ========= main function  =========== */
int main(int argc, char *argv[]) {
  if (argc == 1) {
    fprintf(stderr, "%s : Invalid option passed...\n\ttry -h for hlep",
            *argv + 0);
    exit(1);
  }

  /* local variables */
  int opt, optind;
  /* initializing the struct to false */
  struct trip ntrip = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  const char *short_options = ":hica:m:b:s:lpo:ve:gd:r:u:y:t";
  const struct option long_options[] = {
      {"help", 0 /*if it takes an input*/, NULL, 'h'}};
  int next_option;

  while ((opt = getopt(argc, argv, short_options)) != -1) {
    switch (opt) {
    case 'i':
      // init
      ntrip.init = 1;
      break;
    case 'c':
      // commit
      ntrip.commit = 1;
      break;
    case 'a':
      // add files
      opt_add = optarg;
      ntrip.add = 1;
      break;
    case 'm':
      // optarg message
      ntrip.message = 1;
      opt_message = optarg;
      break;
    case 'b':
      // create branch
      if (ntrip.switch_) {
        notTogether('d', 's');
      }
      ntrip.branch = 1;
      opt_branch = optarg;
      break;
    case 's':
      // switch branch
      if (ntrip.branch) {
        notTogether('d', 's');
      }
      opt_branch = optarg;
      ntrip.switch_ = 1;
      break;
    case 'l':
      // pull
      if (ntrip.push) {
        notTogether('p', 'l');
      }
      ntrip.pull = 1;
      break;
    case 'p':
      // push
      if (ntrip.pull) {
        notTogether('p', 'l');
      }
      ntrip.push = 1;
      break;
    case 'o':
      // add origin
      opt_origin = optarg;
      ntrip.origin = 1;
      break;
    case 'h':
      // help
      print_format(stdout, 0, *argv);
      break;
    case 'v':
      // verbose
      fprintf(stdout, "%s", "verbose enabled**");
      ntrip.verbose = 1;
      break;
    case 'e':
      // merge
      opt_branch = optarg;
      ntrip.merge = 1;
      break;
    case 'g':
      // log
      ntrip.log = 1;
      break;
    case 't':
      // status
      ntrip.status = 1;
      break;
    case 'd':
      // delete branch
      ntrip.deleteBranch = 1;
      opt_branch = optarg;
      break;
    case 'r':
      // repo : name
      ntrip.repoName = 1;
      opt_repoName = optarg;
      break;
    case 'u':
      // repo : user
      ntrip.mode = 1;
      opt_view = optarg;
      break;
    case 'y':
      // repo : description
      ntrip.repoDes = 1;
      opt_description = optarg;
      break;
    case ':':
      // errors
      fprintf(stderr, "%s : option used needs a value\n\ttry -h for help\n",
              *(argv));
      exit(EXIT_FAILURE);
      break;
      // command not found
    default:
      fprintf(stderr, "%s : Invalid option passed\n\ttry -h for help\n",
              argv[0]);
      exit(EXIT_FAILURE);
    }
  }

  /* extra checks before running main logic */
  int prechecker = ntrip.push + ntrip.pull + ntrip.merge + ntrip.origin +
                   ntrip.switch_ + ntrip.branch + ntrip.status + ntrip.log;

  int repochecker = ntrip.repoName + ntrip.repoDes + ntrip.mode;
  if (prechecker > 1 || (repochecker == 2 || repochecker == 1)) {
    fprintf(stderr, "%s : Invalid options used together\n", *argv + 0);
    exit(EXIT_FAILURE);
  }
  /* === end of checks === */

  /* main logic */
  run(&ntrip, command);
  /* final checks */
  if (optind >= argc) {
    fprintf(stderr, "Expected argument after options\n");
    exit(EXIT_FAILURE);
  }

  exit(EXIT_SUCCESS);
}

/* ======= end of main function ========= */

int getdir(char *place, char *thing) {
  DIR *d;
  struct dirent *dir;
  d = opendir(place);
  if (d) {
    while ((dir = readdir(d)) != NULL) {
      if (strcmp(dir->d_name, thing) == 0) {
        closedir(d);
        return 0;
      }
    }
  }
  return 1;
}

void print_format(FILE *std, int exit_v, char *exe_name) {
  fprintf(std, "%s : %s [ %s... ]\n", "Usage", exe_name, "options");
  fprintf(
      std,
      "-h              print this message *\n"
      "-i              initialize a repository\n"
      "-c              add and commit\n"
      "-a 'files'      add files only\n"
      "-m 'message'    add a message\n"
      "-b 'branch'     create a branch *\n"
      "-s 'branch'     switch to a branch *\n"
      "-d 'branch'     delete a branch *\n"
      "-l              pull from repo *\n"
      "-p              push to repo\n"
      "-v              print out the current running code *\n"
      "-g              show git log files *\n"
      "-t              show git status *\n"
      "  ==== Online Repo Maniplualtion ====\n"
      "-o 'user/repo'  add an origin *\n"
      "-r 'repo_name'  create an online repo[ the name of your new repo ]\n"
      "-u 'mode'       needed to create online repo [ true / false for private"
      "repo ]\n"
      "-y 'des'        needed to create an online repo[ the description ]\n"
      "\nThis is just a simple alias for git\n"
      "Example:\n"
      "%s -i -c -m 'initial commit'\n"
      "initialize a git repo, add all files and commit with the message "
      "'initial commt'\n"
      "\nif you want to create online repos from here, make sure you have "
      "the file githubToken in /usr "
      "with your token in it\n"
      "To create an online repo, use all 3 options [ -r <repo name> -u <bool> "
      "-y <description> ]\n"
      "Example:\n\t%s -r \"my_repo\" -u \"true\" -y \"this is my new "
      "repo\"\n"
      "Where <mode> := \"true\" or \"false\"\n"
      "if true, the repository will be private, and not if false\n"
      "If all 3 options arent filled, the program will terminate\n"
      "\nNB: "
      "all options that end with [ * ] denotes they must be used alone\n",
      exe_name, exe_name);

  exit(exit_v);
}

void notTogether(char c1, char c2) {
  fprintf(stdout, "ERR : cannot use options -%c -%c together\n", c1, c2);
  exit(1);
}

void input(char *s1) {
  if (s1 != NULL)
    strcat(command, s1);
}

void end(char *ss, int v) {
  if (ss != NULL) {
    if (v) {
      fprintf(stdout, "Command generated: %s\n", ss);
    }
    system(ss);
    exit(EXIT_SUCCESS);
  } else {
    fprintf(stderr, "ERR: NULL value passed in\n");
    exit(EXIT_FAILURE);
  }
}

void runNcheck(char *s1, char *s2, char *s3) {
  if (s1 != NULL && s2 != NULL && s3 != NULL) {
    input(s1);
    input(s2);
    input(s3);
  }
}

void addCmdRepo(int a, int v) {
  char token[100];
  char crepo[500];
  char confirm;
  if (!a) {
    int check = getdir("/usr", "githubToken");
    if (check) {
      fprintf(stderr,
              "ERROR:\n"
              "the file githubToken was not found in /usr\n"
              "Please create the file and put only your github token inside\n"
              "This file is needed to do any creation of repositories\n");
      exit(0);
    }
    /* getting the token */
    FILE *token_file;
    token_file = fopen("/usr/githubToken", "r");
    if (token_file == NULL) {
      fprintf(stderr,
              "ERR: error occured with file githubToken\n"
              "Please make sure it only contains your github token\n"
              "errno: %d\n",
              errno);
      exit(1);
    }
    fread(token, 40, 1, token_file);
    if (opt_view == NULL || opt_description == NULL || opt_repoName == NULL ||
        (strcmp(opt_view, "true") && strcmp(opt_view, "false"))) {
      fprintf(stderr,
              "One of the options used is doesnt have any data in it's field\n"
              "Or the value of -u is incorrect\n");
      exit(1);
    }
    fprintf(stdout,
            "You are about to create an online repository with the following "
            "data :\n"
            "\tName of repository : %s\n"
            "\tDescription : %s\n"
            "\tPrivate: %s\n"
            "continue with creation of the online repository [Y,n] : ",
            opt_repoName, opt_description, opt_view);
    scanf("%c", &confirm);
    if (confirm == 'N' || confirm == 'n') {
      fprintf(stdout, "\noperation cancelled\n");
      exit(0);
    }
    sprintf(crepo,
            "curl -X POST -H \"Authorization: Bearer %s\""
            " https:\/\/api.github.com\/user\/repos -d "
            "'{\"name\":\"%s\",\"description\":\"%s\",\"homepage\":\"https:"
            "\/\/github.com\",\"private\":%s}'",
            token, opt_repoName, opt_description, opt_view);
    end(crepo, v);
  } else {
    fprintf(stderr, "ERR : %s\n%s\n", "The application curl is needed",
            "Install to use this feature");
    exit(1);
  }
}

void runCheck(char *mainstr, char *s1, char *s2, int v) {
  strcpy(mainstr, s1);
  strcat(mainstr, s2);
  end(mainstr, v);
  exit(EXIT_FAILURE);
}

void run(struct trip *T, char *s) {
  /* ------- start of single checks -------- */

  /* branch */
  if (T->branch) {
    runCheck(s, "git branch ", opt_branch, T->verbose);
  }

  /* switch */
  if (T->switch_) {
    runCheck(s, "git switch ", opt_branch, T->verbose);
  }

  /* Pull */
  if (T->pull) {
    runCheck(s, "git pull", " ", T->verbose);
  }

  /* push */
  if (T->push && !T->commit) {
    runCheck(s, "git push", " ", T->verbose);
  }

  /* push */
  if (T->log) {
    runCheck(s, "git log", " ", T->verbose);
  }
  /* status */
  if (T->status) {
    runCheck(s, "git status", " ", T->verbose);
  }

  /* delete branch */
  if (T->deleteBranch) {
    runCheck(s, "git branch -D ", opt_branch, T->verbose);
  }

  /* === User repo creation ===*/
  if (T->mode && T->repoName && T->repoDes) {
    int curlcheck = getdir("/usr/bin/", "curl");
    addCmdRepo(curlcheck, T->verbose);
  }

  /* origin */
  if (T->origin) {
    char stemp[200];
    sprintf(stemp,
            "git remote add origin git@github.com:%s && git branch -M "
            "main && git push -u origin main",
            opt_origin);
    runCheck(s, stemp, "", T->verbose);
  }

  /* merge */
  if (T->merge) {
    runCheck(s, "git merge ", opt_branch, T->verbose);
  }

  /* ----- end of single checks ------ */

  /* init trip */
  if (T->init) {
    input("cd $(pwd) && git init ");
  }

  /* commit trip */
  if (T->commit) {
    if (T->init) {
      if (T->add) {
        runNcheck("&& git add ", opt_add, " && git commit ");
      } else {
        input("&& git add . && git commit -m ");
      }
    } else {
      if (T->add) {
        runNcheck("git add ", opt_add, " && git commit -m ");
      } else {
        input("git add . && git commit -m ");
      }
    }
  }

  /* only add trip */
  if (T->add && !T->commit) {
    if (T->init) {
      runNcheck("&& git add ", opt_add, " ");
    } else if (!T->init) {
      runNcheck("git add ", opt_add, " ");
    }
    end(s, T->verbose);
  }

  /* message trip */
  if (T->message && T->commit) {
    runNcheck("'", opt_message, "'");
    // input(opt_message);
  } else if (!T->message && T->commit) {
    input(" 'commit made' ");
  }

  if (T->push) {
    input(" && git push");
  }
  end(s, T->verbose);
}

// must alias
// git log, git status ...
//  -- = reverse : git reverse commit id
//  -- = git diff srcb curb : view cahnges before mergeing
//  -- = git log --summary : view evertyhing
//  -- = git add -A : add only new files
//  -- = list the branches
//  -& 'args'       add extra aguments
//  -- 'id'         make a reverse to a commit id
//  --              a git dif
//  --              view your branches
