#include <iostream>
#include <signal.h>
#include <fstream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

using namespace std;

extern char **environ;

//Prints out the user manual for this shell
void help(){

    string input;
    //quit
    cout << endl << "411 COMMAND LINE INTERPRETER MANUAL" << endl << endl;
    cout << "1. quit\t\t\tExits the command line interpreter" << endl << endl;

    //repeat
    cout << "2. repeat [arg]\t\tWrites arguments to standard output" << endl;
    cout << "\tOptions:\n\t> <filename>\tWrites arguments to selected file if it exists. If file does not exist, creates a new file with the designated name." << endl << endl;

    //clr
    cout << "3. clr\t\t\tClears the terminal screen" << endl << endl;

    //myprocess
    cout << "4. myprocess\t\tReturns the Process ID of the currently running process" << endl << endl;

    //allprocesses
    cout << "5. allprocesses\t\tReturns a list of all currently runnning processes and their PIDs using the Linux 'ps' command. For more information, enter 1." << endl;
    getline(cin, input);
    if(input=="1") system("man ps");
    cout << endl << endl;

    //chgd
    cout << "6. chgd [args]\t\tChanges the currently working directory." << endl;
    cout << "\tOptions:\n\t[args]\t\tThe adjacent directory, or the full path to the new directory" << endl << endl;

    //dir
    cout << "7. dir [args]\t\tLists the contents of the specified directory. can be either the currently working directory, or the full path to the desired directory" << endl;
    cout << "\tOptions:\n\t\t-l\tLists the directory's contents in a long vertical format" << endl << endl;

    //environ
    cout << "8. environ\t\tLists all the environment strings. For more information, enter 1" << endl;
    getline(cin, input);
    if(input=="1") system("man env");
    cout << endl;
}

//Establishes a pipe, creates a child process, and has the child send a message to the parent.
void hiMom(){

    int fd[2], nbytes;
    pid_t childpid;
    char msg[] = "hey mom\n";
    char readbuffer[80];

    pipe(fd);
    childpid = fork();

    if(childpid < 0){
        perror("fork");
        exit(1);
    }
    else if(childpid==0){
        close(fd[0]);
        write(fd[1], msg, (strlen(msg))+1);
        exit(0);
    }
    else{
        close(fd[1]);
	    nbytes = read(fd[0], readbuffer, sizeof(readbuffer));
        wait(NULL);
	    cout << "Parent received message: " << readbuffer << endl;
    }

}

//Signal handler that listens for a signal created when the user hits Ctrl-Cs
//When the signal is caught, print the command history and exit the shell
void ctrlCHandler(int sigNum){
    
    fstream history;
    string line;
    history.open("commandHistory.txt", ios::in);

    if(!history.is_open()) cout << "error printing history";
    cout << endl << "COMMAND HISTORY" << endl;
    while(getline(history, line)){
        cout << line << endl;
    }
    history.close();
    exit(SIGINT);
}

int main(int argc, char *argv[]){

    //Registers SIGINT and the signal handler   
    struct sigaction handler;
    sigemptyset(&handler.sa_mask);
    handler.sa_flags = 0;
    handler.sa_handler = ctrlCHandler;
    sigaction(SIGINT, &handler, NULL);
    
    //Defining variables to be used
    string input, command, sysCmd, params, shell, line;
    fstream MyFile;
    MyFile.open("commandHistory.txt");

    if(!MyFile.is_open()){
        cout << "Created file\n";
        fstream MyFile("commandHistory.txt", ios::trunc | ios_base::out | ios_base::in);

        //cout << "Error creating file\n";
        //return EXIT_FAILURE;
    }

    shell = "411shellNickDiaz$\t";
    int delim;
    pid_t pid;

    //Identifies this shell
    cout << shell;
    while(getline(cin, input)){
        delim = input.find(" ");

        //First check to see if user quits before initiating parameters
        command = input.substr(0, delim);
        if(command=="quit"){
            MyFile << input << endl;
            system("cat commandHistory.txt");
            MyFile.close();
            return EXIT_SUCCESS;
        }
        params = input.substr(delim+1);

        //Linux echo command. params can take just the phrase being echoed
        //as well as '> <file>'
        if(command =="repeat"){
            MyFile << input + "\n";
            sysCmd = "echo " + params;
            system(sysCmd.c_str());
        }
        //Linux clear command
        else if(command=="clr"){
            MyFile << input + "\n";
            sysCmd = "clear";
            system(sysCmd.c_str());
        }
        //Gets the currently running process's Process ID
        else if(command=="myprocess"){
            MyFile << input + "\n";
            pid = getpid();
            cout << "Current Process ID: " <<  pid << endl;
        }
        //Lists the PID of all running processes
        else if(command=="allprocesses"){
            MyFile << input + "\n";
            sysCmd = "ps";
            system(sysCmd.c_str());
        }
        //Changes the currently working directory ONLY for the duration of the system command
        //the followup pwd command is to show that the cd worked as properly
        else if(command=="chgd"){
            MyFile << input + "\n";
            chdir(params.c_str());
            cout << "Working path: ";
            system("pwd");

        }
        //Linux ls command. When reading in an input string, if the string is length = 1
        //the parameters = command, so set the command to be executed by system() as "ls"
        //and then check if param = command. If false, append the parameters.
        else if(command=="dir"){
            MyFile << input + "\n";
            sysCmd = "ls";
            if(params != command){
                sysCmd += " " + params;
            }
            system(sysCmd.c_str());
        }
        //Linux env command
        else if(command=="environ"){
            MyFile << input + "\n";
            char **env = environ;
            while(*env){
                cout << (*env++) << endl;
            }
        }
        //User manual for this shell + man for linux commands when used
        else if (command=="help"){
            MyFile << input + "\n";
            help();
        }
	//himom command
	else if(command=="hiMom") {
            MyFile << input + "\n";
            hiMom();
    }
        //If command is not recognized, output an error message
        else{
            cout << command << ": Command not found" << endl;
        }
    
    
    cout << shell;
    }

    MyFile.close();
    return EXIT_SUCCESS;
}
