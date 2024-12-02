#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

void list_directory(const char *dir_name, int depth) {
    DIR *dir;
    struct dirent *entry;
    struct stat statbuf;
    char path[1024];

    dir = opendir(dir_name);
    if (dir == NULL) {
        perror("opendir");
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        snprintf(path, sizeof(path), "%s/%s", dir_name, entry->d_name);

        if (stat(path, &statbuf) == -1) {
            perror("stat");
            continue;
        }

        if (S_ISDIR(statbuf.st_mode)) {
            for (int i = 0; i < depth; i++) printf("    ");
            printf("[%s]:\n", entry->d_name);
            list_directory(path, depth + 1);
        } else {
            for (int i = 0; i < depth; i++) printf("    ");
            printf("%s\n", entry->d_name);
        }
    }

    closedir(dir);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <directory>\n", argv[0]);
        return 1;
    }

    list_directory(argv[1], 0);

    return 0;
}

