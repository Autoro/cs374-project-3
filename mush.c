#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

void parse_args(char* buffer, char** args, const char* delimiters)
{
    int argCount = 0;

    args[argCount] = strtok(buffer, delimiters);
    while (args[argCount] != NULL)
    {
        argCount++;
        args[argCount] = strtok(NULL, delimiters);
    }

    args[argCount + 1] = NULL;
}

void run_command(char** args)
{
    pid_t pid = fork();
    if (pid == 0)
    {
        execvp(args[0], args);
        perror(args[0]);
        exit(1);
    }

    wait(NULL);
}

void change_working_directory(char** args)
{
    int result = chdir(args[1]);
    if (result == -1)
    {
        perror("cd");
    }
}

int main(void)
{
    const char* delimiters = " \t\n\r"; 
    
    char cwd[128];
    char buffer[2048];
    char* args[128];

    while (1)
    {
        getcwd(cwd, sizeof(cwd));

        printf("mush: %s$ ", cwd);
        fflush(stdout);
        fgets(buffer, sizeof(buffer), stdin);

        parse_args(buffer, args, delimiters);
        
        if (strcmp(args[0], "exit") == 0)
        {
            exit(0);
        }
        else if (strcmp(args[0], "cd") == 0)
        {
            change_working_directory(args);
        }
        else
        {
            run_command(args);
        }
    }
}
