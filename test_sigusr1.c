#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

/* (a) 2 programe, care se lanseaza de la terminale diferite de catre acelasi
    utilizator, obtinand astfel 2 procese; fiecare proces va citi de la
    tastatura PID-ul celuilalt.
    (2 puncte) Cerinta (a). Fiecare proces trimite celuilalt 2000 SIGUSR1, apoi
    un SIGINT (care va determina terminarea procesului advers). Inainte de
    terminare, fiecare proces scrie cate semnale a trimis si cate a primit.
    */
int sent = 0;
int received = 0;
void receive_handler() {
    signal(SIGUSR1, receive_handler);
    received += 1;
}
int main() {
    signal(SIGUSR1, receive_handler);
    pid_t pid;
    pid_t own_pid = getpid();
    printf("PID propriu: %d\n", own_pid);
    printf("PID pentru comunicare: ");
    scanf("%d", &pid);
    //fereastra de 10 secunde pentru a avea timp sa verificam PID-ul si sa introducem in ambele procese
    printf("Initializare 10 secunde...\n");
    sleep(10);
    for(int i = 0; i < 2000; i++) {
        //transmis cu succes daca kill() returneaza 0
        if(kill(pid, SIGUSR1) == 0) sent += 1;
    }
    printf("Asteptare finala 10 secunde...\n");
    sleep(10);
    printf("Semnale SIGUSR1 trimise: %d\n", sent);
    printf("Semnale SIGUSR1 primite: %d\n", received);
    return 0;
}