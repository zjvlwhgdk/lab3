#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int my_system(const char *command) {
    pid_t pid;
    int status;

    pid = fork();
    if (pid < 0) {
        return -1;
    }

    if (pid == 0) {
        char *argv[] = {"/bin/sh", "-c", "export LS_COLORS='di=1;34:fi=0;32:*.c=0;37:*.txt=0;37' && ls --color=auto -la", NULL};
        execvp(argv[0], argv);
        exit(1);
    } else {
        wait(&status);
        return status;
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <command>\n", argv[0]);
        exit(1);
    }

    char command[256];
    snprintf(command, sizeof(command), "%s", argv[1]);

    int result = my_system(command);

    if (result == -1) {
        fprintf(stderr, "Command execution failed\n");
        exit(1);
    }

    return 0;
}

