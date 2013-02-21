#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/dispatch.h>
#include <sys/neutrino.h>

#define ATTACH_POINT "myname"


#define PULSE_PRIORITY 10
#define PULSE_CODE _PULSE_CODE_DISCONNECT

int count = 0;

/* We specify the header as being at least a pulse */
typedef struct _pulse msg_header_t;


/* Our real data comes after the header */
typedef struct listNode {
    struct listNode* next_ptr;
    int scoid;
    char text_description[64];
} listNode;

typedef struct _my_data {
    msg_header_t hdr;
    int data;
} my_data_t;

//struct element
//{
  // int x;
  // struct element *next;
//};


void dodaj(listNode **lista, int y)
{
	listNode *e, *tmp;

     e = malloc(sizeof(listNode));
     e->scoid=y;
     printf("dodaj : e->scoid = %d",e->scoid);
     //atomic_add(&count, 1);

     if (*lista == NULL)
      {
    	 //jesli lista ktora przekazalem jest pusta
    	 //to e bedzie wsadozny jako pierwszy node tej listy
        e->next_ptr = *lista;
        *lista = e;//may not need
      }
      else
       {
         tmp = *lista;
         while (tmp->next_ptr != NULL)//tak dlugo az nie dojde do konca tej listy
            tmp=tmp->next_ptr;

            tmp->next_ptr = e;//jak juz jestem na koncu to dodaje e
            e->next_ptr = NULL;//i ustawiam e->next_ptr na NULL bo to ost. element
       }
}



void wyswietl(listNode *lista)
{
   printf("\n\n");
   while(lista)
   {
     printf("[ %i ] ->", lista->scoid);
     lista=lista->next_ptr;
   }
   printf("[NULL]");
   getchar();
   getchar();
}

int szukaj(listNode *lista, int element){
	while(lista){
		if(lista->scoid == element)
			return 1;
		lista=lista->next_ptr;
	}

}



/*void kasuj(listNode *lista)
{
  listNode *e;
  while(!lista)
   {
     e=lista->next;
     free(lista);
     lista=e;
   }
  printf("\n\nlista skasowana");
}*/

void kasujElement(listNode *lista, int scoid){
	while(lista){
			if(lista->scoid == scoid){
				printf("wykasuje element : %d\n", lista->scoid);
				//atomic_sub(count,1);
				lista->scoid = 0;
				return 0;
			}
			lista=lista->next_ptr;
		}


}










/*** Client Side of the code ***/
int client() {
	int status;       //status return value used for ConnectAttach and MsgSend
    my_data_t msg;
    int server_coid;

    if ((server_coid = name_open(ATTACH_POINT, 0)) == -1) {
        return EXIT_FAILURE;
    }
   // printf("scoid : %d", server_coid ); to jest servera

    /* We would have pre-defined data to stuff here */
    msg.hdr.type = 0x00;
    msg.hdr.subtype = 0x00;

    /* Do whatever work you wanted with server connection */
    for (msg.data=0; msg.data < 5; msg.data++) {
        printf("Client sending %d \n", msg.data);
        sleep(2);
        if (MsgSend(server_coid, &msg, sizeof(msg), NULL, 0) == -1) {
        	break;
        }
    }


if(ConnectDetach(server_coid)){//nie jestem pewien czy tak
    status=MsgSendPulse(server_coid,PULSE_PRIORITY,PULSE_CODE,getpid());
          if (-1==status)
          {
              perror("MsgSendPulse");
              exit(EXIT_FAILURE);
          }
          printf("MsgSendPulse status: %d\n",status);
}

    /* Close the connection */
    name_close(server_coid);

    return EXIT_SUCCESS;
}

int server() {
   struct _msg_info info;
   name_attach_t *attach;
   listNode *listNode;//, *e;
   listNode = NULL;

   struct _pulse *mypulse;

   my_data_t msg;
   int rcvid;

   /* Create a local name (/dev/name/local/...) */
   if ((attach = name_attach(NULL, ATTACH_POINT, 0)) == NULL) {
       return EXIT_FAILURE;
   }


   int i = 0;
   /* Do your MsgReceive's here now with the chid */
   while (1) {
       rcvid = MsgReceive(attach->chid, &msg, sizeof(msg), &info);



       //listNode.scoid = info.scoid;
     ////  printf("listNode.scoid : %d \n", listNode.scoid);
      // }else if{
    	//   listNode.next_ptr->scoid = info.scoid;
    //	   printf("listNode.scoid : %d \n", listNode.scoid);

     //  }






       if (rcvid == -1) {/* Error condition, exit */
           break;
       }

       if (rcvid == 0) {/* Pulse received */
           switch (msg.hdr.code) {
           case _PULSE_CODE_DISCONNECT:
               /*
                * A client disconnected all its connections (called
                * name_close() for each name_open() of our name) or
                * terminated
                */
        	   mypulse =(struct _pulse *)&msg;
        	   printf("Pulse received from %d, pulse code: %d, pulse value %d:\n",
        			   info.pid,mypulse->code,mypulse->value.sival_int);

        	   printf("pulse scoid : %d\n", mypulse->scoid);

               ConnectDetach(mypulse->scoid);
               kasujElement(&listNode,mypulse->scoid);
               break;
           case _PULSE_CODE_UNBLOCK:
               /*
                * REPLY blocked client wants to unblock (was hit by
                * a signal or timed out).  It's up to you if you
                * reply now or later.
                */
               break;
           default:
               /*
                * A pulse sent by one of your processes or a
                * _PULSE_CODE_COIDDEATH or _PULSE_CODE_THREADDEATH
                * from the kernel?
                */
               break;
           }
           continue;
       }

       /* name_open() sends a connect message, must EOK this */
       if (msg.hdr.type == _IO_CONNECT ) {
           MsgReply( rcvid, EOK, NULL, 0 );
           continue;
       }

       /* Some other QNX IO message was received; reject it */
       if (msg.hdr.type > _IO_BASE && msg.hdr.type <= _IO_MAX ) {
           MsgError( rcvid, ENOSYS );
           continue;
       }

       /* A message (presumable ours) received, handle */
       printf("Server receive %d \n", msg.data);


       printf("info.scoid : %d \n", info.scoid);//to jest clienta
       //if(listNode->scoid != info.scoid){

       //dodawanie scoid do listy
       if(szukaj(&listNode,info.scoid)!= 1)
       dodaj(&listNode, info.scoid);



       MsgReply(rcvid, EOK, 0, 0);
     //  wyswietl(listNode);

   }


   /* Remove the name from the space */
   name_detach(attach, 0);
  /// printf("afrer all list is : \n");
  // wyswietl(listNode);

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
