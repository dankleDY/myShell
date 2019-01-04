#ifndef __SHELL_H__
#define __SHELL_H__
#include <dirent.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

extern char ** environ;

/*

README
myShell has two classes, Shell and Command. The comment numbers behind function indicates their execution sequences.
For example, func 1.1(getEnvPath) is called by func 1(initialize).
More details are included in command.cpp and shell.cpp

*/

class Shell
{
 private:
  map<string, string> myVarMap;  //hold variables

  void getEnvPath();                 // 1.1
  void varMapInit(char ** environ);  // 1.2

  string replaceVarNameWithValue(string & temp);  // 2.1
  bool mapFindValue(string VarName);              // 2.1.1 + 3.1

  void do_cd();                     // 3.1a
  void do_set_or_inc(string line);  // 3.1b
  void do_export(string Var);       // 3.1c
  bool varNameCheck();              // 3.1b.1

 public:
  string cwd;
  vector<string> parsedInput;
  vector<string> EnvPath;  //hold chopped PATH variables
  Shell() : myVarMap(), cwd(getenv("PWD")), parsedInput(), EnvPath(){};
  void initialize();                             // 1
  bool parse(string & line);                     // 2
  bool noneExctnCmdHandle(const string & line);  // 3
};

class Command
{
 private:
  string realPath;  //to execve, we need a valid path

  string divideNamePath(int s);                                                 // 5b.1
  void rdScanner();                                                             // 6.1
  void rdSelector(string sign, string pathname);                                // 6.1.1
  void w_redirect(string pathname, int filecreation, mode_t mode, int origFd);  // 6.1.1.1a
  void r_redirect(string pathname, int filecreation, int origFd);               // 6.1.1.1b

 public:
  string commandName;
  vector<string> cmd_args;  //same as parsedInput
  Command(vector<string> commandArgs) :
      realPath(),
      commandName(commandArgs[0]),
      cmd_args(commandArgs){};
  bool searchBackSlash();                   // 4
  bool findEnvPath(vector<string> & envP);  // 5a
  bool findGivenPath();                     // 5b
  void execute(bool s);                     // 6
};

#endif
