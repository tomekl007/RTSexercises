  #include <stdio.h>
  #include <stdlib.h>
  #include <fcntl.h>
  #include <sys/stat.h>
  #define FIFO "MyFIFO"

  int main(void) {                     // Zapis do pliku FIFO
      int fdes,res,i=0;
      static char c;

     res = mkfifo(FIFO,O_RDWR | O_CREAT | S_IWUSR | S_IRUSR);
     if(res < 0) {
       perror("mkfifo");
     }
     printf("mkfifo - wynik %d\n",res);
     fdes = open("MyFIFO",O_WRONLY);
     if(fdes < 0) {
        perror("Open");
         return EXIT_FAILURE;
     }
     do {
         c = '0' + (i++)%10;
         printf("Zapis: %c \n", c);
         res = write(fdes, &c, 1);
    } while(i<10);
     close(fdes);
 }
