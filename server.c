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
    struct msgq_data recv_data;
    ssize_t len;

    if ((qid = msgget(QKEY, IPC_CREAT | 0666)) == -1) {
        perror("msgget failed");
        exit(1);
    }

    printf("서버가 준비되었습니다. 메시지를 기다립니다...\n");

    while (1) {
        if ((len = msgrcv(qid, &recv_data, BUFSIZE, 0, 0)) == -1) {
            perror("msgrcv failed");
            exit(1);
        }
        printf("클라이언트: %s\n", recv_data.text);
        
        if (strcmp(recv_data.text, "exit") == 0) {
            printf("서버를 종료합니다.\n");
            break;
        }
    }

    if (msgctl(qid, IPC_RMID, 0) == -1) {
        perror("msgctl failed");
        exit(1);
    }

    return 0;
}

