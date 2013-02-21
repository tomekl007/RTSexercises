#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/dispatch.h>

#define ATTACH_POINT "myname"
#define OPENR 1 // Otwarcie pliku do odczytu
#define OPENW 2 // Otwarcie pliku do zapisu
#define READ 3 // Odczyt fragmentu pliku
#define CLOSE 4 // Zamkniecie pliku
#define WRITE 5 // Zapis fragmentu pliku
#define STOP 10 // Zatrzymanie serwera

/* We specify the header as being at least a pulse */
typedef struct _pulse msg_header_t;


#define SIZE 512
typedef struct  {
    int  typ;       // typ zlecenia
    int  ile;       // liczba bajtow
    int  fh;        // uchwyt pliku
    char buf[SIZE]; // bufor
} mms;



/*** Client Side of the code ***/
int client() {
    mms msg;
    mms rmsg;

    int server_coid;

    if ((server_coid = name_open(ATTACH_POINT, 0)) == -1) {
        return EXIT_FAILURE;
    }

    /* We would have pre-defined data to stuff here */
    //msg.hdr.type = 0x00;
    //msg.hdr.subtype = 0x00;

    /* Do whatever work you wanted with server connection */
    //for (msg.data=0; msg.data < 5; msg.data++) {
     //   printf("Client sending %d \n", msg.data);
    msg.typ = OPENR;

    strcpy(msg.buf, "plik.txt");
        if (MsgSend(server_coid, &msg, sizeof(msg), &rmsg, sizeof(rmsg))== -1) {
        	perror("MsgSend");
            exit(EXIT_FAILURE);
        }

        printf("received file handler in client : %d\n", rmsg.fh);
        FILE *file = rmsg.fh;

        printf("client receive: %s\n ile : %d", rmsg.buf, rmsg.ile);



    //}
    //client send CLOSE
        msg.typ = CLOSE;
        if (MsgSend(server_coid, &msg, sizeof(msg), &rmsg, sizeof(rmsg))== -1) {
                   	perror("MsgSend");
                       exit(EXIT_FAILURE);
                   }
        /* Close the connection */
    name_close(server_coid);


    return EXIT_SUCCESS;
}

int server() {
   name_attach_t *attach;
   mms msg;
   mms rmsg;
   int rcvid;

   /* Create a local name (/dev/name/local/...) */
   if ((attach = name_attach(NULL, ATTACH_POINT, 0)) == NULL) {
       return EXIT_FAILURE;
   }

   /* Do your MsgReceive's here now with the chid */
   while (1) {
       rcvid = MsgReceive(attach->chid, &msg, sizeof(msg), NULL);

       if (rcvid == -1) {/* Error condition, exit */
           break;
       }




       /* A message (presumable ours) received, handle */
       printf("Server receive %s, with type : %d \n", msg.buf, msg.typ);
       FILE *file;



       switch(msg.typ){
       case OPENR:
    	   file = fopen(msg.buf, "r");
    	   printf("file opened");
    	   break;
       case CLOSE:
    	   fclose(file);
    	   break;
       default:
    	   break;

        }

       printf("file handle in server : %d\n", &file);
       rmsg.fh = &file;

       int c = 0;

     /*  if (file) {
           while ((c = getc(file)) != EOF)
               putchar(c);
           fclose(file);
       }
     */




      // if (file) {
        //   while ((nread = fread(rmsg.buf, 1, sizeof (rmsg.buf), file)) > 0)
               //fwrite(buf, 1, nread, stdout);

           //if (ferror(file)) {
               /* deal with error */
         //  }
           //fclose(file);
      // }
      // fread(rmsg.buf, 1, SIZE, file);

       size_t nread;
       /* otherwise do this.  Note 'chunk' instead of 'sizeof buf' */
       int counter = 0;
       while ((nread = fread(rmsg.buf, 1, SIZE, file)) > 0) {
    	   counter = nread;
           /* as above */
       }


       printf("counter = %d", counter);
       rmsg.ile = counter;

       //printf("i really read : %d", nread);
       //printf("buffer : %s\n", rmsg.buf );

    if(counter > 0)
     printf("server send response with data chunk\n");

       MsgReply(rcvid, EOK, &rmsg, sizeof(rmsg));

   }


   /* Remove the name from the space */
   name_detach(attach, 0);

   return EXIT_SUCCESS;
}

int main(int argc, char **argv) {
    int ret;

    if (argc < 2) {
        printf("Usage %s -s | -c \n", argv[0]);
        ret = EXIT_FAILURE;
    }
    else if (strcmp(argv[1], "-c") == 0) {
        printf("Running Client ... \n");
        ret = client();   /* see name_open() for this code */
    }
    else if (strcmp(argv[1], "-s") == 0) {
        printf("Running Server ... \n");
        ret = server();   /* see name_attach() for this code */
    }
    else {
        printf("Usage %s -s | -c \n", argv[0]);
        ret = EXIT_FAILURE;
    }
    return ret;
}
