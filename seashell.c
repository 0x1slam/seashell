#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    while (!token) {
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

void execCmd(char **args)
{

}

void loop()
{
    char *cmd;
    char **args;
    int status = 1;

    do {
        printf(CYAN BOLD "> " RESET);

        cmd = readCmd();
        args = tokCmd(cmd);
        status = execCmd(args);

    } while(status);
}

int main(int argc, char **argv)
{
    loop();

    return 0;
}