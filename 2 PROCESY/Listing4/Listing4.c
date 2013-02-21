#include <stdlib.h>//schemat wykorzystania funkcji spawnl
#include <stdio.h>
#include <process.h>

//Funkcje z rodziny spawn*() dają efekt wywołania sekwencji funkcji fork() i exec*().
//Najprostszą metodą utworzenia nowego procesu jest użycie funkcji system().
int main(int argc, char *argv[]) {
	int pid,i,res,status;
	      char buf[10];

      for(i=0;i<3;i++)
	      {
	          res = spawnl( P_NOWAIT, "myprog","myprog", itoa(i,buf,10), NULL );
	         if (res<0)
	         {
	             perror("Blad funkcji spawn");
	             exit(i);
	         }
	     }
	     while((pid=wait(&status))!=-1)
	         printf("Proces %d zakonczony, status %d\n",pid,WEXITSTATUS(status));
}
/*Pierwszy argument funkcji spawnl() określa tryb wykonania procesu potomnego,
 *  szczegółowo opisany w dokumentacji funkcji spanwl(). W linii 20 Listingu
 *  4 proces macierzysty zawiesza się wykonując funkcję wait() w oczekiwaniu
 *  na zakończenie procesów potomnych. Sposób użycia funkcji wait() opisano w
 *   następnym rozdziale. */
