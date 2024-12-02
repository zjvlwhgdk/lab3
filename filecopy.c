#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/wait.h>

#define SHM_SIZE 1024  
#define SEM_KEY 1234  
#define SHM_KEY 5678  

struct sembuf p_op = {0, -1, 0}; 
struct sembuf v_op = {0, 1, 0};  

struct shm_data {
    char buffer[SHM_SIZE];
    int end_of_file;
};

void sem_wait(int sem_id) {
    if (semop(sem_id, &p_op, 1) == -1) {
        perror("sem_wait failed");
        exit(1);
    }
}

void sem_signal(int sem_id) {
    if (semop(sem_id, &v_op, 1) == -1) {
        perror("sem_signal failed");
        exit(1);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <source_file> <destination_file>\n", argv[0]);
        exit(1);
    }

    char *src_file = argv[1];
    char *dest_file = argv[2];

    int shm_id = shmget(SHM_KEY, sizeof(struct shm_data), IPC_CREAT | 0666);
    if (shm_id == -1) {
        perror("shmget failed");
        exit(1);
    }

    struct shm_data *shm_ptr = (struct shm_data *)shmat(shm_id, NULL, 0);
    if ((void *)shm_ptr == (void *)-1) {
        perror("shmat failed");
        exit(1);
    }

    int sem_id = semget(SEM_KEY, 1, IPC_CREAT | 0666);
    if (sem_id == -1) {
        perror("semget failed");
        exit(1);
    }

    if (semctl(sem_id, 0, SETVAL, 1) == -1) {
        perror("semctl failed");
        exit(1);
    }

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        exit(1);
    }

    if (pid == 0) {
        FILE *dest_fp = fopen(dest_file, "w");
        if (!dest_fp) {
            perror("fopen destination file failed");
            exit(1);
        }

        while (1) {
            sem_wait(sem_id);

            if (shm_ptr->end_of_file == 0) {
                break;
            }

            fwrite(shm_ptr->buffer, sizeof(char), SHM_SIZE, dest_fp);
            sem_signal(sem_id);
        }

        fclose(dest_fp);
        shmdt(shm_ptr);
        exit(0);
    } else {
        FILE *src_fp = fopen(src_file, "r");
        if (!src_fp) {
            perror("fopen source file failed");
            exit(1);
        }

        int bytes_read;
        while ((bytes_read = fread(shm_ptr->buffer, sizeof(char), SHM_SIZE, src_fp)) > 0) {
            shm_ptr->end_of_file = 1;
            sem_signal(sem_id);

            sem_wait(sem_id);
        }

        shm_ptr->end_of_file = 0;
        sem_signal(sem_id);

        fclose(src_fp);
        wait(NULL);
        shmdt(shm_ptr);
        shmctl(shm_id, IPC_RMID, NULL);
        semctl(sem_id, 0, IPC_RMID);
    }

    return 0;
}

