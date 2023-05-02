# Custom-Shell

Implementation of a simple command-line shell. The purpose was to have practice using Linux system calls.

# Supported Commands
To see the list of supported commands during runtime, use "help"

```
quit:		Exits the command line interpreter

repeat [arg]:	Writes arguments to standard output
	Options:
	> <filename>	Writes arguments to selected file if it exists. If file does not exist, creates a new file with the designated name.

clr:		Clears the terminal screen

myprocess:	Returns the Process ID of the currently running process

allprocesses:	Returns a list of all currently runnning processes and their PIDs using the Linux 'ps' command. For more information, enter 1.

chgd [args]:	Changes the currently working directory.
	Options:
	[args]		The adjacent directory, or the full path to the new directory

dir [args]:	Lists the contents of the specified directory. can be either the currently working directory, or the full path to the desired directory
	Options:
		-l	Lists the directory's contents in a long vertical format

environ:	Lists all the environment strings
```

# Instructions for Use
```
g++ -o [name] shell411.cpp
./[name]
```
Treat as any other command-line shell; enter the supported commands and arguments if necessary.
