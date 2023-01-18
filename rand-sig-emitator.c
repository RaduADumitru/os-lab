#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>

/*5. (4.5 puncte) Cerinta (a). Procesul 1 trimite un numar oarecare (aleator) de
    semnale SIGUSR1 procesului 2, apoi scrie cate i-a trimis. Procesul 2 
    armeaza ceasul (alarm) pentru 4 secunde, apoi incepe sa numere semnalele 
    SIGUSR1 primite. Dupa primirea lui SIGALRM scrie cate a primit si trimite
    procesului 1 SIGKILL (asa se va termina procesul 1). Se va asigura
    protectia la pierderea unor semnale.*/
//Proces emitator
int nr = 0; 
pid_t pa; 
sigset_t smask;

//apelat la captarea sigusr1
void sigusr1_handler(int n) {
    //se reinnoieste captarea semnalului
    signal(n, sigusr1_handler);
}

// //apelat la captarea sigkill: arata cate semnale s-au trimis
// void sigkill_handler(int n) {
//     //se reinnoieste captarea in caz ca este trimis inca un semnal
//     signal(n, sigkill_handler);
//     printf("Am trimis %d\n", nr);
//     exit(0);
// }

int main() {
    int nr_s, inf = 1, sup = 50;
    //initializare generator numere aleatoare
    srand(time(0));

    printf("Limite numar ales aleatoriu: %d-%d\n", inf, sup);

    nr_s = (rand() % (sup - inf + 1)) + inf;

    printf("Numar ales aleatoriu: %d\n", nr_s);

    //se vor capta semnalele cu handlerele respective
    signal(SIGUSR1, sigusr1_handler);
    // signal(SIGTERM, sigkill_handler);

    //initializare masca de semnale ce vor fi blocate, 
    sigemptyset(&smask);
    sigaddset(&smask, SIGUSR1);
    //SIG_SETMASK asigura inlocuirea vechii masti de semnale a procesului
    sigprocmask(SIG_SETMASK, &smask, NULL);

    printf("PID propriu: %d\n", getpid());
    printf("PID advers: "); 
    scanf("%d", &pa);

    //initializare masca astfel incat sa includa toate semnalele
    sigfillset(&smask);
    //eliminare SIGUSR1 din masca: toate semnalele vor fi blocate in afara de SIGUSR1, primit de la receptor drept confirmare ca a receptionat un semnal trimis
    sigdelset(&smask, SIGUSR1);
    //pentru a putea opri programul din executie excludem SIGTERM din masca
    sigdelset(&smask, SIGTERM);

    for(int i = 0; i < nr_s; ++i) {
        //trimite semnalul si numara semnalele trimise
        if(kill(pa, SIGUSR1) == 0) nr++;
        //se suspenda procesul pana la primirea unui semnal SIGUSR1, ce nu e parte din masca
        sigsuspend(&smask);
    }
    
    //va continua rularea pana primeste SIGTERM de la celalalt proces
    while(1) {}

    return 0;
}