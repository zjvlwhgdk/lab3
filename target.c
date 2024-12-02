#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int main(int argc, char *argv[])
{
    FILE *src;
    FILE *dst;
    char ch;
    int count = 0;

    if (argc < 3) {
        printf("Usage: file_copy source_file destination_file\n");
        exit(1);
    }

    if ((src = fopen(argv[1], "r")) == NULL) {
        perror("fopen: src");
        exit(1);
    }

    if ((dst = fopen(argv[2], "w")) == NULL) {
        perror("fopen: dst");
        exit(1);
    }

    while ((ch = fgetc(src)) != EOF) {
        fputc(ch, dst);
        count++;
    }

    fclose(src);
    fclose(dst);

    return 0;
}

