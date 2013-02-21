#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

sig_atomic_t signal_count;

void MyHandler( int sig_number )
  {
	printf("in myHandler");
     ++signal_count;
  }

int main()
  {
    unsigned int  timeleft;

    printf( "Set the alarm and sleep\n" );
    alarm( 10 );
    sleep( 5 );   /* go to sleep for 5 seconds */

    /*
     * To get the time left before the SIGALRM is
     * to arrive, one must cancel the initial timer,
     * which returns the amount of time it had
     * remaining.
     */
    timeleft = alarm( 0 );
    printf( "Time left before cancel, and rearm: %d\n",
        timeleft );

    /*
     * Start a new timer that kicks us when timeleft
     * seconds have passed.
     */

    alarm( timeleft );
    signal( SIGALRM, MyHandler );   /* set own handler */

    /*
     * Wait until we receive the SIGALRM signal; any
     * signal kills us, though, since we don't have
     * a signal handler.
     */
    printf( "Hanging around, waiting to die\n" );
    pause();

    //signal( SIGALRM, MyHandler );   /* set own handler */
    signal( SIGABRT, SIG_DFL );    /* Default action */
    signal( SIGFPE, SIG_IGN );     /* Ignore condition */
    return EXIT_SUCCESS;
  }
