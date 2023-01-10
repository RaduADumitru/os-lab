#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

/* (1.5 puncte) Scrieti niste programe care arata daca fii obtinuti cu system
 ai unui proces "p" mostenesc suplimentar eventualele argumente in linia de
 comanda cu care a fost lansat "p". */
 //Proces parinte, ce va porni un fiu cu system()
int main(int argc, char **argv) {
    printf("Proces parinte pornit\n");
    system("./test_fiu_argumente_linie_comanda");
    wait(NULL);
    printf("argumente linie de comanda parinte: ");
    for(int i=1; i<=argc-1; i++){
        printf("%s ", argv[i]);
    }
    printf("\n");
    return 0;
}