#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdbool.h>

/* (a) 2 programe, care se lanseaza de la terminale diferite de catre acelasi
    utilizator, obtinand astfel 2 procese; fiecare proces va citi de la
    tastatura PID-ul celuilalt.
    (2 puncte) Cerinta (a). Fiecare proces trimite celuilalt 2000 SIGUSR1, apoi
    un SIGINT (care va determina terminarea procesului advers). Inainte de
    terminare, fiecare proces scrie cate semnale a trimis si cate a primit.
    */
int sent = 0;
int received = 0;
bool received_sigusr2 = false;
void sigusr1_handler(int signum) {
    signal(SIGUSR1, sigusr1_handler);
    received += 1;
};
void sigint_handler(int signum) {
    printf("Proces intrerupt de SIGINT:\nSemnale SIGUSR1 trimise: %d\n", sent);
    printf("Semnale SIGUSR1 primite: %d\n", received);
    exit(0);
}
//folosim sigusr2 pentru a transmite ca procesul a citit PID-ul
void sigusr2_handler(int signum) {
    received_sigusr2=true;
}
int main() {
    signal(SIGUSR1, sigusr1_handler);
    signal(SIGINT, sigint_handler);
    signal(SIGUSR2, sigusr2_handler);
    pid_t other_pid;
    pid_t own_pid = getpid();
    printf("PID propriu: %d\n", own_pid);
    printf("PID pentru comunicare: ");
    scanf("%d", &pid);
    //trimite sigusr2 la celalalt proces dupa citire
    kill(pid, SIGUSR2);
    //amanam executia pana este primit sigusr2 de la celalalt proces
    while(received_sigusr2==false);
    for(int i = 0; i < 2000; i++) {
        //transmis cu succes daca kill() returneaza 0
        if(kill(pid, SIGUSR1) == 0) sent += 1;
    }
    kill(pid, SIGINT);
    printf("Final: Semnale SIGUSR1 trimise: %d\n", sent);
    printf("Semnale SIGUSR1 primite: %d\n", received);
    return 0;
}