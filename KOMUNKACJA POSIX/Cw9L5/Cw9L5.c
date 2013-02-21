#include <signal.h>
  #include <stdlib.h>
  #include <stdio.h>

  int sigcnt = 0;
  int sig = 0;

  void sighandler(int signum) { // Funkcja obslugi sygnalu
     sigcnt++;
     sig = signum;
 }

 int main(void) {
     int i =0;

     signal(SIGINT, sighandler);
     do {
       printf(" %d %d %d  \n",i,sigcnt,sig);
         pause();


         i++;
     } while(1);
     return EXIT_SUCCESS;
 }
