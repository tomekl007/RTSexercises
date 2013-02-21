#include <stdio.h>
#include <sys/neutrino.h>
#include <sys/syspage.h>
#include <process.h>


#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/neutrino.h>
#include <sys/iomsg.h>
#include <sys/iofunc.h>
#include <sys/dispatch.h>

#define MY_SERV "server_nameE"

#define MY_PULSE_CODE    SIGEV_INTR
struct sigevent event2;
//struct sigevent event;
volatile unsigned counter;

struct _pulse   pulse;



int thread_chid;

typedef struct my_msg
{
	short type;

}my_msg;



void *dummy(void *arg) {
	int srv_coid;
    my_msg msg;
    my_msg reply;

  printf("in thread : %d", pthread_self());
 /* find the server */
 if ( (srv_coid = name_open( MY_SERV, 0 )) == -1)
 {
    printf("failed to find server, errno %d\n", errno );
    exit(1);
 }
printf("msg sending\n");
 MsgSend( srv_coid, &msg, sizeof(msg), &reply, sizeof(reply));
 printf("\nThread %d unblock",pthread_self() );
 //MsgReceive();






	//MsgReceive(attach->chid, NULL, NULL, NULL );
	return 0;
}


const struct sigevent *handler( void *area, int id ) {
    // Wake up the thread every 100th interrupt
    if ( ++counter == 100 ) {
        counter = 0;
        return( &event2 );
        }
    else
        return( NULL );
    }


int main() {
	  my_msg msg;
	  my_msg reply;


	name_attach_t *attach;


		  /* attach the name the client will use to find us */
		  /* our channel will be in the attach structure */
		  if ( (attach = name_attach( NULL, MY_SERV, 0 )) == NULL)
		  {
		     printf("server:failed to attach name, errno %d\n", errno );
		     exit(1);
		  }
		  //mam server
//   rcvid =


	thread_chid = ChannelCreate(0);


    int i;
    int id;
    int chid,coid, rcvid;
    int tid;

int rcvidMsg[2];

    for(i = 0; i < 2 ;i++){
    	//powinienem wczesniej utrzoc te watki, i one maja czekac(blokowac sie)  na wiadomosc

    	  pthread_create(&tid,NULL,dummy,NULL );//(void*) zadania[i+1]
    	  rcvidMsg[i] = MsgReceive( attach->chid, &msg, sizeof( msg ), NULL );
    	  printf("msgReceive\n");

    }




    // Request I/O privileges
    ThreadCtl( _NTO_TCTL_IO, 0 );

    chid = ChannelCreate( 0 );

     coid = ConnectAttach( 0, 0, chid, _NTO_SIDE_CHANNEL, 0 );


    SIGEV_PULSE_INIT( &event2, coid, getprio(0), MY_PULSE_CODE, 0 );
    //SIGEV_INTR_INIT(&event2);

    // Initialize event structure
    //event.sigev_notify = SIGEV_INTR;

    //printf("%d", SYSPAGE_ENTRY(qtime)->intr);
    // Attach ISR vector
    id=InterruptAttach( SYSPAGE_ENTRY(qtime)->intr, &handler,
                        NULL, 0, 0 );

   // for( i = 0; i < 10; ++i ) {
        // Wait for ISR to wake us up
    //    InterruptWait( 0, NULL );

    //    printf( "100 events, event : %d\n",event2.sigev_notify );
    //    }


    //while(1){
    rcvid = MsgReceivePulse( chid, &pulse, sizeof( pulse ), NULL );

    if (pulse.code == MY_PULSE_CODE) {
    	printf("i get handle interrupt as pulse");
    	MsgReply(rcvidMsg[0], EOK, &reply, sizeof(reply) );
    	MsgReply(rcvidMsg[1], EOK, &reply, sizeof(reply) );
    }

    sleep(1);
   // }

    // Disconnect the ISR handler
    InterruptDetach(id);
    return 0;
    }
