#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#define NUM_WORKERS 2

static void spin(int id) {
    printf("worker %d (pid %d) spinning...\n", id, getpid());
    fflush(stdout);
    while (1);
}

int main(void) {
    for (int i = 0; i < NUM_WORKERS; i++) {
        pid_t pid = fork();
        if (pid < 0) {
            perror("fork");
            return 1;
        }
        if (pid == 0) {
            spin(i);
        }
    }

    for (int i = 0; i < NUM_WORKERS; i++) wait(NULL);

    return 0;
}
