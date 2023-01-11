#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdbool.h>

bool running = false;
void sigalrm_handler(int signum) {
    running = true;
}
int main() {
    signal(SIGALRM, sigalrm_handler);
    pid_t own_pid = getpid();
    printf("PID propriu: %d\n", own_pid);
    printf("Se armeaza ceasul...\n");
    sleep(2);
    printf("Ceas armat!\n");
    while(running == false) {
        printf("Continuare...\n");
        sleep(1);
    }
    printf("Program finalizat!\n");
    return 0;
}