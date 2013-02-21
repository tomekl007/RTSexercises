#include <stdlib.h>
#include <stdio.h>
#include <sched.h>


//Na Listingu 1 przedstawiono program testujacy i ustawiajacy priorytet i strategie szeregowania dla procesu biezacego.
int main(void) {

	     struct sched_param par;
	     int str;
	     sched_getparam(0,&par);
	     printf("Przydzielony priorytet: %d\n",par.sched_priority);

	    str=sched_getscheduler(0);
	    printf("Strategia szeregowania: %d\n",str);


	    par.sched_priority=9;
	    sched_setparam(0,&par);
	    sched_setscheduler(0,SCHED_FIFO,&par);

	    sleep(20);
	    return(0);
}
