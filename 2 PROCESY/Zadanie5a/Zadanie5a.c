#include <stdlib.h>
#include <stdio.h>
#include <sched.h>

int main(int argc, char *argv[]) {
	int i = 0;
	struct sched_param par;
    //int str;

	int arg = atoi(argv[1]);
	int newPriority = atoi(argv[2]);
	printf("jestem w zadaniu 5a wywolanym przez funkcje execl, arg: %d \n", arg);

	int status = getpid();

	sched_getparam(0,&par);
    par.sched_priority = newPriority;
    sched_setparam(0,&par);
	//sched_setscheduler(0,SCHED_FIFO,&par);

	for ( i = 0; i < arg ; i++ ){
	          printf ( "Proces potomny(priorytet %d) o pid %d krok %d\n",par.sched_priority, getpid(), i + 1 );

	          sleep(1);
	   }
   exit(status);//chyba nie potrzebne
	//exit();

}
