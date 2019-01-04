#include "myShell.h"

int main() {
  Shell shell;
  string line;
  while (1) {  // loop stops when input is "exit" or getline reaches eof.
    shell.initialize();
    cout << "myShell:" << shell.cwd << "$ ";
    if (getline(cin, line)) {
      bool validInput = shell.parse(line);
      if (validInput == false) {  //invalid input: "", "   "
        continue;
      }
      else {
        if (shell.parsedInput[0] == "exit") {
          //instead of calling exit(), we break the loop to let the program exit gracefully by itself so that destructors are called to clean up.
          break;
        }
        bool exctnRequired = shell.noneExctnCmdHandle(shell.parsedInput[0]);
        if (exctnRequired == false) {  // execution not required
          continue;
        }
        else {  //require execution
          Command cmd(shell.parsedInput);
          bool backSlashFound = cmd.searchBackSlash();
          bool cmdNameFound = false;
          if (backSlashFound == false) {  // no '/' in input, search PATH
            cmdNameFound = cmd.findEnvPath(shell.EnvPath);
          }
          else {  // has '/' in input, check the validity of the given path
            cmdNameFound = cmd.findGivenPath();
          }
          cmd.execute(cmdNameFound);
        }
      }
    }
    else {  //getline eof
      break;
    }
  }
  return EXIT_SUCCESS;
}
