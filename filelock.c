#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main() {
    int fd;
    struct flock testlock;
    int pid;

    testlock.l_type = F_WRLCK;
    testlock.l_whence = SEEK_SET;
    testlock.l_start = 0;
    testlock.l_len = 10;

    fd = open("testlock", O_RDWR | O_CREAT, 0666);
    if (fd == -1) {
        perror("open failed");
        exit(1);
    }

    if (fcntl(fd, F_SETLKW, &testlock) == -1) {
        perror("parent: locking");
        exit(1);
    }
    printf("parent: locked record\n");

    pid = fork();
    if (pid == 0) {
        testlock.l_len = 5;
        if (fcntl(fd, F_SETLKW, &testlock) == -1) {
            perror("child: locking");
            exit(1);
        }
        printf("child: locked\n");
        sleep(5);
        printf("child: exiting\n");
    } else if (pid > 0) {
        sleep(5);
        printf("parent: exiting\n");
    } else {
        perror("fork failed");
    }

    close(fd);
    return 0;
}

