#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>

/*5. (4.5 puncte) Cerinta (a). Procesul 1 trimite un numar oarecare (aleator) de
    semnale SIGUSR1 procesului 2, apoi scrie cate i-a trimis. Procesul 2 
    armeaza ceasul (alarm) pentru 4 secunde, apoi incepe sa numere semnalele 
    SIGUSR1 primite. Dupa primirea lui SIGALRM scrie cate a primit si trimite
    procesului 1 SIGKILL (asa se va termina procesul 1). Se va asigura
    protectia la pierderea unor semnale.*/
//Proces receptor

int nr = 0; 
pid_t pa;
sigset_t smask;


void sigusr1_handler(int n) {
    //se reimpune handlerul, dupa care se trimite SIGUSR1 catre emitator pentru confirmarea primirii
    signal(n, sigusr1_handler);
    kill(pa, SIGUSR1);
}

void sigalrm_handler(int n) {

    //La primirea SIGALRM se afiseaza numarul de semnale primite, se opreste celalalt proces si se iese din executie
    printf("S-au primit %d semnale\n", nr); 
    kill(pa, SIGKILL);
    exit(0);
}

int main() {
    //se vor capta semnalele cu handlerele respective
    signal(SIGUSR1, sigusr1_handler);
    signal(SIGALRM, sigalrm_handler);

    //initializare masca de semnale ce vor fi blocate, 
    sigemptyset(&smask); 
    sigaddset(&smask, SIGUSR1);
    //SIG_SETMASK asigura inlocuirea vechii masti de semnale a procesului
    sigprocmask(SIG_SETMASK, &smask, NULL);

    printf("PID propriu: %d\n", getpid());
    printf("PID advers: ");
    scanf("%d", &pa);

    //Se vor bloca toate semnalele pe langa SIGUSR! si SIGALRM
    sigfillset(&smask);
    sigdelset(&smask, SIGUSR1);
    sigdelset(&smask, SIGALRM);

    //dupa 4 secunde se va trimite SIGALRM propriului proces
    alarm(4);
    while(1) {
        //Se suspenda programul pana la primirea unui SIGUSR1 si se actualizeaza contorul
        sigsuspend(&smask);
        nr++;
    }
    
    return 0;
}