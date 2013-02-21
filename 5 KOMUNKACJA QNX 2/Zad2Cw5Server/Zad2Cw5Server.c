/* server.c */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/neutrino.h>
#include <sys/iomsg.h>
#include <sys/iofunc.h>
#include <sys/dispatch.h>

#include "my_hdr.h"

pthread_mutex_t setReceiveIdMutex = PTHREAD_MUTEX_INITIALIZER;

pthread_t tid;

void *generateEvents(void *arg){
	printf("in thread %d ",pthread_self());
	//pthread_mutex_lock(&setReceiveIdMutex);
	my_msg *msg;
	msg = (my_msg*)arg;
	//= (my_msg*)arg;
	sleep(1);
	//int rcvid = msg->rcvid;
	printf("\nin generate events msg->rcvid = %d ",msg->rcvid);
	printf("&msg->event : %d ",msg->event);

	int ret = MsgDeliverEvent( msg->rcvid, &msg->event );
    printf("server:delivered event, return  : %d\n",ret);
    //pthread_mutex_unlock(&setReceiveIdMutex);
    pthread_exit(0);


return 0;
}


//pthread_t tid[5];
int main( int argc, char **argv)
{
	//printf("MSG_GIVE_PULSE : %d, MY_PULSE_CODE :%d", MSG_GIVE_PULSE ,MY_PULSE_CODE );

int i = 0;//
  int rcvid;
  struct my_msg msg[100];//for every thread i define new msg i assume that will be 100 tread
  name_attach_t *attach;
  my_msg msg2;
  int status;

  /* attach the name the client will use to find us */
  /* our channel will be in the attach structure */
  if ( (attach = name_attach( NULL, MY_SERV, 0 )) == NULL)
  {
     printf("server:failed to attach name, errno %d\n", errno );
     exit(1);
  }

  while(1){
   /* wait for the message from the client */
  // pthread_mutex_lock(&setReceiveIdMutex);
   rcvid = MsgReceive( attach->chid, &msg[i], sizeof( msg[0] ), NULL );
   MsgReply(rcvid, 0, NULL, 0);
   printf("receiveId : %d and msg.type  :%d ", rcvid, msg[i].type);
   if ( msg[i].type == MSG_GIVE_PULSE )//typ msg zgadza sie
   {

      /* wait until it is time to notify the client */
      sleep(2);
      //wiec moge dostarczyc event

      msg[i].rcvid = rcvid;
      printf("\n--->>>in main set msg.rcvid = %d ", msg[i].rcvid);
     // pthread_mutex_unlock(&setReceiveIdMutex);
      /* deliver notification to client that client requested */
      /*Wykonując funkcję MsgDeliverEvent() serwer powoduje,
       * że system operacyjny dostarcza impuls do kanału komunikacyjnego A*/
      pthread_mutex_lock(&setReceiveIdMutex);
      pthread_create(&tid, NULL, generateEvents, (void*) &msg[i]);//&tid[i]
      i++;
      pthread_mutex_unlock(&setReceiveIdMutex);


   } else if(msg[i].type == 0){
	   printf("get msg.type == 0, rcvid : %d ",rcvid);


   }else if(msg[i].type == 1){
	   //
	   sleep(3);
	   printf("\n !!!!!    i receive normal message , type = %d", msg[i].type);
	   rcvid = MsgReceive(attach->chid, &msg2, sizeof(msg2), NULL);
	    i++;
	   		if(rcvid == -1) {
	   			perror("MsgReceive");
	   			break;
	   		}

	   		msg2.type = 100;
	   		printf("\nbefore send type = %d", msg2.type);
	  status = MsgReply(rcvid, EOK, &msg2, sizeof(msg2) );

   }else
   {
     printf("server: unexpected message \n");
   }


   printf("--cos w glownym");
  }

  return 0;
}
