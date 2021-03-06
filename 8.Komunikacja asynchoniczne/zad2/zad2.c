#include <mqueue.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/dispatch.h>

#define MSG_SIZE       4096

char * queue_name = "my_queue";
#define ATTACH_POINT "myname"

/* Our real data comes after the header */
typedef struct _my_data {
    int data;
} my_data_t;


// This handler will be called when the queue
// becomes non-empty.
void handler (int sig_num) {
    printf ("Received sig %d.\n", sig_num);
}

void p1()
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

void p2()
{
    my_data_t msg, msg_in;
    int server_coid;

    if ((server_coid = name_open(ATTACH_POINT, 0)) == -1) {
        return EXIT_FAILURE;
    }

    int number = 0;

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
	    	printf ("Received a queue message with priority %d.\n", prio);

	        msg.data = number;

	    	printf("Client sending %d \n", msg.data);
	        //MsgSend(coid, &outgoing_msg, sizeof(outgoing_msg),&incoming_msg, sizeof(incoming_msg) );
	    	if (MsgSend(server_coid, &msg, sizeof(msg), &msg_in, sizeof(msg_in)) == -1)
	    	{
	            break;
	        }

	        printf("reply: %d\n", msg_in.data);

	        ++number;
	      }

	    // The call failed.  Make sure errno is EAGAIN
	    if (errno != EAGAIN) {
	      perror ("mq_receive()");
	      _exit (EXIT_FAILURE);
	    }

	  mq_close (mqdes);

	    /* Close the connection */
	    name_close(server_coid);

}

void p3()
{
	   name_attach_t *attach;
	   my_data_t msg, msg_reply;
	   int rcvid;

	   if ((attach = name_attach(NULL, ATTACH_POINT, 0)) == NULL) {
	       return EXIT_FAILURE;
	   }

	   while (1) {
	       rcvid = MsgReceive(attach->chid, &msg, sizeof(msg), NULL);

	       if (rcvid == -1) {/* Error condition, exit */
	           puts("error");
	    	   break;
	       }

	       printf("Server receive %d \n", msg.data);

	       msg_reply.data = -msg.data;

	       MsgReply(rcvid, EOK, &msg_reply, sizeof(msg_reply));
	   }

	   /* Remove the name from the space */
	   name_detach(attach, 0);

	   return EXIT_SUCCESS;
}

int main(int argc, char** argv)
{
//	puts("start---");

	if(argc != 2)
	{
		puts("1 argument, 1, 2 or 3");
		return -1;
	}

	printf("argument: %s\n", argv[1]);

	if(strcmp(argv[1], "1") == 0)
	{
		puts("process 1");
		p1();
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
}
