#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

#define MAX_COMMAND_LENGTH 101
#define DELIM " \n"
#define PATH "PATH"
#define ERROR (-1)
#define LENGTH_OF_APPENDED_STR 8
#define EXIT "exit"
#define HISTORY_CMD "history"
#define CMD_NAME 0
#define CD_CMD "cd"

void setPathEnvVar(int argc, char *argv[])
{
        char *newPath = getenv(PATH);
        for (int i = 1; i < argc; ++i)
        {
            strcat(newPath, ":");
            strcat(newPath, argv[i]);
        }
        setenv(PATH, newPath, 1);
}

void setHistory(char *argv[], int argsCount, char *cmdHistory[], int pidHistory[], int *cmdCount, int pid)
{
    int lastElmIndex = *cmdCount;
    double sizeOfCurrentCmd = 0;
    for (int i = 0; i < argsCount; ++i)
    {
        sizeOfCurrentCmd += sizeof(argv[i]) + 1;
    }

    cmdHistory[lastElmIndex] = (char *)malloc(sizeOfCurrentCmd);
    if (cmdHistory[lastElmIndex]== NULL){
        perror("malloc failed");
    }
        strcpy(cmdHistory[lastElmIndex], argv[CMD_NAME]);
    for (int i = 1; i < argsCount; ++i)
    {
        strcat(cmdHistory[lastElmIndex], " ");
        strcat(cmdHistory[lastElmIndex], argv[i]);
    }
    pidHistory[lastElmIndex] = pid;
    ++(*cmdCount);
}

void execCmd(char *argv[], int argsCount, char *cmdHistory[], int pidHistory[], int *cmdCount)
{

    if (strcmp(argv[CMD_NAME], HISTORY_CMD) == 0)
    {
        int numOfCmds = *cmdCount;
        for (int i = 0; i < numOfCmds; ++i)
        {
            printf("%d %s", pidHistory[i], cmdHistory[i]);
            for (int j = 1; j < argsCount; ++j)
            {
                printf(" %s", argv[j]);
            }
            printf("\n");
        }
        printf("%d %s\n", getpid(), argv[CMD_NAME]);
        setHistory(argv, argsCount, cmdHistory, pidHistory, cmdCount, getpid());
    }
    else if (strcmp(argv[CMD_NAME], CD_CMD) == 0)
    {
        if (chdir(argv[1]) == -1)
        {
            perror("chdir failed");
            exit(1);
        }
        setHistory(argv, argsCount, cmdHistory, pidHistory, cmdCount, getpid());
    }
    else
    {
        pid_t pid = fork();
        if(pid < 0){
            perror("fork failed");
        }
        if (pid == 0)
        { // if in child

            if ((execvp(argv[CMD_NAME], argv)) == ERROR)
            {
                char *errorMsg = (char *)malloc(sizeof(char) * (strlen(argv[CMD_NAME]) + LENGTH_OF_APPENDED_STR));
                if (errorMsg == NULL)
                {
                    perror("malloc failed");
                    exit(1);
                }
                strcpy(errorMsg, argv[CMD_NAME]);
                strcat(errorMsg, " failed");
                perror(errorMsg);
                free(errorMsg);
                errorMsg = NULL;
                exit(1);
            }
        }
        else
        { // in parent
            if (waitpid(pid, NULL, 0) != pid)
            {
                printf("waitpid failed");
            }
            setHistory(argv, argsCount, cmdHistory, pidHistory, cmdCount, pid);
        }
    }
}

bool parseCommand(char *cmd, char *cmdHistory[], int pidHistory[], int *numOfCmds)
{
    char *argv[MAX_COMMAND_LENGTH];
    int argsCount = 0;

    argv[argsCount] = strtok(cmd, DELIM);
    if (strcmp(argv[CMD_NAME], EXIT) == 0)
    { // test if the user entered exit cmd
        return false;
    }
    while (argv[argsCount] != NULL)
    {
        ++argsCount;
        argv[argsCount] = strtok(NULL, DELIM);
    }

    execCmd(argv, argsCount, cmdHistory, pidHistory, numOfCmds);
    return true;
}

bool getCommand(char *cmdHistory[], int pidHistory[], int *numOfCmds)
{
    char command[MAX_COMMAND_LENGTH];
    printf("$ ");
    fflush(stdout);
    fgets(command, sizeof(command), stdin);
    return parseCommand(command, cmdHistory, pidHistory, numOfCmds);
}

void runShellLoop(int argc, char *argv[])
{
    setPathEnvVar(argc, argv);
    int pidHistory[MAX_COMMAND_LENGTH];
    int numOfCmds = 0;
    char *cmdHistory[100];
    while (getCommand(cmdHistory, pidHistory, &numOfCmds))
    {
    }
    for (int i = 0; i < numOfCmds; ++i)
    {
        free(cmdHistory[i]);
        cmdHistory[i] = NULL;
    }
}

int main(int argc, char *argv[])
{

    runShellLoop(argc, argv);

    return 0;
}
