#include <stdio.h>
#include <stdlib.h>
//(1 punct) Scrieti un program care primeste ca argument in linia de
// comanda un intreg si afisaza suma cifrelor sale.
long suma_cifrelor(long nr) {
        long suma = 0;
        while(nr != 0) {
            suma += nr % 10;
            nr /= 10;
        }
        return suma;
    }
int main(int argc, char **argv) {
    // numaram argumentele primite, primul fiind numele programului, iar al doilea trebuie sa fie numarul caruia i se calculeaza suma
    if(argc != 2) {
        fprintf(stderr, "Eroare: format invalid; utilizare: %s <nr>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    //pointer ce va arata locatia unei posibile erori in transmiterea argumentului; strtol ii va seta locatia de memorie in caz de erori
    char *errorPtr = NULL;
    long nr = strtol(argv[1], &errorPtr, 10);
    if(*argv[1] == '\0' || *errorPtr != '\0')
    {
        fprintf(stderr, "Eroare: valoarea citita %s nu este numerica: greseala la %s\n", argv[1], errorPtr);
        exit(EXIT_FAILURE);
    }
    printf("Suma cifrelor pentru numarul %ld este %ld\n", nr, suma_cifrelor(nr));
    return 0;
}