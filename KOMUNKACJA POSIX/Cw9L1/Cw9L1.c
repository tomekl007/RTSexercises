#include <unistd.h>
 #include <stdlib.h>

  int main(void) {
      int fd[2], child;
      char buf[] = "Komunikacja przez lacze nienazwane";
      char buf2[64];

      pipe(fd);            //Utworzenie łącza
     if ((child = fork()) == 0) {
         close(fd[0]);        //Proces  potomny – przesyła wiadomosc do macierzystego
         write(fd[1], buf, sizeof(buf));
         close(fd[1]);
         exit(0);
     }

     close(fd[1]);        //  Proces macierzysty – odczytuje wiadomosc od potomka
     read(fd[0], buf2, sizeof(buf));
     printf("%s\n", buf2);
     close(fd[0]);
 }
