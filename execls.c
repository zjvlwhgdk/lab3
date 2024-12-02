#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    printf("Executing execl.\n");
    execl("/bin/ls", "ls", "-l", (char *) 0);
    
    perror("execl failed to run ls");
    exit(1);
}

