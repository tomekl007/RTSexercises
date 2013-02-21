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
        int iloscProc = 3;//argc + 1;
        int licznik = 0;
        int j;
        int pid = 1;
        int i;

        //tworze tyle dzieci procesow ile wynosi "iloscProc"
        //printf("%d", iloscProc );
        for ( ; licznik < iloscProc && pid != 0; licznik++ ){
                j = licznik + 1;
                //Funkcja fork() tworzy proces potomny będący kopią procesu macierzystego
                pid = fork();
                printf("funkcja fork stworzyla proces potomny o id %d\n", pid);
                printf("licznik : %d\n", licznik);
        }
        if ( pid == 0 ){        //potomny

        	//atoi czyta z konosli ilosc krokow dla procesow potomnych np./Zadanie3 3 6
        	//to bedzie "6"
                for ( i = 0; i < atoi( argv[ j + 1 ] ); ++i ){
                	//printf("argv[j + 1] = %c\n", argv[ j + 1 ] );

                        printf ( "Proces nr. %d o pid %d krok %d\n", j, getpid(), i + 1 );
                        sleep( 1 );
                }
                exit( j );
        }
        else{   //macierzysty
                for ( i = 0; i < atoi( argv[ 1 ]); ++i ){
                        printf ( "Proces macierzysty nr. %d o pid %d krok %d\n", 0, getpid(), i + 1 );
                        sleep(1);
                }
                for ( i = 0; i < iloscProc; ++i ){
                        pid = wait( &status );
                        printf( "Pid dziecka zakonczonego %d, kod jego powrotu %d\n", pid, status );
                }
        }
        return EXIT_SUCCESS;
}
