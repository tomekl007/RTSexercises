#include <mqueue.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#define MSG_SIZE       4096


char * queue_name = "my_queue_zad1";

// This handler will be called when the queue
// becomes non-empty.
void handler (int sig_num) {
    printf ("Received sig %d.\n", sig_num);
}

void writer()
{
	  struct mq_attr attr;
	  mqd_t mqdes;             // Message queue descriptors
	  char buf[MSG_SIZE];              // A good-sized buffer
	  unsigned int prio;               // Priority

	  attr.mq_maxmsg = 300;
	  attr.mq_msgsize = MSG_SIZE;
	  attr.mq_flags = 0;

	  // Open a queue with the attribute structure
	  mqdes = mq_open (queue_name, O_WRONLY | O_CREAT, 0664, &attr);

	  int a;
	  int priority = 15;

	  while(1)
	  {
	    printf ("Writing a message with priority %d.\n", priority);
	    if (mq_send (mqdes, "message", 4, prio) == -1)
	      perror ("mq_send()");

	    sleep(1);
	  }

	  // Close all open message queue descriptors
	  mq_close (mqdes);
}

void reader()
{
	  struct mq_attr attr, old_attr;   // To store queue attributes
	  struct sigevent sigevent;        // For notification
	  mqd_t mqdes, mqdes2;             // Message queue descriptors
	  char buf[MSG_SIZE];              // A good-sized buffer
	  unsigned int prio;               // Priority

	  // First we need to set up the attribute structure
	  attr.mq_maxmsg = 300;
	  attr.mq_msgsize = MSG_SIZE;
	  attr.mq_flags = 0;

	  // We want to be notified when something is there

	  // Open a queue with the attribute structure
	  mqdes = mq_open (queue_name, O_RDONLY,
	                   0664, &attr);

	  signal (SIGUSR1, handler);
	  sigevent.sigev_signo = SIGUSR1;

	  if (mq_notify (mqdes, &sigevent) == -1) {
	    if (errno == EBUSY)
	      printf (
	        "Another process has registered for notification.\n");
	    _exit (EXIT_FAILURE);
	  }

	    while (mq_receive (mqdes, &buf[0], MSG_SIZE, &prio) != -1)
	    {
	      printf ("Received a message with priority %d.\n", prio);
	    }
	    // The call failed.  Make sure errno is EAGAIN
	    if (errno != EAGAIN) {
	      perror ("mq_receive()");
	      _exit (EXIT_FAILURE);
	    }

	  mq_close (mqdes);
}

int main(int argc, char** argv)
{
	puts("start---");

	if(argc != 2)
	{
		puts("1 argument, -w or -r");
		return -1;
	}

	if(strcmp(argv[1], "-r") == 0)
	{
		puts("reader");
		reader();
	}
	else if(strcmp(argv[1], "-w") == 0)
	{
		puts("writer");
		writer();
	}
}
