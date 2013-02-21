#include <stdio.h>
#include <sys/neutrino.h>
#include <sys/syspage.h>

struct sigevent event;
volatile unsigned counter;




const struct sigevent *handler( void *area, int id ) {
    // Wake up the thread every 100th interrupt
    if ( ++counter == 100 ) {
        counter = 0;
        return( &event );
        }
    else
        return( NULL );
    }

int main() {



    int i;
    int id;

    // Request I/O privileges
    ThreadCtl( _NTO_TCTL_IO, 0 );

    // Initialize event structure
    event.sigev_notify = SIGEV_INTR;

    printf("%d", SYSPAGE_ENTRY(qtime)->intr);
    // Attach ISR vector
    id=InterruptAttach( SYSPAGE_ENTRY(qtime)->intr, &handler,
                        NULL, 0, 0 );

    for( i = 0; i < 10; ++i ) {
        // Wait for ISR to wake us up
        InterruptWait( 0, NULL );

        printf( "100 events, event : %d\n",event.sigev_notify );
        }

    // Disconnect the ISR handler
    InterruptDetach(id);
    return 0;
    }
