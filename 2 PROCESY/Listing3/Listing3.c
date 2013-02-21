#include <stdlib.h>
#include <stdio.h>
#include <process.h>


/*Wykonanie funkcji z rodziny exec*() spowoduje zastapienie biezacego procesu procesem utworzonym z
 *  podanego jako parametr funkcji pliku wykonywalnego. Funkcje z rodziny exec*() są zwykle uzywane
 *  w polaczeniu z funkcja fork().*/
int main(int argc, char *argv[]) {//schemat wykorzystania funkcji exec
	 int pid;
	     if ((pid=fork())==0)
	     {
	        //Uruchomienie programu my_prog i przekazanie argumentu z linii poleceń
	       execl("my_prog","my_prog",argv[1],NULL);
	        perror("Blad funkcji exec");
	     }
	     else
	     {
	        printf("Proces macierzysty PID: %d\n",getpid());
	        //instrukcje procesu macierzystego
	    }
}
