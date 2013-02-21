#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

/*Podobnie jak w przypadku kończenia procesu, procedura tworząca
 * watek może otrzymać kod powrotu od zakończonego wątku. Kodem tym
 *  jest albo wartość zwracana przez funkcję, definiującą kod wątku,
 *  albo wartość przekazana przez argument funkcji pthread_exit(). Kod
 *   powrotu jest przekazywany do funkcji pthread_join() jako drugi argument
 *   */



  #define NUM_THREADS 4
  #define LICZBA_KROKOW 8

  pthread_t tid[NUM_THREADS];        //Tablica z numerami TID wątków

 void *kod(void *arg)
 {
     int numer=(int)arg;
     int i;

     fprintf(stderr,"Proces: %d, watek: %d\n",getpid(),tid[numer-1]);
     for(i=0;i<LICZBA_KROKOW;i++) {
         fprintf(stderr,"\twatek %d, krok %d\n",tid[numer-1],i);
         sleep(1);
     }
     return (void*) numer;        //lub pthread_exit((void)*numer);
 }

 int main(int argc,char *argv[])
 {
     int i,status;
     for(i=0;i<NUM_THREADS;i++)         //utworzenie wątków
              pthread_create(&tid[i],NULL,kod,(void *)(i+1));
     for(i=0;i<NUM_THREADS;i++) {        //oczekiwanie na zakończenie
              pthread_join(tid[i],(void*)&status);
              printf("watek %d zakonczony, kod powrotu %d\n",tid[i],status);
          }
     return 0;
 }
