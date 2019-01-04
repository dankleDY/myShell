myShell:shell.cpp command.cpp main.cpp
	g++ -o myShell -Wall -Werror -pedantic -std=gnu++98  shell.cpp command.cpp main.cpp
clean:
	rm -f *~ *# myShell
