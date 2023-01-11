#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

int main() {
    pid_t pid;
    printf("PID pentru comunicare: ");
    scanf("%d", &pid);
    kill(pid, SIGALRM);
    printf("SIGALRM trimis!\n");
    return 0;
}