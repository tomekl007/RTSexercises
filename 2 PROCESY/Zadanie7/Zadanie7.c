#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <process.h>
#include <unistd.h>


int main(int argc, char *argv[]) {
	//W argc jest liczba argumentów przekazana do programu, argv to tablica
	//o długości (argc + 1) przechowująca te argumenty

	//int l ;
	//for(l = 0 ; l < argc; l++){
	//    printf("argv[%d] = %c",l ,*argv[l] );
	//}

        int status;
        int iloscProc = atoi(argv[2]) ;
        int licznik = 0;
        int j;
        int pid = 1;
        int i;

        int response;

        //tworze tyle dzieci procesow ile wynosi "iloscProc"
        //printf("%d", iloscProc );
        for ( ; licznik < iloscProc && pid != 0; licznik++ ){
                j = licznik + 1;
                //Funkcja fork() tworzy proces potomny będący kopią procesu macierzystego
                //jesli fork() zwrocilo 0, czyli jest to proces child to:
                if( (pid = fork()) == 0){
                printf("funkcja fork stworzyla proces potomny o id %d\n", pid);
                printf("licznik : %d\n", licznik);
                //execl("Zadanie5a", "Zadanie5a", argv[1], NULL);

                /*P_WAIT	The exit status of the child process.
                 P_NOWAIT	The process ID of the child process. To get the exit status for a P_NOWAIT process, you must use the waitpid() function, giving it this process ID.
                 P_NOWAITO	The process ID of the child process, or 0 if the process is being started on a remote node. You can't get the exit status of a P_NOWAITO process.*/


//response = spawnl( P_NOWAITO, "Zadanie5a","Zadanie5a", argv[1], NULL );
               // response = spawnl( P_WAIT, "Zadanie5a","Zadanie5a", argv[1], NULL );
               // printf("response from spawl(exit status of child process ) : %d \n",response);

                response = spawnl( P_NOWAIT, "Zadanie5a","Zadanie5a", argv[1], NULL );
                  printf("response from spawl(id of child process) : %d \n",response);



                        if (response<0)
                        {
                            perror("Blad funkcji spawn");
                            exit(-1);
                        }

                }
        }

        //pid = getid();


        //wait return The process ID of the terminating child process,
        //or -1 if an error occurred or on delivery of a signal
        while((pid=wait(&status))!=-1)
                printf("Proces %d zakonczony, status %d\n",pid,WEXITSTATUS(status));

        return EXIT_SUCCESS;
}
