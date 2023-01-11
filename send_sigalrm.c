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
    if(kill(pid, SIGALRM) == 0){
        printf("SIGALRM trimis!\n");
    }
    else {
        fprintf(stderr, "Eroare la trimiterea semnalului");
    }
    return 0;
}