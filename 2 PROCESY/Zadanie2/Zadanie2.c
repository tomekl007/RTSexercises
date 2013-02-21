#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
	int rc;

	    rc = system( "ls" );
	    if( rc == -1 ) {
	      printf( "shell could not be run\n" );
	    } else {
	      printf( "result of running command is %d\n",
	          WEXITSTATUS( rc ) );
	      //Zwraca kodpowrotu przekazany przez funkcję exit() z procesu potomnego
	    }
	    return EXIT_SUCCESS;
}
