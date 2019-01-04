#include "myShell.h"

extern char ** environ;

//4

bool Command::searchBackSlash() {
  int i = 0;
  bool bsFound = false;
  while (commandName[i] != '\0') {
    if (commandName[i] == '/') {
      bsFound = true;
    }
    i++;
  }
  //DEBUG  cout << "backSlashfound is : " << bsFound << endl;
  return bsFound;
}

//5 set realPath to proper value

// The two functions below look similar, an abstraction is required. But since they are handling dirctory, class members, using functions on class members, it is easy to make mistakes. I left them seperate to avoid mistakes.

//5a
bool Command::findEnvPath(vector<string> & envP) {
  DIR * curr_dir;
  struct dirent * dir_ent;
  //iterator and open path vector
  for (vector<string>::iterator it = envP.begin(); it != envP.end(); ++it) {
    if ((curr_dir = opendir((*it).c_str())) == 0) {
      perror("Can't open directory\n");
      closedir(curr_dir);
      exit(EXIT_FAILURE);
    }
    else {
      while ((dir_ent = readdir(curr_dir)) != NULL) {
        string fileName(dir_ent->d_name);
        if (fileName == "." || fileName == "..") {
          continue;
        }
        else if (fileName == commandName) {
          if (envP.size() > 1) {
            realPath = *it + "/" + commandName;
            closedir(curr_dir);
            return true;
          }
          else {
            realPath = *it + commandName;
            closedir(curr_dir);
            return true;
          }
        }
      }
      closedir(curr_dir);
    }
  }
  cout << "Command " << commandName << " not found" << endl;
  return false;
}

//5b
bool Command::findGivenPath() {
  commandName = divideNamePath(2);
  realPath = divideNamePath(0);
  DIR * curr_dir;
  struct dirent * dir_ent;
  if ((curr_dir = opendir(divideNamePath(1).c_str())) == 0) {
    perror("Can't open directory\n");
    closedir(curr_dir);
    exit(EXIT_FAILURE);
  }
  else {
    while ((dir_ent = readdir(curr_dir)) != NULL) {
      string fileName(dir_ent->d_name);
      if (fileName == "." || fileName == "..") {
        continue;
      }
      else if (fileName == commandName) {
        closedir(curr_dir);
        return true;
      }
    }
    closedir(curr_dir);
  }
  cout << "Command " << commandName << " not found" << endl;
  return false;
}

//5b.1  seperate /home/bin/ls to /home/bin/ and ls
string Command::divideNamePath(int s) {
  if (s == 0) {  // no action
    return cmd_args[0];
  }
  int rear = cmd_args[0].length() - 1;
  int k = rear;
  while (cmd_args[0][k] != '/') {
    k--;
  }  // k is the index of the last /
  string temp(cmd_args[0]);
  if (s == 1) {  // return commandPath
    return temp.substr(0, k + 1);
  }
  if (s == 2) {  // return commandName
    return temp.substr(k + 1, rear - k);
  }
  return temp;
}

//6 fork, redirection, execution

void Command::execute(bool s) {
  //DEBUG cout << "cName " << commandName << endl;
  //DEBUG cout << "realPath " << realPath << endl;
  if (s == true) {  //command found, should execute
    pid_t pid, w;
    int status;
    pid = fork();
    if (pid == 0) {
      if (cmd_args.size() > 2) {
        rdScanner();
      }
      vector<char *> args;  //transfer a vector of string to vector of char*
      for (size_t i = 0; i < cmd_args.size(); ++i) {
        args.push_back(const_cast<char *>(cmd_args[i].c_str()));
      }
      args.push_back(NULL);  //required by execve()
      if (execve(realPath.c_str(), &args[0], environ) == -1) {
        perror("execve");
      };
    }
    else if (pid < 0) {
      perror("fork");
      exit(EXIT_FAILURE);
    }
    else {
      do {
        w = waitpid(pid, &status, WUNTRACED | WCONTINUED);
        if (w == -1) {
          perror("waitpid");
          exit(EXIT_FAILURE);
        }
        if (WIFEXITED(status)) {
          printf("Program exited with status %d\n", WEXITSTATUS(status));
        }
        else if (WIFSIGNALED(status)) {
          printf("Program was killed by signal %d\n", WTERMSIG(status));
        }
      } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
    return;
  }
  else if (s == false) {
    return;
  }
}

//6.1 scan to find the sign and file
void Command::rdScanner() {
  //DEBUG cout << "I am in the scanner\n";
  for (unsigned long i = 0; i < cmd_args.size(); i++) {
    if (cmd_args[i] == ">" || cmd_args[i] == "<" || cmd_args[i] == ">>" || cmd_args[i] == "2>" ||
        cmd_args[i] == "2>>") {
      //check i+1
      if (i == cmd_args.size() - 1) {
        return;
      }
      rdSelector(cmd_args[i], cmd_args[i + 1]);
      cmd_args.erase(cmd_args.begin() + i);  // remove the sign as it is no longer an argument
      cmd_args.erase(cmd_args.begin() + i);  // remove the destination file, too
      i--;  // at the end of this loop, i will inc by 1, which means it jumps over the next argument, so i-- to prevent this.
    }
  }
  return;
}

//6.1.1 select the parameters to pass in to "open"
void Command::rdSelector(string sign, string pathname) {
  int filecreation;
  mode_t mode;
  int origFd;
  if (sign == "<") {
    filecreation = O_RDONLY;
    origFd = STDIN_FILENO;
    //DEBUG    cout << "READIN" << endl;
    r_redirect(pathname, filecreation, origFd);
    return;
  }
  if (sign == ">") {
    filecreation = O_WRONLY | O_CREAT;
    mode = S_IRWXU;
    origFd = STDOUT_FILENO;
  }
  if (sign == ">>") {
    filecreation = O_WRONLY | O_CREAT | O_APPEND;
    mode = S_IRWXU;
    origFd = STDOUT_FILENO;
  }
  if (sign == "2>") {
    filecreation = O_WRONLY | O_CREAT;
    mode = S_IRWXU;
    origFd = STDERR_FILENO;
  }
  if (sign == "2>>") {
    filecreation = O_WRONLY | O_CREAT | O_APPEND;
    mode = S_IRWXU;
    origFd = STDERR_FILENO;
  }
  //DEBUG cout << "In rdSelector: " << pathname << " " << filecreation << " " << origFd << " " << endl;
  w_redirect(pathname, filecreation, mode, origFd);
}

//6.1.1a  write out
void Command::w_redirect(string pathname, int filecreation, mode_t mode, int origFd) {
  int fd = open(pathname.c_str(), filecreation, mode);
  if (fd == -1) {
    perror("open");
    return;
  }
  dup2(fd, origFd);
  close(fd);
}

//6.1.1b read in
void Command::r_redirect(string pathname, int filecreation, int origFd) {
  int fd = open(pathname.c_str(), filecreation);
  if (fd == -1) {
    perror("open");
    return;
  }
  dup2(fd, origFd);
  close(fd);
}
