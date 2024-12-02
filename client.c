#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUFSIZE 128
#define QKEY (key_t)0111

struct msgq_data {
    long type;
    char text[BUFSIZE];
};

int main() {
    int qid;
    struct msgq_data send_data;

    if ((qid = msgget(QKEY, 0666)) == -1) {
        perror("msgget failed");
        exit(1);
    }

    while (1) {
        printf("메시지를 입력하세요 (종료하려면 'exit' 입력): ");
        fgets(send_data.text, BUFSIZE, stdin);
        send_data.text[strcspn(send_data.text, "\n")] = '\0';

        send_data.type = 1;

        if (msgsnd(qid, &send_data, strlen(send_data.text) + 1, 0) == -1) {
            perror("msgsnd failed");
            exit(1);
        }

        if (strcmp(send_data.text, "exit") == 0) {
            printf("클라이언트를 종료합니다.\n");
            break;
        }
    }

    return 0;
}

