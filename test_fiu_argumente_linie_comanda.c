#include <stdio.h>
#include <stdlib.h>
/* A15. (1.5 puncte) Scrieti niste programe care arata daca fii obtinuti cu system
 ai unui proces "p" mostenesc suplimentar eventualele argumente in linia de
 comanda cu care a fost lansat "p". */
 // Fiu pornit cu system(); nu e menit sa fie rulat prin linia de comanda
 int main(int argc, char **argv) {
    printf("Proces fiu pornit prin apel system()\n");
    printf("argumente linie de comanda fiu: ");
    for(int i=1; i<=argc-1; i++){
        printf("%s ", argv[i]);
    }
    printf("\n");
    return 0;
}