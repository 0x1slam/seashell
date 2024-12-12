#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define CYAN "\033[36m"
#define YELL "\033[33m"
#define RED "\033[31m"
#define RESET "\033[0m"
#define BOLD "\033[1m"

#define BUFSIZE 1024
#define TOK_BUFSIZE 64
#define TOK_DELIMS " \t\r\n\a"

void die(const char *error) 
{
    fprintf(stderr, RED);
    perror(error);
    fprintf(stderr, RESET);
    exit(1);
}

char *readCmd(void)
{
    int bufsize = BUFSIZE;
    char *buf = malloc(sizeof(char) * bufsize);
    if (!buf) {
        die("malloc");
    }

    int c, pos = 0;
    while (1) {
        c = getchar();
        if (c == EOF || c == '\n') {
            buf[pos] = '\0';
            return buf;
        } else {
            buf[pos++] = c;
        }

        if (pos >= bufsize) {
            bufsize += BUFSIZE;
            buf = realloc(buf, bufsize);
            if (!buf) {
                die("malloc");
            }
        }
    }
}

char **tokCmd(char *cmd)
{
    int bufsize = TOK_BUFSIZE, pos = 0;
    char **tokens = malloc(sizeof(char*) * bufsize);
    if (!tokens) {
        die("malloc");
    }

    char *token = strtok(cmd, TOK_DELIMS);
    while (token != NULL) {
        tokens[pos++] = token;

        if (pos >= bufsize) {
            bufsize += TOK_BUFSIZE;
            tokens = realloc(tokens, sizeof(char*) * bufsize);
            if (!tokens) {
                die("malloc");
            }
        }

        token = strtok(NULL, TOK_DELIMS);
    }

    tokens[pos] = NULL;
    return tokens;
}

int execCmd(char **args)
{
    pid_t pid, wpid;
    int status;
    pid = fork();
    
    if (pid == 0) {
        if (-1 == execvp(args[0], args)) {
            die("seashell");
        }
    } else if (pid < 0) {
        die("seashell");
    } else {
        do {
            wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
}

int cdFn(char **args)
{
    if (args[1] == NULL) {
        fprintf(stderr, RED "seashell: no arguments given to \"cd\"\n" RESET);
    } else {
        if (0 != chdir(args[1])) {
            perror("seashell");
        }
    }
    return 1;
}

void loop()
{
    char *cmd;
    char **args;
    int status;

    do {
        printf(CYAN BOLD ">> " RESET);

        cmd = readCmd();
        args = tokCmd(cmd);

        if (args[0] != NULL && !strcmp(args[0], "exit")) {
            exit(0);
        } else if (args[0] != NULL && !strcmp(args[0], "cd")) {
            status = cdFn(args);
        } else {
            status = execCmd(args);
        }

        free(cmd);
        free(args);
    } while(status);
}

int main(int argc, char **argv)
{
    loop();
    return 0;
}