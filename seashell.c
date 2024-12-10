#include <string.h>
#include <stdio.h>
#include <unistd.h>

int typeFn(const char *path) 
{
    if (access(path, F_OK) == 0) 
        return 1;
    return 0;
}

int main()
{
    char input[100];
    
    while (1) {
        printf("$ ");
        fflush(stdout);
        
        fgets(input, 100, stdin);
        input[strlen(input) - 1] = '\0';
        
        if (!strcmp(input, "exit")) { 
            return 0;
        } else if (!strncmp(input, "echo", 4)) {
            printf("%s\n", &input[5]);
        } else if (!strncmp(input, "type", 4)) {
            char path[200];
            snprintf(path, sizeof(path), "/usr/bin/%s", &input[5]);
            if (typeFn(path)) {
                printf("%s is /usr/bin/%s\n", &input[5], &input[5]);
            } else {
                printf("%s: command not found\n", &input[5]);
            }
        } else if (strlen(input) == 0) {
            continue;
        } else {
            printf("%s: command not found\n", input);
        }
    }
    return 0;
}