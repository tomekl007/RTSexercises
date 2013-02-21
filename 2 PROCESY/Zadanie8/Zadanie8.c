#include <stdlib.h>
#include <stdio.h>
#include <sched.h>


// arg[1] = ilosc krokow, nastepnie prirytety kolejnych procesow
int main(int argc, char *argv[]) {

	    int iloscProc = argc - 3 ;

		struct sched_param par;

	    int status;

	    int licznik = 0;
	    int j;
	   	int pid = 1;
	    int i;

	    int scheduler = 1;

	  //  printf("argc : %d, argv[argc] : %s\n", argc, argv[argc-1]);

	  //  char typeOfSchedule;

	   // typeOfSchedule = argv[argc - 1];
	   // printf("type of schedule : %s", typeOfSchedule);
	    const char *fifo = "FIFO";
	    const char *rr = "RR";

	  // int bool = strcmp((argv[argc-1]), fifo);

	  // printf("i from strcmp : %d\n", bool);

	    if( (strcmp((argv[argc-1]), fifo)) == 0 ){
	    	scheduler = 1;
	    }else if( (strcmp((argv[argc-1]), rr)) == 0 ){
	    	scheduler = 2;
	    }else{
	    	scheduler = 3;
	    }

	    sched_getparam(0,&par);
	    sched_setscheduler(0,scheduler,&par);

	    printf("after change algoritm of schedule = %d \n", sched_getscheduler(0) );



	    for ( ; licznik < iloscProc && pid != 0; licznik++ ){
	   	                    j = licznik + 1;
	   	                    //Funkcja fork() tworzy proces potomny będący kopią procesu macierzystego
	   	                    //jesli fork() zwrocilo 0, czyli jest to proces child to:
	   	                    if( (pid = fork()) == 0){
	   	                    printf("funkcja fork stworzyla proces potomny o id %d\n", pid);
	   	                    printf("licznik : %d\n", licznik);



	   	                    execl("Zadanie5a", "Zadanie5a", argv[1], argv[1 + licznik], NULL);
	   	                    }
	   	            }


	    // printf("Przydzielony priorytet: %d\n",par.sched_priority);
//
	   // str=sched_getscheduler(0);
	  //  printf("Strategia szeregowania: %d\n",str);




	  //  sleep(20);
	  //  return(0);







	            //pid = getid();


	            //wait return The process ID of the terminating child process,
	            //or -1 if an error occurred or on delivery of a signal
	            while((pid=wait(&status))!=-1)
	                    printf("Proces %d zakonczony, status %d\n",pid,WEXITSTATUS(status));

	            return EXIT_SUCCESS;
}
