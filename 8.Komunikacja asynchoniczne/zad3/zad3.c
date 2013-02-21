#include <mqueue.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include <string.h>
#include <sys/neutrino.h>
#include <sys/netmgr.h>

#define MSG_SIZE      4096

char * queue_dev = "q_dev";
char * queue2 = "q2";
char * queue3 = "q3";


// This handler will be called when the queue
// becomes non-empty.
void handler (int sig_num) {
    printf ("Received sig %d.\n", sig_num);
}

typedef struct  {
   int  typ;        // typ komunikatu
   int  from;       // nr procesu kt�ry wys�a� komunikat
   int  ile;        // ile by�o malych liter
   char text[256]; // tekst komunikatu
 } mss_t;

void device()
{
	  struct mq_attr attr;
	  mqd_t out;             // Message queue descriptors
	  char buf[MSG_SIZE];              // A good-sized buffer
	  unsigned int prio;               // Priority

	  attr.mq_maxmsg = 300;
	  attr.mq_msgsize = MSG_SIZE;
	  attr.mq_flags = 0;

	  // Open a queue with the attribute structure
	  out = mq_open (queue_dev, O_WRONLY | O_CREAT, 0664, &attr);

	  int a;
	  int priority = 15;

	  while(1)
	  {
	    printf ("device writing a message with priority %d.\n", priority);
	    if (mq_send (out, "message", strlen("message"), prio) == -1)
	      perror ("mq_send()");

	    sleep(5);
	  }

	  // Close all open message queue descriptors
	  mq_close (out);
}
/*
void p2()
{
	mss_t msg;

	struct mq_attr attr, old_attr;   // To store queue attributes
	  struct sigevent sigevent;        // For notification
	  mqd_t in, out;             // Message queue descriptors
	  char buf[MSG_SIZE];              // A good-sized buffer
	  unsigned int prio;               // Priority

	  // First we need to set up the attribute structure
	  attr.mq_maxmsg = 300;
	  attr.mq_msgsize = MSG_SIZE;
	  attr.mq_flags = 0;

	  // We want to be notified when something is there

	  // Open a queue with the attribute structure
	  in = mq_open (queue_dev, O_RDONLY,
	                   0664, &attr);

	  out = mq_open (queue2, O_WRONLY | O_CREAT, 0664, &attr);

	  puts("-1-");

	  int chid = ChannelCreate(0);
	  int coid = ConnectAttach(ND_LOCAL_NODE, getpid(), chid, _NTO_SIDE_CHANNEL, 0);

	  SIGEV_PULSE_INIT(&sigevent,coid,1,1,0);
//	  signal (SIGUSR2, handler);
//	  sigevent.sigev_signo = SIGUSR2;

	  puts("-1.2-");

	  if (mq_notify (queue_dev, &sigevent) == -1) {
	    if (errno == EBUSY)
	      printf ("Another process has registered for notification.\n");
	    else if(errno == EBADF)
		      printf ("bad queue\n");

	    _exit (EXIT_FAILURE);
	  }

	  char new_msg[50];
	  puts("-2-");

	  int rcvid;

	    while (1)
	    {
	    	rcvid=MsgReceive(chid,&msg,sizeof(msg),NULL);
	    	        if (rcvid==0)
	    	        {
	    		    	printf ("p2 received a message with priority %d, sending further\n", prio);
	    	            mq_receive(in,&buf,sizeof(buf),&prio);

	    		    	sprintf(new_msg, "%s-2", buf[0]);

//	    		    	if (mq_send (out, "message", 4, prio) == -1)
//	    		    		      perror ("mq_send()");
	    	        }
	    }

	    // The call failed.  Make sure errno is EAGAIN
	    if (errno != EAGAIN) {
	      perror ("mq_receive()");
	      _exit (EXIT_FAILURE);
	    }

	  mq_close (in);
	  mq_close (out);

	  puts("p2 end");
}
*/

void p2()
{
	  struct mq_attr attr, old_attr;   // To store queue attributes
	  struct sigevent sigevent;        // For notification
	  mqd_t in, out;             // Message queue descriptors
	  char buf[MSG_SIZE + 20];              // A good-sized buffer
	  unsigned int prio;               // Priority

	  // First we need to set up the attribute structure
	  attr.mq_maxmsg = 300;
	  attr.mq_msgsize = MSG_SIZE;
	  attr.mq_flags = 0;

	  // We want to be notified when something is there

	  // Open a queue with the attribute structure
	  in = mq_open (queue_dev, O_RDONLY,
	                   0664, &attr);

	  out = mq_open (queue2, O_WRONLY | O_CREAT, 0664, &attr);

	  signal (SIGUSR1, handler);
	  sigevent.sigev_signo = SIGUSR1;

	  if (mq_notify (in, &sigevent) == -1) {
	    if (errno == EBUSY)
	      printf (
	        "Another process has registered for notification.\n");
	    _exit (EXIT_FAILURE);
	  }

	  char new_msg[50];

	    while (mq_receive (in, &buf[0], MSG_SIZE + 10, &prio) != -1)
	    {
	    	printf ("p2 received a message %s with priority %u, sending further\n", buf, prio);

	    	sprintf(new_msg, "%s-2", buf);

	    	if (mq_send (out, new_msg, strlen(new_msg), prio) == -1)
	    		      perror ("mq_send()");
	    }

	    // The call failed.  Make sure errno is EAGAIN
	    if (errno != EAGAIN) {
	      perror ("---mq_receive()");
	      _exit (EXIT_FAILURE);
	    }

	  mq_close (in);
	  mq_close (out);

	  puts("p2 end");
}


void p3()
{
	  struct mq_attr attr, old_attr;   // To store queue attributes
	  struct sigevent sigevent;        // For notification
	  mqd_t in, out;             // Message queue descriptors
	  char buf[MSG_SIZE];              // A good-sized buffer
	  unsigned int prio;               // Priority

	  // First we need to set up the attribute structure
	  attr.mq_maxmsg = 300;
	  attr.mq_msgsize = MSG_SIZE;
	  attr.mq_flags = 0;

	  // We want to be notified when something is there

	  // Open a queue with the attribute structure
	  in = mq_open (queue2, O_RDONLY,
	                   0664, &attr);

	  out = mq_open (queue3, O_WRONLY | O_CREAT, 0664, &attr);

	  signal (SIGUSR2, handler);
	  sigevent.sigev_signo = SIGUSR2;

	  if (mq_notify (in, &sigevent) == -1) {
	    if (errno == EBUSY)
	      printf (
	        "Another process has registered for notification.\n");
	    _exit (EXIT_FAILURE);
	  }

	  char new_msg[50];

	    while (mq_receive (in, &buf[0], MSG_SIZE, &prio) != -1)
	    {
	    	printf ("p3 received a message %s with priority %u, sending further\n", buf, prio);

	    	sprintf(new_msg, "%s-3", buf);

	    	if (mq_send (out, new_msg, strlen(new_msg), prio) == -1)
	    		      perror ("mq_send()");
	    }

	    // The call failed.  Make sure errno is EAGAIN
	    if (errno != EAGAIN) {
	      perror ("mq_receive()");
	      _exit (EXIT_FAILURE);
	    }

	  mq_close (in);
	  mq_close (out);

	  puts("p3 end");
}

void p4()
{
	  struct mq_attr attr, old_attr;   // To store queue attributes
	  struct sigevent sigevent;        // For notification
	  mqd_t in;             // Message queue descriptors
	  char buf[MSG_SIZE];              // A good-sized buffer
	  unsigned int prio;               // Priority

	  // First we need to set up the attribute structure
	  attr.mq_maxmsg = 300;
	  attr.mq_msgsize = MSG_SIZE;
	  attr.mq_flags = 0;

	  // We want to be notified when something is there

	  // Open a queue with the attribute structure
	  in = mq_open (queue3, O_RDONLY,
	                   0664, &attr);

	  signal (SIGUSR1, handler);
	  sigevent.sigev_signo = SIGUSR1;

	  if (mq_notify (in, &sigevent) == -1) {
	    if (errno == EBUSY)
	      printf (
	        "Another process has registered for notification.\n");
	    _exit (EXIT_FAILURE);
	  }

	  char new_msg[50];

	    while (mq_receive (in, &buf[0], MSG_SIZE, &prio) != -1)
	    {
	    	printf ("p4 received a message %s with priority %u\n", buf, prio);

	    }

	    // The call failed.  Make sure errno is EAGAIN
	    if (errno != EAGAIN) {
	      perror ("mq_receive()");
	      _exit (EXIT_FAILURE);
	    }

	  mq_close (in);

	  puts("p4 end");
}

int main(int argc, char** argv)
{
	puts("start");

	if(argc != 2)
	{
		puts("1 argument, 1,2,3 or 4");
		return -1;
	}

	if(strcmp(argv[1], "1") == 0)
	{
		puts("device");
		device();
	}
	else if(strcmp(argv[1], "2") == 0)
	{
		puts("p2");
		p2();
	}
	else if(strcmp(argv[1], "3") == 0)
	{
		puts("p3");
		p3();
	}
	else if(strcmp(argv[1], "4") == 0)
	{
		puts("p4");
		p4();
	}


}
