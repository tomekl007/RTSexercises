#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/dispatch.h>
#include <sys/neutrino.h>
#include <signal.h>
#include <process.h>

#include "my_hdr.h"

#define MY_PULSE_CODE2   _PULSE_CODE_MINAVAIL




typedef union {
        struct _pulse   pulse;
        /* your other message structures would go
           here too */
} my_message_t;




/*** Client Side of the code ***/
void *client(void *arg) {

	int timer = (int*)arg;
	printf("client get %d : ", timer);

	struct sigevent         event;
	struct itimerspec       itime;//The timespec structure specifies a time in seconds and nanoseconds.
    timer_t                 timer_id;







  int chid, coid, srv_coid, rcvid;
  struct my_msg msg;
  struct _pulse pulse;

  /* we need a channel to receive the pulse notification on */
  chid = ChannelCreate( 0 );

  /* and we need a connection to that channel for the pulse to be
     delivered on */
  coid = ConnectAttach( 0, 0, chid, _NTO_SIDE_CHANNEL, 0 );

  /* fill in the event structure for a pulse */
  SIGEV_PULSE_INIT( &msg.event, coid, SIGEV_PULSE_PRIO_INHERIT,
                    MY_PULSE_CODE, 0 );
  msg.type = MSG_GIVE_PULSE;

  /* find the server */
  if ( (srv_coid = name_open( MY_SERV, 0 )) == -1)
  {
     printf("failed to find server, errno %d\n", errno );
     exit(1);
  }

  /* give the pulse event we initialized above to the server for
     later delivery */
  MsgSend( srv_coid, &msg, sizeof(msg), NULL, 0 );

  /* wait for the pulse from the server */
  rcvid = MsgReceivePulse( chid, &pulse, sizeof( pulse ), NULL );
  printf("got pulse with code %d, waiting for %d\n", pulse.code,
         MY_PULSE_CODE );


  //tutaj obsluze timer
  my_message_t            msg2;


  event.sigev_notify = SIGEV_PULSE;
         event.sigev_coid = ConnectAttach(0, 0,
                                          chid,
                                          _NTO_SIDE_CHANNEL, 0);

         //w kazdym z watkow tworze timer o interval = arg ktory dostalem z main
         //i priority = ten sam arg
         event.sigev_priority = timer;
         event.sigev_code = MY_PULSE_CODE2;
         timer_create(CLOCK_REALTIME, &event, &timer_id);
         //&event - pointer to a sigevent structure containing the event that you want to deliver when the timer fires.
         //&timer_id - A pointer to a timer_t object where the function stores the ID of the new timer.

         itime.it_value.tv_sec = 0;
         /* 500 million nsecs = .5 secs */
         itime.it_value.tv_nsec = 00000001;

         itime.it_interval.tv_sec = timer;
         /* 500 million nsecs = .5 secs */
         //itime.it_interval.tv_nsec = 500000000;
         timer_settime(timer_id, 0, &itime, NULL);



         //w pentli msgReceive ktory czeka na msg z pulse.come == MY_PULSE_CODE
         for (;;) {
               rcvid = MsgReceive(chid, &msg2, sizeof(msg2), NULL);
               if (rcvid == 0) { /* we got a pulse */

                    if (msg2.pulse.code == MY_PULSE_CODE2) {
                    	struct sched_param param;
                    	         sched_getparam(0, &param);

                        //printf("we got a pulse from our timer, now i send : %d element of array\n", i);
                        //pthread_create(&tid,NULL,dummyFunction, NULL);//(void*) zadania[i+1]
                      //  i ++;
        printf("odebralem puls w watku : %d o priority : %d\n",pthread_self(),param.sched_curpriority );

                      //  pthread_mutex_unlock( &mutex );

                    } /* else other pulses ... */
               } /* else other messages ... */
           }






  return 0;

}

int server() {

	int zadania[7];

	zadania[0] = 10;
	zadania[1] = 5;
	zadania[2] = 2;

	int rcvid;
	  struct my_msg msg;
	  int tid;
	  name_attach_t *attach;

	  /* attach the name the client will use to find us */
	  /* our channel will be in the attach structure */
	  if ( (attach = name_attach( NULL, MY_SERV, 0 )) == NULL)
	  {
	     printf("server:failed to attach name, errno %d\n", errno );
	     exit(1);
	  }

int i = 0;

	  for(i = 0; i < 3 ;i++){
	  pthread_create(&tid,NULL,client, (void*) zadania[i]);//(void*) zadania[i+1]
	  }
	 // client();
	  /* wait for the message from the client */
	  while(1){
	  rcvid = MsgReceive( attach->chid, &msg, sizeof( msg ), NULL );
	  MsgReply(rcvid, 0, NULL, 0);
	  if ( msg.type == MSG_GIVE_PULSE )
	  {
	     /* wait until it is time to notify the client */
	     sleep(2);

	     /* deliver notification to client that client requested */
	     MsgDeliverEvent( rcvid, &msg.event );
	     printf("server:delivered event\n");
	  } else
	  {
	    printf("server: unexpected message \n");
	  }

	  }

	  return 0;
}

int main(int argc, char **argv) {
    int ret;




    if (argc < 2) {
        printf("Usage %s -s | -c \n", argv[0]);
    //    ret = client();
        //ret = EXIT_FAILURE;
    }
   // else if (strcmp(argv[1], "-c") == 0) {
  //      printf("Running Client ... \n");
  //      ret = client();   /* see name_open() for this code */
  //  }
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
