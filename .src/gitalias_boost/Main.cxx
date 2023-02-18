/* Copyright 2023 potato@c137 */
/* Gitalias re-writeen in cpp for long options as get_optlong did not meet requirements*/
#include <boost/program_options.hpp>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <boost/program_options/errors.hpp>
#include <boost/algorithm/string.hpp>

namespace opt = boost::program_options;
/* global varibales */
std::string programName,messagebox,addbox,branchbox,originbox,reponame,repodes,command="[[ -f /bin/git || -f /usr/bin/git ]] ";
std::vector<std::string>(addfiles);
struct Trips
{
  bool commit, message, add, init, branch, switch_, deleteBranch, merge, pull, push, origin,log, status, repoName, repoDes, mode, verbose;
};

/* ============== functions ================*/
void debugg(std::string e){
  std::cout << e << " was called \n";
}

/* ============== Main ============*/
int main(int argc, char *argv[]) {

if ( argc < 2){
  std::cerr << *argv << ": No arguments passed...\n\ttry -h for help\n";
  exit(1);
}
programName = *argv;
Trips *T = new Trips;
try
{
  /* with notifier we can call directly to fnuctions 
    add implicit values as default values trip no matter waht 
  */
  opt::options_description desc(std::string(*argv).append(" options"));
  desc.add_options()
      ("help,h", "print this message")
      ("i", "init a repository")
      ("c", "add all and commit")
      ("a",opt::value<std::vector<std::string>>(&addfiles),"add [files] only")
      ("m", opt::value<std::string>(&messagebox)->default_value("commit message")->notifier(&debugg), "add a message")
      ("b", opt::value<std::string>(&branchbox)->default_value(static_cast<std::string>("")), "create a branch")
      ("s",opt::value<std::string>(&branchbox), "switch to a branch")
      ("d", opt::value<std::string>(&branchbox), "delete a branch")
      ("e",opt::value<std::string>(&branchbox),"merge branch [ branch-name ] with current branch")
      ("pull,l",opt::value<std::string>(&originbox),"pull from origin")
      ("push,p",opt::value<std::string>(&originbox),"push into origin")
      ("verbose,v","print out parsed code")
      ("log,g","show log files")
      ("status,t","show statuts")
      ("origin,o",opt::value<std::string>(&originbox)->implicit_value(""),"add an origin")
      ("repo,r",opt::value<std::string>(&reponame),"name for creating an online repo * [i]")
      ("des,y",opt::value<std::string>(&repodes),"description for the online repo * [ii]")
      ("mode,u",opt::value<bool>(&T->mode)->default_value(true),"bool : true/false | if repo should be private *[iii]")
      ;

  opt::variables_map args;
  opt::store(opt::command_line_parser(argc, argv).options(desc).style(
    opt::command_line_style::default_style | opt::command_line_style::allow_long_disguise).run(), args);
  opt::notify(args);
  if (args.count("help"))
  {
    /* fixing the stream */
    std::stringstream stream;
    stream << desc;
    std::string helpMsg = stream.str();
    boost::algorithm::replace_all(helpMsg,"--","-");
    boost::algorithm::replace_all(helpMsg,"[ -","[ --");
    /* maybe not */
    std::cout << helpMsg << "\n";
    exit(0);
  }

  if (args.count("c")){
    debugg("c");
    T->commit = true;
  }
  if ( args.count("m")){
    debugg("m");
    T->message = true;
    if ( messagebox.length() < 0){
      messagebox += "commit made";
    }
  }
 if ( args.count("i")){
    debugg("i");
    T->init = true;
  }
  if( args.count("a") ){
    debugg("a");
    T->add = true;
  }
if( args.count("b") ){
    debugg("b");
    T->branch = true; 
  }

 if ( args.count("s")){

    debugg("s");
   T->switch_ = true;
 }
 if( args.count("d")){
    debugg("d");
   T->deleteBranch = true;
 }
 if( args.count("e")){
    debugg("e");
   T->merge = true;
 }
 if( args.count("pull")){
    debugg("l");
   T->pull = true;
 }
 if(args.count("push")){
    debugg("p");
   T->push = true;
 }
 if(args.count("verbose")){
    debugg("v");
   T->verbose = true;
 }
 if( args.count("log")){
    debugg("g");
   T->log = true;
 }
 if( args.count("status")){
    debugg("t");
   T->status = true;
 }
 if( args.count("origin") ){
    debugg("o");
    T->origin= true; 
  }
  if( args.count("repo") ){
    debugg("r");
    T->repoName= true; 
  }
  if( args.count("des") ){
    debugg("y");
    T->repoDes= true; 
  }
  if( args.count("mode") ){
    debugg("u");
    T->mode= true; 
  }
  exit(EXIT_SUCCESS);
}
catch (const opt::error &ex)
{
  std::cerr <<programName << ": " << ex.what() << "\n";
  exit(1);
}

}
