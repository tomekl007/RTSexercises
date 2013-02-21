#include <stdio.h>
  #include <stdlib.h>
  #include <fcntl.h>
  #include <sys/stat.h>
  #define FIFO "MyFIFOsl"

  int main(void) {              // Odczyt z pliku FIFO
      int fdes,res;
      static char c;

     res = mkfifo(FIFO,O_RDWR | O_CREAT | S_IWUSR | S_IRUSR);
     if(res < 0) {
         perror("mkfifo");
     }
     printf("mkfifo - wynik: %d\n",res);
     fdes = open(FIFO,O_RDONLY);
     if(fdes < 0) {
         perror("Open");
         return EXIT_FAILURE;
     }


     do {
         res = read(fdes, &c, 1);
         if(res < 0) {
             perror("read");
             return EXIT_FAILURE;
         }
         printf("Odczyt: %c \n", c);
     } while(res > 0);
     close(fdes);
     unlink(FIFO);
 }
