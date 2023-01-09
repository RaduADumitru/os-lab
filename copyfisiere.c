#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

/* (2 puncte) Scrieti un program "copy" care se va lansa sub forma:
     copy   f1 + ... + fn    f
 (unde f1, ..., fn, f sunt fisiere) si are ca efect crearea lui f continand
 concatenarea lui f1, ..., fn; daca n=1 se copiaza f1 in f. Se vor folosi
 doar functiile de la nivelul inferior de prelucrare a fisierelor. */

int main(int argc, char **argv) {
    // validare intrare
    // 1) numar impar de argumente, diferit de 1
    if(argc % 2 != 1 || argc == 1) {
        fprintf(stderr, "Eroare: numar invalid de argumente; utilizare: %s <f1> + <f2> + ... + <fn> f\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    //2) argumentele de pe pozitii impare pe langa primul si ultimul sa fie +
    for(int i = 2; i < argc-1; i+=2) {
        if(strcmp(argv[i], "+") != 0) {
            fprintf(stderr, "Eroare: format invalid; utilizare: %s <f1> + <f2> + ... + <fn> f\n", argv[0]);
            exit(EXIT_FAILURE);
        }
    }
    //verificam daca ultimul fisier este deja existent prin flagul O_EXCL; in caz ca da, eroare, pentru ca trebuie sa fie nou creat.
    //eroare si in cazul in care nu a putut fi creat
    int fd_final = open(argv[argc-1], O_WRONLY | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);
    if(fd_final<0) {
        if (errno == EEXIST) {
            fprintf(stderr, "Eroare: fisierul %s este deja existent", argv[argc-1]);
        }
        else {
            perror(argv[argc-1]);
        }
        exit(EXIT_FAILURE);
    }
    /*    if(close(fd_final)<0) {
        fprintf(stderr, "Eroare: fisierul %s nu a putut fi inchis", argv[argc-1]);
        exit(EXIT_FAILURE);
    } */
    int fd;
    for(int i = 1; i < argc-1; i+=2) {
        fd = open(argv[i], O_RDONLY);
        if(fd == -1) {
            perror(argv[i]);
            exit(EXIT_FAILURE);
        }
    /*       fd_final = open(argv[argc-1], O_APPEND);
        if(fd_final == -1) {
            perror(argv[argc-1]);
            exit(EXIT_FAILURE);
        } */
        // citire din fd in fd_final caracter cu caracter
        char c;
        while (read(fd, &c, sizeof(char) > 0))
        {
            write(fd_final, &c, sizeof(char));
        }
        if(close(fd)<0) {
            perror(argv[i]);
            exit(EXIT_FAILURE);
        }
    }
    if(close(fd_final)<0) {
            perror(argv[argc-1]);
            exit(EXIT_FAILURE);
        }
    return 0;
    }