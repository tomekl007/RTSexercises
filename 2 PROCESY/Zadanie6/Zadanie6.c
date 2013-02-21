#include <stdlib.h>
#include <stdio.h>
#include <sys/resource.h>
/*Ustawianie i testowanie limitów na zużycie zasobów systemowych
Proszę napisać program ustawiający ograniczenia RLIMIT_CPU, RLIMIT_CORE,
 RLIMIT_DATA, RLIMIT_FSIZE, RLIMIT_NOFILE, RLIMIT_STACK, RLIMIT_NPROC.
  Proszę zademonstrować i obsłużyć sytuacje gdy ograniczenia te są przekraczane.*/

int main(int argc, char *argv[]) {
	struct rlimit limit;


	getrlimit(RLIMIT_NPROC, &limit);

	int current = limit.rlim_cur;

	printf("maxNumberOfProcessToCreate : %d\n ", current );
	//ustawiam limit procesow dzieci ktore moze stworzyc ten proces = 2
	limit.rlim_cur = 2;
	//limit.rlim_max = 2;
	//limit.rlim_cur_hi = 2;
	//limit.rlim_max_hi = 2;
	setrlimit(RLIMIT_NPROC, &limit);

	current = limit.rlim_cur;


	printf("maxNumberOfProcessToCreate : %d\n ", current );

	int pid = 0;
	int i = 0;
	int status;

	//mimo ze w petli jest zeby tworzyc 8 procesow dzieci, to stowrzy tylko dwa ze
	//wzgledu na ograniczenie
	for(; i < 8; i++){
		if( (pid = fork()) == 0){
		                printf("funkcja fork stworzyla proces potomny o id %d\n", pid);
		                printf("licznik : %d\n", i);
		                execl("Zadanie5a", "Zadanie5a", argv[1], NULL);
	    }


	}

	 while((pid=wait(&status))!=-1)
	                printf("Proces %d zakonczony, status %d\n",pid,WEXITSTATUS(status));


}
