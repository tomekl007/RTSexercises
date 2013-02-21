/* client.c */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/neutrino.h>
#include <sys/iomsg.h>

#include "my_hdr.h"

int chid, rcvid;
struct _pulse pulse;


void* receivePulse(void *arg){
	while(1){

	printf("receive Pulse, thread id : %d\n", pthread_self() );
	rcvid = MsgReceivePulse( chid, &pulse, sizeof( pulse ), NULL );
	 printf("got pulse with code %d,while i was waiting for %d, rcvid : %d\n", pulse.code,
	         MY_PULSE_CODE, rcvid );
	 //return 0;
	}
}



int main( int argc, char **argv)
{
  int coid, srv_coid; //chid,rcvid
  struct my_msg msg;
  struct _pulse pulse;
  int flag = 1;
  pthread_t tid;
  int status;
  int count = 0;

  /* we need a channel to receive the pulse notification on */
  chid = ChannelCreate( 0 );
  /*Zgodnie z opisem w dokumentacji funkcji MsgDeliverEvent(), aby serwer mógł wysyłać
   *  zdarzenia do klienta, klient tworzy kanał komunikacyjny (nazwijmy go A),którym
   *   łączy się z samym sobą, */

  /* and we need a connection to that channel for the pulse to be
     delivered on */
  coid = ConnectAttach( 0, 0, chid, _NTO_SIDE_CHANNEL, 0 );

  /* fill in the event structure for a pulse */

  /*identyfikator połączenia na tym kanale(coid) (zwracany przez funkcję ConnectAttach())
   *  jest używany do inicjalizacji zdarzenia o typie struktury sigevent
   *  (makro SIGEV_PULSE_INIT()),*/
  while(1){
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
  /*a samo zdarzenie jest przesyłane do serwera zwykłym komunikatem (MsgSend())
   * przez kolejny kanał komunikacyjny. */
  MsgSend( srv_coid, &msg, sizeof(msg), NULL, 0 );
  printf("wait for msgReveivePulse");

  //to zostaje oddelegowane do watku pobocznego
  if(flag){

	  pthread_create(&tid, NULL, receivePulse, NULL);
	  flag = 0;
  }
  /* wait for the pulse from the server */
  //odbieram event z servera w channel(0) - "A",odbieram ten event jako pulse

  //tutaj moge robic dalej co chce
    if(count < 4){
    my_msg msg2;
    my_msg reply;
    msg2.type = 1;

    printf("cos w glownym");
    sleep(2);
    status = MsgSend(srv_coid, &msg2, sizeof(msg2), &reply, sizeof(reply) );
    		if(-1 == status) {
    			perror("MsgSend");
    			exit(EXIT_FAILURE);
    		}
    		sleep(2);

    printf("received type = %d\n", reply.type);
    printf("MsgSend return status: %d\n", status);
    count++;
    }


  }
  return 0;
}
