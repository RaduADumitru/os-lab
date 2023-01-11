#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

/* (3 puncte) Scrieti un program "nrc" care se lanseaza sub forma:
     nrc   com
 unde "com" este o comanda externa (adica asociata unui fisier executabil de 
 pe disc) avand eventual si argumente in linia de comanda (deci com este un 
 sir de cuvinte) si care lanseaza comanda "com", numarand cuvintele scrise 
 de ea pe standard output. In acest scop procesul "nrc" creaza un tub fara
 nume, apoi se bifurca (cu "fork") a.i. intrarea standard a tatalui si iesirea 
 standard a fiului sa fie in acest tub, apoi fiul se inlocuieste (cu o functie
 din familia "exec") cu un proces ce executa "com". */
int main(int argc, char **argv)
{

  if (argc < 2)
  {
    fprintf(stderr, "Eroare: format invalid; utilizare: %s <comanda> <argumente comanda>\n", argv[0]);
        exit(EXIT_FAILURE);
  }
    //tub
  int pipefd[2];
  if (pipe(pipefd) < 0)
  {
    perror("Nu s-a putut crea tubul");
    exit(EXIT_FAILURE);
  }

  pid_t pid = fork();
  if (pid < 0)
  // Eroare
  {
    perror("Eroare: nu s-a putut face fork");
    exit(EXIT_FAILURE);
  }

  if (pid == 0)
  //copil
  {
    //inchidem capatul de citire
    close(pipefd[0]);
    //capatul de scrie va fi setat STDOUT
    dup2(pipefd[1], STDOUT_FILENO);
    close(pipefd[1]);
    //executa procesul pasat in linia de comanda, inlocuindu-l pe cel curent
    execvp(argv[1], &argv[1]);

    //procesul este inlocuit de exec; la codul de mai jos se mai ajunge numai in caz de erori
    perror(argv[1]);
    exit(EXIT_FAILURE);
  }

  else
  //parinte
  {
    //inchidem capatul de scriere
    close(pipefd[1]);
    //capatul de citire va fi setat STDIN
    dup2(pipefd[0],STDIN_FILENO);
    close(pipefd[0]);
    //citire bit cu bit pentru numararea cuvintelor
    int word_count = 0;
    char buffer[256];
    int bytes_read;
    bool in_word = false; 
    while ((bytes_read = read(STDIN_FILENO, buffer, sizeof(buffer))) > 0)
    { 
      for (int i = 0; i < bytes_read; i++)
      {
        if (isspace(buffer[i]))
        {
          if (in_word)
          //daca s-a gasit un spatiu in timp ce citeam un cuvant, s-a terminat citirea cuvantului
          { 
            word_count++;
            in_word = false;
          }
        }
        //daca se gaseste o litera, am intrat inauntrul unui cuvant
        else
        { 
          in_word = true;
        }
      }
    }
    if (bytes_read == -1)
    {
      perror("Citire esuata");
      return 1;
    }
    //adaugarea unui ultim cuvant daca exista, ce nu ar fi fost gasit din bucla precedenta
    if (in_word)
    {
      word_count++;
    }
    wait(NULL);

    printf("Numarul de cuvinte scrise de procesul copil: %d\n", word_count);
    return 0;
  }
}