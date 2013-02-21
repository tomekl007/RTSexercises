#include <stdlib.h>
#include <stdio.h>
#include <process.h>


//Funkcja fork() tworzy proces potomny bedacy kopia procesu macierzystego
int main(int argc, char *argv[]) {
	int pid;
	      if ((pid=fork())==0)
	      {
	          printf("Proces potomny PID: %d\n",getpid());
	          //instrukcje procesu potomnego
	     }
	     else
	     {
	         printf("Proces macierzysty PID: %d\n",getpid());
	         //instrukcje procesu macierzystego
	     }
}
