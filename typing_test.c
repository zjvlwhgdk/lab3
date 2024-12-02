#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_LENGTH 1024

int main() {
    char sentence[MAX_LENGTH];
    char user_input[MAX_LENGTH];
    int incorrect_count = 0;
    int total_chars = 0;
    time_t start, end;
    double elapsed_time;
    
    strcpy(sentence, "Lab 3 The typing test is starting. Let's copy this sentence");

    printf("타자 연습 시작\n");
    printf("\"%s\"\n\n", sentence);

    time(&start);

    printf("문장을 입력하세요: ");
    fgets(user_input, MAX_LENGTH, stdin);
    user_input[strcspn(user_input, "\n")] = '\0';

    time(&end);

    elapsed_time = difftime(end, start);

    for (int i = 0; i < strlen(sentence); i++) {
        if (user_input[i] != sentence[i]) {
            incorrect_count++;
        }
    }

    total_chars = strlen(sentence);

    double typing_speed = (total_chars / elapsed_time) * 60;

    printf("\n타자 연습 완료\n");
    printf("잘못 타이핑한 횟수: %d\n", incorrect_count);
    printf("소요 시간: %.2f 초\n", elapsed_time);
    printf("평균 분당 타자 수: %.2f WPM\n", typing_speed);

    return 0;
}

