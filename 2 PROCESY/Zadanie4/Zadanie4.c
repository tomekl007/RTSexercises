#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <process.h>
#include <unistd.h>
#include <stdbool.h>

int main(int argc, char *argv[]) {
	//W argc jest liczba argumentów przekazana do programu, argv to tablica
	//o długości (argc + 1) przechowująca te argumenty

	//int l ;
	//for(l = 0 ; l < argc; l++){
	//    printf("argv[%d] = %c",l ,*argv[l] );
	//}

        int status;
        int iloscProc = 4;//argc + 1;
        int licznik = 0;
        int j;
        int pid = 1;
        int i;
        bool first = true;

        for ( ; licznik < iloscProc && pid != 0; licznik++ ){
                        j = licznik + 1;
                        //Funkcja fork() tworzy proces potomny będący kopią procesu macierzystego
                        pid = fork();
                        //A value of zero to the child process; and the process ID
                        //of the child process to the parent process
                        printf("funkcja fork stworzyla proces potomny o id %d, a parent proces iD :%d \n", pid, getppid() );
                        printf("licznik : %d\n", licznik);
                }

        if(pid == 0){
        	//pid = fork();

        	printf("child process id:%d his child id : %d, parent id: %d\n ",getpid(), pid, getppid() );
        }else {

        	printf("parent process, his child: %d, parent id: %d\n ", pid, getppid() );
        }




        return EXIT_SUCCESS;
}
