#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdbool.h>
/*S4 (2 puncte) Program care armeaza ceasul ("alarm") pentru 2 secunde, apoi 
    intra intr-un ciclu infinit. La primirea lui SIGALRM nu se va termina 
    procesul ci se va iesi din ciclu (se va continua programul dupa ciclul 
    respectiv).*/

//controleaza bucla infinita
bool running = false;
void sigalrm_handler(int signum) {
    // asigura iesirea din bucla infinita
    running = true;
}
int main() {
    //recepteaza semnalul SIGALRM cu handlerul definit
    signal(SIGALRM, sigalrm_handler);
    //afiseaza propriul PID pentru a fi mai usor sa transmitem semnal SIGALRM prin alt proces de la tastatura
    pid_t own_pid = getpid();
    printf("PID propriu: %d\n", own_pid);
    printf("Se armeaza ceasul...\n");
    sleep(2);
    printf("Ceas armat!\n");
    while(running == false) {
        printf("Continuare...\n");
        sleep(2);
    }
    printf("Program finalizat!\n");
    return 0;
}