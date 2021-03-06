Using and Testing myShell

Nibo Ying  12/04/2018   ny38

note: In this file, "abc" means the command or the output is abc, quotation marks are not part of the command or output.

Welcome to my baby shell. Please start myShell by typing 
    "./myShell" or "valgrind ./myShell"

Then you should see a prompt like this on the screen:
    "myShell:/home/ny38/ece551/mp_miniproject$"

Between "myShell:" and "$" is the current working directory.

Now we can start testing myShell with crazy commands.

(1) Run:     "ls"

    The program prints names of all the files in the current directory and print 
    "Program exited with status 0" at the end. 
    This is correct because the program successfully executes ls, and lists all files in the current directory. 
    The last sentence indicates "ls" program succeeds and exited without error.This sentence will show up as long as the program called "execve" to executes a command. 

(2) Run:     "env"

    The program prints all the environment variables.
    "Program exited with status 0" at the end. (I will omit this in the following cases)
    This is correct because env is executed. 

(3) Run:     "/bin/ls"

    The program again prints the names of files.
    This is correct because an absolute path command "/bin/ls"is executed.

(4) Run:     "./hello"

    The program prints "hello world". 
    This is correct because a relative path command "./hello" is executed.
   
(5) Run:     ""  (no input, just hit "enter")

    The program prints nothing but the prompt.
    This is correct because there is no input.
    
(6) Run:     "xyz"

    The program prints "Command xyz not found".
    This is correct because myShell interprates "xyz" as a command name and failed to find a such a command.
    
(7) Run:     "123"

    The program prints "Command 123 not found".
    Same as the last case.
    
(8) Run:     "!$%"

    The program prints "Command !$% not found".
    Same as the last case.

(9) Run:     "xyz123!$"

    The program prints "Command xyz123!$ not found".
    Same as the last case.

(10) Run:     "Ls"

    The program prints "Command Ls not found".
    This is correct because myShell is case-sensitive.
    If a user mistakely inputs upper case commands, myShell will print "Command not found".

    
(11) Run:     "    ls     "   (some spaces before and after "ls")

    The program again prints all names of files.
    This is correct because the program filters white spaces and successfully interprates the input as "ls". 
    
(12) Run:     "   cat     hello.txt"

    The program prints "hello world".
    Double check white spaces handle.

(13) Run:     "echo \"

    The program prints "\".
    This is correct because myShell interprates "\" as a common input if it is followed by nothing.

(14) Run:     "echo \\"

    The program prints "\".
    This is correct because the first "\" helps the second "\" to escape.

(15) Run:     "echo a\ b"

    The program prints "a b".
    This is correct because the white space behind "\" is escaped.
    
(16) Run:     "echo a\ b"

    The program prints "a b".
    This is correct because the white space behind "\" is escaped.
    
(17) Run:     "echo c   d"  (three white spaces between "c" and "d")

    The program prints "c d". (One white space in between)
    This is correct because echo takes two parameters "c" and "d" and prints them seperately.
    This white spaces are added by echo.

(18) Run:     "echo c\ \ \ d"

    The program prints "c   d". (Three spaces between "c" and "d")
    This is correct because all three spaces are escaped.
    These white spaces are not added by echo.
    
(19) Run:     "echo hello\ worl\d"

    The program prints "hello world"
    This is correct because "\" makes sure the character behind it is escaped.
    
(20) Run:     "echo hello\\       world"

    The program prints "hello\ world"
    This is correct because "\" makes sure the character behind it is escaped.
    Thus all the white spaces are omited and echo has two parameters "hello\" and "world".
    
(21) Run:     "cd testDir"

    The program prints nothing but a new prompt.
    It looks like this "myShell:/home/ny38/ece551/mp_miniproject/testDir$".
    This is correct because cd is executed.
    
(22) Run:     "ls"

    The program prints "IamtestDir.txt"
    This is correct because now this is the only file in this directory.
    
(23) Run:     "cd .."

    The program prints our former prompt so that we can keep testing.
    This case is optional.

(24) Run:     "set"

    The program prints "Set needs two arguments, you have: 0".
    This is correct and the reason is explained.

(25) Run:     "set a 1"
    Then    "inc a"

    The program prints "Variable a = 1 set" and "Variable a inced to 2".
    The sentence "Program exited with status 0" will not show up.
    This is correct because myShell will give you a response to show you that a variable is set. 
    Also, "set" and "inc" are built-in functions, thus do not use "execve".
    
(26) Run:     "set b 0"
    Then    "inc b"

    The program prints "Variable b = 0 set" and "Variable b inced to 1".
    This is correct because myShell can handle 0.
    
(27) Run:     "set c -1"
    Then    "inc c"
    The program prints "Variable c = -1 set" and "Variable c inced to 0"
    This is correct because myShell can handle negative integers.
    
(28) Run:     "set d -10000"
    Then    "inc d"
    The program prints "Variable d = -10000 set" and "Variable d inced to -9999"
    This is correct because myShell can handle negative integers.

(29) Run:     "set d 0.05"
    
    The program prints "Variable d = 0.05 set".
    This is correct because myShell regards all values as strings by default.
    When the user wants to inc a variable, myShell then interperate that variable into a number.
    So if you run    "inc d"
    The program prints "Variable d inced to 1".

(30) Run:     "set d -000123"
    
    The program prints "Variable d = -000123 set".
    This is correct because, again, myShell regards all values as strings by default.
    When the user wants to inc a variable, myShell then interperate that variable into a number.
    So if you run    "inc d"
    The program handles all the 0s and prints "Variable d inced to -122".

(31) Run:     "set e hello world"

    The program prints "Set needs 2 arguments, you have: 3"
    This is correct because "hello world" is interprated as two different arguments.
    
(32) Run:     "set e helloworld"

    The program prints "Variable e = helloworld set"
    This is correct now.

(33) Run:     "set _a1 1"

    The program prints "Variable _a1 = 1 set"
    This is correct because _a1 is a valid variable name.

(34) Run:     "set qwe@12!3 1"

    The program prints "Variable name cannot contain @"
    This is correct because qwe@12!3 is a valid variable name. MyShell reports the first invalid sign it sees, in this case "@".

(35) Run:     "export h"

    The program prints "Variable h doesn't have a value to export".
    This is correct because we have not set a value for h yet.
    If you export a illegal name, you will see the same output.

(36) Run:     "set h $a"

    The program prints "Variable h = 2 set"
    This is correct because "$a" is replaced by its value, which is 2.
    
(37) Run:     "expot h"

    The program prints "Command expot not found".
    This is correct because "expot" is a typo. :) 

(38) Run:     "export"

    The program prints "Export needs 1 variable, you have: 0".
    This is correct. 

(39) Run:     "export h"

    The program prints "Variable h = 2 exported"
    Now we input the right command and it worked!
    
(40) Run:     "env"

    The program prints all environment variables, "h=2" included.
    This is correct because export is executed.

(41) Run:     "echo $h-"

    The program prints 2.
    This is correct because "-" is used to mark the name of the variable to expand. 
    So this command equals to "echo $h".

(42) Run:     "echo "1$h-3"

    The program prints "123".
    This is correct because for the same reason.

(43) Run:     "echo "1$h-33$h-1"

    The program prints "123321".
    This is correct because for the same reason.

(44) Run:     "echo "1$h  3$h-1"

    The program prints "12 321".
    This is correct because white space also marks the end of "$" expansion.

(45) Run:     "echo $hh"

    The program prints "$hh".
    This is correct because "hh" is not set as a variable. 
    So myShell's handle is to interprate "$hh" as a normal string. 

(46) Run:     "echo 1$h3"

    The program prints "1$h3".
    This is correct because for the same reason.

(47) Run:     "echo $"

    The program prints "$".
    This is correct because for the same reason.

(48) Run:     "set USER aaa"
    Then    "env"

    The program prints "Variable PATH = aaa set". And you see in env, PATH is unchanged.
    This is correct because we have not exported PATH yet.
    
(49) Run:     "export USER"
    Then    "env"
    
    Now you see USER is changed to "aaa".

    In addition: 
    If you run :  "set PATH aaa", "export PATH", "ls"   myShell exits with a message "Can't open directory".
    This is correct because you changed PATH to "aaa" which is not a directory thus "opendir" function complains and exits the whole program.
    If you did run these commands, you will need to run "./myShell" again to continue testing.
    
(50) Run:     "set i hello.txt"
    Then    "cat $i"

    The program prints "Variable i = hello.txt set" and then "hello world".
    This is correct because $i is replaced with hello.txt.
    
(51) Run:     "<"

    The program prints "Command < not found".
    This is correct because "<" is not a command.

(52) Run:     "< hello.txt"

    The program prints "Command < not found".
    This is correct for the same reason.
    
(53) Run:     "cat <"

    The program prints "cat: '<': No such file or directory" and program exited with status 1.
    This is correct because myShell can't recognize redirections when the redirection sign is the last valid input.

(54) Run:     "./hello > out.txt"
    Then    "cat out.txt"

    The program prints "Program exited with status 0" then "hello world" and "Program exited with status 0".
    This is correct because the output is redirected to out.txt.
    
(55) Run:     "./hello >> out.txt"
    Then    "cat out.txt"

    The program prints "Program exited with status 0" then two lines of "hello world".
    This is correct because ">>" not only redirectes output, but also "appends" it to the original text.
    
(56) Run:     "cat xyz > errout.txt"

    The program prints "cat: xyz: No such file or directory". The error message is not redirected.
    This is correct because ">" cannot redirect error messages. However, it does creates a file called "errout.txt".
    If you cat errout.txt, you will see it is empty.
    
(57) Run:     "cat xyz 2> errout.txt"

    The program prints only "Program exited with status 1".
    This is correct because this time error messages are directed to errout.txt by "2>".
    If you cat errout.txt, you will see the error message.

(58) Run:     "cat xyz 2>> errout.txt"

    As you can imagine, it appends error messages to errout.txt.
    
(59) Run:     "cat hello.txt 3>> anotherout.txt"

    The program prints 
    "
    hello world
    cat: '3>>': No such file or directory
    cat: anotherout.txt: No such file or directory
    Program exited with status 1
    "
    This is correct because there is no redirection sign like "3>>". 
    So this means cat 3 parameters. "hello.txt"exists, so "hello world"; "3>>" and "anotherout.txt" do not, so error messages.
    
(60) Run:     "cat < hello.txt > anotherout.txt"

    The program prints "Program exited with status 0".
    This is correct because "<" and ">" both functions.
    If you "cat anotherout.txt", you will see "hello world".

(61) Run:     "cat < hello.txt >> anotherout.txt"

    The program prints "Program exited with status 0".
    This is correct because "<" and ">>" both functions.
    If you "cat anotherout.txt", you will now see two lines of "hello world".
    
(62) Run:     "ls > out.txt < hello.txt"

    The program prints "Program exited with status 0".
    This is correct because ">" first redirect output to "out.txt".
    And "ls" doesn't need a input, so "< hello.txt" is omitted.
    If you "cat anotherout.txt", you will see the list of names of files.

(63) Run:     "ls > o1.txt > o2.txt"
    
    The program prints "Program exited with status 0".
    
    Then:     "cat o1.txt"
        
    The program prints "Program exited with status 0" and nothing else.
    
    Then:     "cat o2.txt"
    
    The program prints all the names.
    
    In myShell, when the same redirection sign is encountered, the new pathname(o2.txt) will replace the old one(o1.txt).
    Therefore, only the last ">" works.
    In a word, only one output redirection sign ">" or ">>" or "2>" or "2>>" works at one time.
    This makes sense because:
    1. The output can only be either normal output or error message 
    2. A need to redirect output to two different files at the same time is not required by this task.
    
(64) Run:     "cat < nnn.txt 2> double_dir_errout.txt"

    The program prints "Program exited with status 0".
    This is correct because "2>>" functions.
    If you "cat double_dir_errout.txt", you will now see the error message.
    
(65) Run:     "cat < nnn.txt 2>> double_dir_errout.txt"

    The program prints "Program exited with status 0".
    This is correct because "2>>" functions.
    If you "cat double_dir_errout.txt", you will now see two error messages.   


Now please run "exit" to exit myShell. If you run myShell with valgrind, you should see myShell is valgrind clean. 
Thank you for playing with myShell, hope you had fun :)

Nibo Ying
ny38
12/04/2018

    
