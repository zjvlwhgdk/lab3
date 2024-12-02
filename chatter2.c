#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <unistd.h>

#define QUEUE_NAME_SEND "/chat_chatter2"
#define QUEUE_NAME_RECEIVE "/chat_chatter1"
#define MAX_SIZE 1024
#define STOP_MSG "exit"

int main() {
    mqd_t mq_send_queue, mq_receive_queue;
    struct mq_attr attr;
    char buffer[MAX_SIZE];

    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = MAX_SIZE;
    attr.mq_curmsgs = 0;

    mq_send_queue = mq_open(QUEUE_NAME_SEND, O_WRONLY | O_CREAT, 0644, &attr);
    if (mq_send_queue == (mqd_t)-1) {
        perror("mq_open (send)");
        exit(EXIT_FAILURE);
    }

    mq_receive_queue = mq_open(QUEUE_NAME_RECEIVE, O_RDONLY | O_CREAT, 0644, &attr);
    if (mq_receive_queue == (mqd_t)-1) {
        perror("mq_open (receive)");
        mq_close(mq_send_queue);
        exit(EXIT_FAILURE);
    }

    while (1) {
        ssize_t bytes_read = mq_receive(mq_receive_queue, buffer, MAX_SIZE, NULL);
        if (bytes_read == -1) {
            perror("mq_receive");
            break;
        }
        buffer[bytes_read] = '\0';
        printf("Friend: %s\n", buffer);

        if (strcmp(buffer, STOP_MSG) == 0) {
            break;
        }

        printf("You: ");
        fflush(stdout);
        fgets(buffer, MAX_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = '\0';

        if (mq_send(mq_send_queue, buffer, strlen(buffer) + 1, 0) == -1) {
            perror("mq_send");
            break;
        }

        if (strcmp(buffer, STOP_MSG) == 0) {
            break;
        }
    }

    mq_close(mq_send_queue);
    mq_close(mq_receive_queue);
    mq_unlink(QUEUE_NAME_SEND);
    return 0;
}

