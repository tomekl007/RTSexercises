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
        int iloscProc = atoi(argv[1]) ;
        int licznik = 0;
        int j;
        int pid = 1;
        int i;
     //   int flag = 1;

        //tworze tyle dzieci procesow ile wynosi "iloscProc"
        //printf("%d", iloscProc );
        for ( ; licznik < iloscProc && pid != 0; licznik++ ){
                j = licznik + 1;
                //Funkcja fork() tworzy proces potomny będący kopią procesu macierzystego
                //jesli fork() zwrocilo 0, czyli jest to proces child to:
                //if(j == iloscProc - 1){
               // 	exit(1);
              //  	flag = 0;
               // }

                //while(flag == 1){
                if( (pid = fork()) == 0){
                printf("funkcja fork stworzyla proces potomny o id %d, a parent : %d\n", getpid(), getppid() );

                printf("licznik : %d\n", licznik);
                //to wywoluje ten proces jeszcze raz(rekurencja)
                execl("Zadanie4b", "Zadanie4b", "1", NULL);
                }
               // }
              //  exit(1);
        }

        //pid = getid();


        //wait return The process ID of the terminating child process,
        //or -1 if an error occurred or on delivery of a signal
        while((pid=wait(&status))!=-1)
                printf("Proces %d zakonczony, status %d\n",pid,WEXITSTATUS(status));

        return EXIT_SUCCESS;
}
