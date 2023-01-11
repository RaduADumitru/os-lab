#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

/* A23. (3 puncte) Scrieti un program "nrc" care se lanseaza sub forma:
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
    fprintf(stderr, "Eroare: format invalid! Utilizare: %s <com> <args>\n", argv[0]);
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
  // tratam eroare in caz ca nu se poate face fork
  if (pid < 0)
  {
    perror("Eroare: nu s-a putut face fork");
    exit(EXIT_FAILURE);
  }

  if (pid == 0)
  //copil
  {
    //inchidem capatul de citire, nefolosit aici
    close(pipefd[0]);
    //capatul de scriere va fi setat STDOUT, pentru ca output-ul procesului sa fie scris in pipe
    dup2(pipefd[1], STDOUT_FILENO);
    close(pipefd[1]);
    //executa procesul pasat in linia de comanda, inlocuindu-l pe cel curent
    //cu execvp putem pasa si argumente de linie de comanda
    //parametrii de linie de comanda sunt pasati drept al doilea parametru, sub forma de vector de stringuri
    execvp(argv[1], &argv[1]);

    //procesul este inlocuit de exec; la codul de mai jos se mai ajunge numai in caz de erori
    perror(argv[1]);
    exit(EXIT_FAILURE);
  }

  else
  //parinte
  {
    //inchidem capatul de scriere, nefolosit aici
    close(pipefd[1]);
    //capatul de citire va fi setat STDIN, ca sa citeasca din pipe output-ul procesului
    dup2(pipefd[0],STDIN_FILENO);
    close(pipefd[0]);
    //citire bit cu bit pentru numararea cuvintelor
    int word_count = 0;
    char buffer[100];
    int bytes_read;
    bool is_in_word = false; 
    while ((bytes_read = read(STDIN_FILENO, buffer, sizeof(buffer))) > 0)
    { 
      for (int i = 0; i < bytes_read; i++)
      {
        //verificam daca am ajuns la spatiu
        if (isspace(buffer[i]))
        {
          if (is_in_word)
          //daca s-a gasit un spatiu in timp ce citeam un cuvant, s-a terminat citirea cuvantului
          { 
            word_count++;
            is_in_word = false;
          }
        }
        //daca se gaseste o litera, am intrat inauntrul unui cuvant
        else
        { 
          is_in_word = true;
        }
      }
    }
    //read returneaza -1 la erori
    if (bytes_read == -1)
    {
      perror("Citire esuata");
      return 1;
    }
    //adaugarea unui ultim cuvant daca exista, ce nu ar fi fost gasit din bucla precedenta
    if (is_in_word)
    {
      word_count++;
    }

    wait(NULL);
    printf("%d cuvinte scrise de procesul fiu %s\n", word_count, argv[1]);
    return 0;
  }
}