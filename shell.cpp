#include "myShell.h"

using namespace std;
extern char ** environ;

//1 Initialization
void Shell::initialize() {
  EnvPath.clear();
  parsedInput.clear();
  getEnvPath();
  varMapInit(environ);
}

//1.1 Put PATH into EnvPath vector
void Shell::getEnvPath() {
  char * localPath = getenv("PATH");
  char * str = (char *)malloc(strlen(localPath) + 1);
  strcpy(str, localPath);
  char * tempPath;
  tempPath = strtok(str, ":");
  while (tempPath != NULL) {
    string a(tempPath);
    EnvPath.push_back(a);
    //Then strtok expects a NULL pointer so that it can use the previous position to keep splitting the string
    tempPath = strtok(NULL, ":");
  }
  free(str);
}

//1.2 Put all environment variables into myVarMap
void Shell::varMapInit(char ** environ) {
  for (size_t i = 0; environ[i] != NULL; i++) {
    string temp(environ[i]);
    int len = strlen(environ[i]);
    for (int k = 0; k < len; k++) {
      if (temp[k] == '=') {
        string name = temp.substr(0, k);
        string value = temp.substr(k + 1, len - k - 1);
        myVarMap.insert(pair<string, string>(name, value));
      }
    }
  }
}

// 2  Parse user input, handle escape and $ expansion.Store input.
bool Shell::parse(string & line) {
  if (line == "") {
    return false;
  }
  while (line[0] == ' ') {  //remove white spaces
    line.erase(0, 1);
  }
  if (line == "") {
    return false;
  }
  int i = 0;
  int k = 0;
  while (line[i] != '\0') {
    while (line[i] == ' ') {
      i++;
    }  //find the first character
    if (line[i] == '\0') {
      return true;
    }       //a loop in a loop, i could exceed boundary
    k = i;  //mark
    while (line[i] != ' ' && line[i] != '\0') {
      if (line[i] == '\\') {  // handle escape
        line.erase(i, 1);
      }
      if (line[i] == '\0') {
        return false;
      }
      i++;
    }  //i at the end of a substring
    string temp = line.substr(k, i - k);
    //DEBUG  cout << "now parsed:" << temp << endl;
    temp = replaceVarNameWithValue(temp);
    //DEBUG  cout << "now replaced:" << temp << endl;
    parsedInput.push_back(temp);
  }
  return true;
}

//2.1 $ expansion
string Shell::replaceVarNameWithValue(string & notreplaced) {
  string replaced = notreplaced;
  unsigned long j = 0;
  unsigned long m = 0;
  while (j < replaced.length() - 1) {  // if the last word  we don't care
    string VarName = "";
    if (replaced[j] == '$') {
      j++;
      m = j;  //the beginning index of variable name
      while (replaced[j] != '$' && replaced[j] != '-' && replaced[j] != '\0') {
        j++;
      }  // j-m is the length of the name
      if (replaced[j] == '-') {
        replaced.erase(j, 1);
      }
      VarName = replaced.substr(m, j - m);
      // cout << "replaced: " << replaced << endl;
      //     cout << "VarName is:" << VarName << endl;
    }
    if (VarName != "") {
      bool varFound = mapFindValue(VarName);
      if (varFound == true) {  // $ found and VarName found
        string value = myVarMap.find(VarName)->second;
        string name = '$' + VarName;
        replaced.replace(replaced.find(name), name.length(), value);
        j -= 1;  //avoid jump over next character
      }
      else {  //VarName not found
        m = j;
        j -= 1;  //avoid jump over
      }
    }
    j++;
  }
  return replaced;
}

//2.1.1
bool Shell::mapFindValue(string VarName) {
  map<string, string>::iterator it;
  it = myVarMap.find(VarName);
  if (it != myVarMap.end()) {
    return true;
  }
  return false;
}

//3 handle cd/set/inc/export

bool Shell::noneExctnCmdHandle(const string & line) {
  if (line == "") {
    return false;
  }
  if (line == "cd") {  //cd
    do_cd();
    return false;
  }
  if (line == "set") {
    if (parsedInput.size() != 3) {
      cout << "Set needs 2 arguments, you have:" << parsedInput.size() - 1 << endl;
    }
    else {
      do_set_or_inc(line);
    }
    return false;
  }
  if (line == "inc") {
    if (parsedInput.size() != 2) {
      cout << "Inc needs 1 arguments, you have:" << parsedInput.size() - 1 << endl;
    }
    else {
      do_set_or_inc(line);
    }
    return false;
  }
  if (line == "export") {
    do_export(parsedInput[1]);
    return false;
  }
  return true;
}

//3.1a
void Shell::do_cd() {
  if (parsedInput.size() != 2) {
    cout << "Cd needs 1 argument, you have:" << parsedInput.size() - 1 << endl;
  }
  else {  //change cwd
    if (chdir((parsedInput[1]).c_str()) != 0) {
      perror("chdir() failed:");
    }
    else {  //update cwd/prompt
      size_t a = 1024;
      char array[1024];
      getcwd(array, a);
      cwd = array;
    }
  }
}

//3.1b
void Shell::do_set_or_inc(string line) {
  bool varNameLegal = varNameCheck();
  if (varNameLegal == false) {
    return;
  }
  bool varFound = mapFindValue(parsedInput[1]);
  if (line == "set") {
    if (varFound == true) {  //if found, update
      myVarMap.find(parsedInput[1])->second = parsedInput[2];
      cout << "Variable " << parsedInput[1] << " = " << parsedInput[2] << " set" << endl;
    }
    else {  //if not, insert
      myVarMap.insert(pair<string, string>(parsedInput[1], parsedInput[2]));
      cout << "Variable " << parsedInput[1] << " = " << parsedInput[2] << " set" << endl;
    }
  }
  if (line == "inc") {
    if (varFound == true) {  //if found, update
      char * p;
      long converted = strtol(myVarMap.find(parsedInput[1])->second.c_str(), &p, 10);
      if (*p) {
        myVarMap.find(parsedInput[1])->second = "1";
        cout << "Variable " << parsedInput[1] << " inced to " << 1 << endl;
      }
      else {
        converted += 1;
        stringstream ss;
        ss << converted;
        string newValue = ss.str();
        myVarMap.find(parsedInput[1])->second = newValue;
        cout << "Variable " << parsedInput[1] << " inced to " << converted << endl;
      }
    }
    else {  //if not, insert
      myVarMap.insert(pair<string, string>(parsedInput[1], "1"));
    }
  }
}

//3.1b.1
bool Shell::varNameCheck() {
  for (unsigned long i = 0; i < parsedInput[1].length(); i++) {
    char a = parsedInput[1][i];
    if (!isalpha(a) && (!isdigit(a)) && a != '_') {
      cout << "Variable name cannot contain:" << a << endl;
      return false;
    }
  }
  return true;
}

//3.1c
void Shell::do_export(string Var) {
  if (parsedInput.size() != 2) {
    cout << "Export needs 1 arguments, you have:" << parsedInput.size() - 1 << endl;
    return;
  }
  bool varNameFound = mapFindValue(Var);
  if (varNameFound == true) {
    if (setenv(Var.c_str(), myVarMap.find(Var)->second.c_str(), 1) == 0) {
      cout << "Variable:" << Var << " = " << myVarMap.find(Var)->second << " exported" << endl;
    }
    else {
      perror("setenv");
    }
  }
  else {
    cout << "Variable " << Var << " doesn't have a value to export" << endl;
  }
  return;
}

/* DEBUG
  for (map<string, string>::const_iterator it = myVarMap.begin(); it != myVarMap.end(); ++it) {
    cout << it->first << " : " << it->second << endl;
  }
*/
