/* Copyright 2023 potato */
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
/* global variables */
char command[2048];
char *optarg, *opt_message, *opt_branch, *opt_origin, *opt_add, *opt_user,
    *opt_repoName, *opt_description, *githubCreate;

struct trip {
  int init, commit, add, message, branch, switch_, merge, pull, push, origin,
      log, status, deleteBranch, repoName, repoDes, User;
};

/* functions Order matters */
int getdir(char *place, char *thing);
void print_format(FILE *std, int exit_v, char *exe_name);
void input(char *s2);
void end(char *ss);
void addCmdRepo(int a);
/* they might be the same but they arent */
void runNcheck(char *s1, char *s2, char *s3);
void runCheck(char *mainstr, char *s1, char *s2, int gitdir);
/* ====================================== */
void run(struct trip *T, char *s);

/* main function */
int main(int argc, char *argv[]) {
  if (argc == 1) {
    fprintf(stderr, "%s : Invalid option passed\n", *argv + 0);
    print_format(stderr, 1, *argv + 0);
  }

  /* local variables */
  int opt, optind;
  struct trip ntrip = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  /*if an ignore variable is given
   * we make check true, run normally
   * so as to no create a hell
   *
   * */
  while ((opt = getopt(argc, argv, ":hica:m:b:s:lpo:ve:gd:r:u:y:t")) != -1) {
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
        fprintf(stderr, "%s\n", "ERR: cannot used -d && -s together");
        exit(1);
      }
      ntrip.branch = 1;
      opt_branch = optarg;
      break;
    case 's':
      // switch branch
      if (ntrip.branch) {
        fprintf(stderr, "%s\n", "ERR: cannot used -d && -s together");
        exit(1);
      }
      opt_branch = optarg;
      ntrip.switch_ = 1;
      break;
    case 'l':
      // pull
      if (ntrip.push) {
        fprintf(stderr, "%s\n", "ERR: cannot used -l && -p together");
        exit(1);
      }
      ntrip.pull = 1;
      break;
    case 'p':
      // push
      if (ntrip.pull) {
        fprintf(stderr, "%s\n", "ERR: cannot used -l && -p together");
        exit(1);
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
      fprintf(stdout, "%s\n", "thinking of what -v should do");
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
      ntrip.User = 1;
      opt_user = optarg;
      break;
    case 'y':
      // repo : description
      ntrip.repoDes = 1;
      opt_description = optarg;
      break;
    case ':':
      // errors
      fprintf(stderr, "option  needs a value\n\tTry -h for help\n");
      exit(EXIT_FAILURE);
      break;
      // command not found
    default:
      fprintf(stderr, "%s : Invalid option passed\n\tTry -h for help\n",
              argv[0]);
      exit(EXIT_FAILURE);
    }
  }

  /* extra checks before running main logic */
  int prechecker = ntrip.push + ntrip.pull + ntrip.merge + ntrip.origin +
                   ntrip.switch_ + ntrip.branch + ntrip.status + ntrip.log;

  int repochecker = ntrip.repoName + ntrip.repoDes + ntrip.User;
  if (prechecker > 1 || (repochecker == 2 || repochecker == 1)) {
    fprintf(stderr, "%s : Invalid options used together\n", *argv + 0);
    exit(EXIT_FAILURE);
  }
  /* === end of checks === */

  /* main logic */
  run(&ntrip, command);
  if (optind >= argc) {
    fprintf(stderr, "Expected argument after options\n");
    exit(EXIT_FAILURE);
  }

  exit(EXIT_SUCCESS);
}

/* end of main function */

/* functions */
/* getdir : looks in current folder to see if it has a git repo in it */
int getdir(char *place, char *thing) {
  DIR *d;
  struct dirent *dir;
  d = opendir(place);
  if (d) {
    while ((dir = readdir(d)) != NULL) {
      if (strcmp(dir->d_name, thing) == 0) {
        closedir(d);
        return 1;
      }
    }
  }
  return 0;
}

/* print_format : prints the help message */
void print_format(FILE *std, int exit_v, char *exe_name) {
  fprintf(std, "%s : %s [ %s... ]\n", "Usage", exe_name, "options");
  fprintf(std,
          "-h              print this message *\n"
          "-i              initialize a repository\n"
          "-c              add and commit\n"
          "-m 'message'    add a message\n"
          "-b 'branch'     create a branch *\n"
          "-s 'branch'     switch to a branch *\n"
          "-o 'user/repo'  add an origin *\n"
          "-l              pull from repo *\n"
          "-p              push to repo *\n"
          "-v              NULL *\n"
          "-g              show git log files *\n"
          "-t              show git status *\n"
          "-d 'branch'     delete a branch *\n"
          "\n+++ These features do not seem to work +++\n"
          "-r 'repo_name'  create an online repo[ the name of your new repo ]\n"
          "-u 'user_name'  needed to create online repo[ your user name ]\n"
          "-y 'des'        needed to create an online repo[ the description ]\n"
          " +++ fixing it up +++\n"
          "\nFeatures to be added soon\n"
          " -&& 'args'      add extra aguments\n"
          " -r 'id'         make a reverse to a commit id\n"
          " --              a git diff\n"
          " --              view your branches\n"
          " -- 'bool'       help the commit without an initialization\n"
          "\nThis is just a simple alias for git\n"
          "all options that end with [ * ] denotes they must be used alone\n"
          "\nExample:\n");

  fprintf(std,
          "%s -i -c -m 'initial commit'\n"
          "initialize a git repo, add all files and commit with the message "
          "'initial commt'\n",
          exe_name);
  // verbose
  fprintf(std,
          "\nSide Notes:\n"
          "Can : commit a repository without initializing it, it will "
          "automatically do it\nExample:\n"
          "\n\t%s -c -m \"This is a commit made in an uninitialized folder\"\n"
          "But this has it own bugs as if you were in a sub-folder it would "
          "not be able to tell\n"
          "Hence the need for another option to let it know if you your "
          "already in a git folder\n"
          "But due to specifing this always, i will just remove it, feels like "
          "a bigger issue, than just specifing once\n"
          "Can : not specify the files to add, it will automatically add all\n"
          "Can : create an online github repository, but all 3 options [-r -u "
          "-y ]should be filled [ This feature has be outdated, but working on "
          "it ]\n"
          "more Notes will be made\n",
          exe_name);

  exit(exit_v);
}
// sooon
//  r = reverse : git reverse commit id
//  d = git diff srcb curb : view cahnges before mergeing
//  v = git log --summary : view evertyhing
//  git add -A : add only new files
//  create a github repo here
// list the branches
/* input: puts into command */
void input(char *s2) {
  if (s2 != NULL)
    strcat(command, s2);
}

/* passes the string ss into system() and exits */
void end(char *ss) {
  if (ss != NULL) {
    system(ss);
    exit(EXIT_SUCCESS);
  } else {
    fprintf(stderr, "ERR: NULL value passed in\n");
    exit(EXIT_FAILURE);
  }
}

/* runNcheck : this function takes in 3 strings s1,s2,s3 and if none is NULL
 * will concat it to the string command */
void runNcheck(char *s1, char *s2, char *s3) {
  if (s1 != NULL && s2 != NULL && s3 != NULL) {
    input(s1);
    input(s2);
    input(s3);
  }
}

void addCmdRepo(int a) {
  /* strings we need */
  char crepo[500] = {'\0'};
  if (a == 1) {
    sprintf(
        crepo,
        "curl '%s' https://api.github.com/%s/repos -d "
        "'{\"name\":\"%s\",\"description\":\"%s\"}' && git remote add origin "
        "git@github.com:%s/%s.git && git branch -M main && git push -u "
        "origin main",
        opt_user, opt_user, opt_repoName, opt_description, opt_user,
        opt_repoName);
    /*probably read token from a file
    curl \
  -X POST \
  -H "Accept: application/vnd.github+json" \
  -H "Authorization: Bearer <YOUR-TOKEN>"\
  -H "X-GitHub-Api-Version: 2022-11-28" \
  https://api.github.com/user/repos \
  -d '{"name":"Hello-World","description":"This is your first
  repo!","homepage":"https://github.com","private":false,"is_template":true}'
  */
    end(crepo);
  } else {
    fprintf(stderr, "ERR : %s\n%s", "The application curl is needed",
            "Install to use this feature");
    exit(1);
  }
}

/*runCheck : This function takes in 3 strings mainstr,s1,s2  and int gitdir
 * it treats the first string as the mainstring to concat the other 2 strings
 * into s1 will flush mainstr with it's contents and s2 will concat to mainstr
 * after the flush if gitdir is 1 this operation will run, else it will not
 * */
void runCheck(char *mainstr, char *s1, char *s2, int gitdir) {
  if (gitdir) {
    strcpy(mainstr, s1);
    strcat(mainstr, s2);
    end(mainstr);
  } else {
    fprintf(stderr, "%s :  %s\n", "ERR",
            "Git repository not found in current location");
  }
  exit(EXIT_FAILURE);
}

void run(struct trip *T, char *s) {
  int check = getdir(".", ".git");

  /* ======================= Stuff that needs to run once */

  /* branch */
  if (T->branch) {
    runCheck(s, "git branch ", opt_branch, check);
  }

  /* switch */
  if (T->switch_) {
    runCheck(s, "git switch ", opt_branch, check);
  }

  /* Pull */
  if (T->pull) {
    runCheck(s, "git pull", " ", check);
  }

  /* push */
  if (T->push && !T->commit) {
    runCheck(s, "git push", " ", check);
  }

  /* push */
  if (T->log) {
    runCheck(s, "git log", " ", check);
  }
  /* status */
  if (T->status) {
    runCheck(s, "git status", " ", check);
  }

  /* delete branch */
  if (T->deleteBranch) {
    runCheck(s, "git branch -D ", opt_branch, check);
  }

  /* === User repo creation ===*/
  if (T->User && T->repoName && T->repoDes) {
    int curlcheck = getdir("/usr/bin/", "curl");
    addCmdRepo(curlcheck);
  }

  /* origin */
  if (T->origin) {
    char stemp[200];
    sprintf(stemp,
            "git remote add origin git@github.com:%s && git branch -M "
            "main && git push -u origin main",
            opt_origin);
    runCheck(s, stemp, "", check);
  }

  /* merge */
  if (T->merge) {
    runCheck(s, "git merge ", opt_branch, check);
  }

  /*==== end of stuff ========================= */
  /* init trip */
  if (T->init) {
    if (check) {
      fprintf(stdout, "%s\n", "git repo already initialized, skipping...\n");
      T->init = 0;
    } else {
      input("cd $(pwd) && git init ");
    }
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
      if (check) {
        if (T->add) {
          runNcheck("git add ", opt_add, " && git commit -m ");
        } else {
          input("git add . && git commit -m ");
        }
      } else {
        if (T->add) {
          runNcheck("git init && git add ", opt_add, " && git commit -m ");
        } else {
          input("git init && git add . && git commit -m ");
        }
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
    end(s);
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
  end(s);
}

// must alias  git log, git status ...
