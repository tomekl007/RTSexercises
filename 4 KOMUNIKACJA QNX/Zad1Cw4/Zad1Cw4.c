#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/dispatch.h>
#include <ctype.h>


#define ATTACH_POINT "myname"
#define SIZE 24

/* We specify the header as being at least a pulse */
typedef struct _pulse msg_header_t;


/* Our real data comes after the header */
typedef struct _my_data {
    msg_header_t hdr;
    int data;
} my_data_t;

typedef struct  {
   int  typ;        // typ komunikatu
   int  from;       // nr procesu który wysłał komunikat
   int  ile;        // ile było malych liter
   char text[SIZE]; // tekst komunikatu
 } mss_t;



/*** Client Side of the code ***/
int client() {
    mss_t msg;
    int server_coid;
    mss_t rmsg;

    if ((server_coid = name_open(ATTACH_POINT, 0)) == -1) {
        return EXIT_FAILURE;
    }

    /* We would have pre-defined data to stuff here */
   // msg.hdr.type = 0x00;
    //msg.hdr.subtype = 0x00;
    msg.ile = 0;
    int typ = 1;
    msg.typ = typ;
    int from = getpid();
    msg.from = from;


    int i = 0;
    int count = 0;

    /* Do whatever work you wanted with server connection */

    	printf("what you want Send?");
    	scanf("%s", msg.text);

    	for(i = 0; i < strlen(msg.text); i++){
    		if(islower(msg.text[i]) )
    		count++;
    	}
    	printf("count : %d ", count);
    	msg.ile = count;

    	printf("Client sending msg, lower case letters: %d \n", msg.ile);

        if (MsgSend(server_coid, &msg, sizeof(msg), &rmsg, sizeof(rmsg)) == -1) {
        	perror("MsgSend");
        	exit(EXIT_FAILURE);
        }

        printf("received message %s from server %d\n",rmsg.text, rmsg.from);


    /* Close the connection */
    name_close(server_coid);
    return EXIT_SUCCESS;
}

int server() {
   name_attach_t *attach;
   mss_t msg;
   mss_t rmsg;

   //my_data_t msg;
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

      // if (rcvid == 0) {/* Pulse received */
       //    switch (msg.hdr.code) {
       //    case _PULSE_CODE_DISCONNECT:
               /*
                * A client disconnected all its connections (called
                * name_close() for each name_open() of our name) or
                * terminated
                */

       //        ConnectDetach(msg.hdr.scoid);
      //         break;
      //     case _PULSE_CODE_UNBLOCK:
               /*
                * REPLY blocked client wants to unblock (was hit by
                * a signal or timed out).  It's up to you if you
                * reply now or later.
       //         */
      //         break;
       //    default:
               /*
                * A pulse sent by one of your processes or a
                * _PULSE_CODE_COIDDEATH or _PULSE_CODE_THREADDEATH
                * from the kernel?
                */
         //      break;
        //   }
       //    continue;
     //  }

       /* name_open() sends a connect message, must EOK this */
 //      if (msg.hdr.type == _IO_CONNECT ) {
    //       MsgReply( rcvid, EOK, NULL, 0 );
    //       continue;
    //   }

       /* Some other QNX IO message was received; reject it */
      // if (msg.hdr.type > _IO_BASE && msg.hdr.type <= _IO_MAX ) {
     //      MsgError( rcvid, ENOSYS );
     //      continue;
     //  }

       /* A message (presumable ours) received, handle */
       printf("Server receive text :%s \n", msg.text);

       //czyszcze rmsg.text przed ponownym urzyciem
       memset(&rmsg.text[0], 0, sizeof(rmsg.text));

       rmsg.from = getpid();
       rmsg.typ = 1;
       rmsg.ile = 0;//tu modyf


       int i = 0;
      // for(i = 0; i < strlen(msg.text); i++)
      //       	rmsg.text[i] = msg.text[i];



       char letter ;
       for(i = 0; i < strlen(msg.text); i++){
       		//if(islower(msg.text[i]) ){
             letter = toupper(msg.text[i]);
           //  rmsg.text[i] = letter;//844
    	   rmsg.text[i]  = letter;
    			   //msg.text[i];
       		//}
       		//}
	}

       printf("server will send text : %s\n", rmsg.text);


       //rmsg.text[1] = 's';
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
