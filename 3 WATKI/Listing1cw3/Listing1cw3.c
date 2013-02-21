 #include <pthread.h>
  #include <stdio.h>

/*Na Listingu 1 przedstawiono przykład programu tworzącego wątek.
 * Dokładny opis argumentów funkcji pthread_create() znajduje się w
 *  dokumentacji QNX. Parametry startowe wątku definiuje się ustawiając
 *   odpowiednio pola struktury typu pthread_attr_t, która jest jednym
 *   z argumentów przekazywanych do funkcji pthread_create().*/
  void *kod(void *argv)
  {
      while(1)
      {
          putc('W',stderr);
          sleep(1);
     }
     return NULL;
 }
 int main(int argc,char *argv[])
 {
     int tid;
     pthread_attr_t attr;
     pthread_attr_init( &attr );
    /*W zależności od atrybutów wątku, procedura, która utworzyła wątek może
      *  blokować się w oczekiwaniu na jego zakończenie. Oczekiwanie na zakończenie
      *  utworzonego wątku rozpoczyna się po wykonaniu funkcji pthread_join() i
      *  procedura tworząca wątek pozostaje zablokowana do momentu zakończenia wątku.
      *   Jeśli jednak wątek zakończył się przed wykonaniem przez procedurę, która
      *   go utworzyła funkcji pthread_join(), zasoby wątku nie są zwalniane dopóki
      *    funkcja pthread_join() nie zostanie wywołana. Takie zachowanie jest domyślne
      *     dla wątków dołączalnych. Jeśli nie życzymy sobie blokady w oczekiwaniu na
      *     zakończenie wątku, musimy utworzyć wątek w stanie niedołączalnym,
      *     odpowiednio inicjując strukturę typu pthread_attr_t zawierającą atrybuty
      *      tworzonego wątku (linie 16-18 powyższego listingu).

Różnicę między wątkiem dołączalnym i niedołączalnym najlepiej przetestować,
 podmieniając linie 18-24 w Listingu 1 poniższym kodem,a następnie okomentowując
  pierwszą lub drugą linię z kodu poniżej.*/
    // pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
     pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);
     pthread_create(&tid,&attr,kod,NULL);
     putc('M',stderr);
     sleep(1);
     pthread_join(tid,NULL);
     printf("M - koniec\n");
     pthread_create(&tid,&attr,kod,NULL);
     while(1)
     {
         putc('M',stderr);
         sleep(1);
     }
     pthread_attr_destroy(&attr);
 }
