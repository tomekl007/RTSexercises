/*
 * Demonstrate how to set up a timer that, on expiry,
 * sends us a pulse.  This example sets the first
 * expiry to 1.5 seconds and the repetition interval
 * to 1.5 seconds.
 */

#include <stdio.h>
#include <time.h>
#include <sys/netmgr.h>
#include <sys/neutrino.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>


#define MY_PULSE_CODE   _PULSE_CODE_MINAVAIL

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

typedef union {
        struct _pulse   pulse;
        /* your other message structures would go
           here too */
} my_message_t;


int nwd(int a, int b) // funkcja obliczająca nwd dwóch liczba - a i b
{
    if (b == 0) // jezeli b jest rowne 0, a jest najwieszym wspolnym dzielnikiem podanych liczb
		return a;
	// jezeli b jest rozne od zera wywolujemy funkcje rekurencyjnie dla b oraz a%b
	nwd(b,a%b);
}


void *dummyFunction(void *arg){
    int time = 0;
	time =  (int)arg;
	printf("in dummyFucntion with arg : %d \n", time);

	if(time > 2 || time < 0)
		time = 2;
	sleep(time);
	printf("dummyFunction end\n");

	return 0;


}

int main(void)
{
	int tid;


	int zadania[7];// w [0] deadline J1 w [1] czas wykonania

	zadania[0] = 4;
	zadania[1] = 1;
	zadania[2] = 5;
    zadania[3] = 2;
    zadania[4] = 20;
    zadania[5] = 1;
    zadania[6] = 20;
    zadania[7] = 2;
    //zadania[8] = 0;


    int hiperokres = 20;

    int rozmiarKlatki = 2;
    int iloscRamek = hiperokres/rozmiarKlatki;

   // int n = 4 * 5 / nwd(4,20);
   // printf("nwd : %d ", n);




   struct sigevent         event;
   struct itimerspec       itime;//The timespec structure specifies a time in seconds and nanoseconds.
   timer_t                 timer_id;
   int                     chid;
   int                     rcvid;
   my_message_t            msg;

   chid = ChannelCreate(0);

   event.sigev_notify = SIGEV_PULSE;
   event.sigev_coid = ConnectAttach(ND_LOCAL_NODE, 0,
                                    chid,
                                    _NTO_SIDE_CHANNEL, 0);
   event.sigev_priority = getprio(0);
   event.sigev_code = MY_PULSE_CODE;
   timer_create(CLOCK_REALTIME, &event, &timer_id);
   //&event - pointer to a sigevent structure containing the event that you want to deliver when the timer fires.
   //&timer_id - A pointer to a timer_t object where the function stores the ID of the new timer.

   itime.it_value.tv_sec = 0;
   /* 500 million nsecs = .5 secs */
   itime.it_value.tv_nsec = 00000001;
   itime.it_interval.tv_sec = rozmiarKlatki;
   /* 500 million nsecs = .5 secs */
   //itime.it_interval.tv_nsec = 500000000;
   timer_settime(timer_id, 0, &itime, NULL);
   //timer_id = A timer_t object that holds a timer ID, as set by timer_create().
   //&itime = A pointer to a itimerspec structure that specifies the value that you want to set for the timer's time until expiry. For more information, see timer_gettime()

   /*
    * As of the timer_settime(), we will receive our pulse
    * in 1.5 seconds (the itime.it_value) and every 1.5
    * seconds thereafter (the itime.it_interval)
    */


   int i = 0;
   int j = 0;

   for (;;) {
       rcvid = MsgReceive(chid, &msg, sizeof(msg), NULL);
       if (rcvid == 0) { /* we got a pulse */
 msg.pulse.
            if (msg.pulse.code == MY_PULSE_CODE) {
            	  //pthread_mutex_lock( &mutex );
            	  if(j == iloscRamek){
            	        i = 0;
            	        j = 0;
            	    }
                printf("we got a pulse from our timer, now i send : %d element of array\n", i+1);
                pthread_create(&tid,NULL,dummyFunction,(void*) zadania[i+1] );
                i +=2;
                j++;

              //  pthread_mutex_unlock( &mutex );

            } /* else other pulses ... */
       } /* else other messages ... */
   }
}
