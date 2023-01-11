#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>

/*
D1. (4 puncte) Program care primeste ca argument in linia de comanda un
 director si afisaza arborescenta de directoare si fisiere cu originea
 in el (similar comenzii tree /f din DOS).
 */
void tree(char* path, int level) {
    DIR* dp = opendir(path);
    struct dirent* direntptr;
    //parcurgem fiecare obiect din director
    while((direntptr=readdir(dp)) != NULL){
        //evitam . si .., prezente in toate directoarele, pentru a evita recursia infinita
        if ((strcmp(direntptr->d_name, ".") == 0) || (strcmp(direntptr->d_name, "..") == 0)){
            continue;
        }
        //obtinem calea absoluta catre obiectul gasit;
        //marime: cale originala + nume obiect + 2 ( / si terminator de sir)
        char* newpath = (char*)malloc(sizeof(path) + sizeof(direntptr->d_name)+2);
        sprintf(newpath, "%s/%s", path, direntptr->d_name);
        printf("\n");
        //afisare ramura arbore
        for(int prez_lvl=0; prez_lvl < level; prez_lvl++){
            printf("|");
            for(int sp = 0; sp < 3; sp++){
                printf(" ");
            }
        }
        printf(">");
        for(int poz=0; poz<3; poz++){
            printf("-");
        }
        //daca gasim un director in care avem acces, facem apel recursiv de functie din acesta, la un nivel in plus de afisare
        if((direntptr->d_type == DT_DIR) && (access(newpath, R_OK) == 0)){
            printf("%s", direntptr->d_name);
            tree(newpath, level+1);
            free(newpath);
            continue;
        }
        struct stat buf;
        //folosim lstat() in loc de stat() pentru a putea gasi si legaturile simbolice pe langa fisiere propriu-zise
        int r = lstat(newpath, &buf);
        //afisam: fisiere propriu-zise, legaturi simbolice sau tuburi
        if(S_ISREG(buf.st_mode) || S_ISLNK(buf.st_mode) || S_ISFIFO(buf.st_mode)){
            printf("%s", direntptr->d_name);
            free(newpath);
            continue;
        }
        free(newpath);
    }
    closedir(dp);
}
int main(int argc, char **argv) {
    if(argc != 2) {
        fprintf(stderr, "Eroare: format invalid! Utilizare: %s <cale>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    //verificam: calea specificata sa fie director
    struct stat buf;
    int r = stat(argv[1], &buf);
    if(!S_ISDIR(buf.st_mode)) {
        fprintf(stderr, "Eroare: %s nu este un director!\n", argv[1]);
        exit(EXIT_FAILURE);
    }
    //obtine cale absoluta de la cale relativa;
    //al doilea parametru e buffer, daca e setat null, se aloca un buffer nou si se returneaza un pointer catre acesta
    char* path = realpath(argv[1], NULL);
    printf("%s", path);
    tree(path, 0);
    printf("\n");
    free(path);
    return 0;
}