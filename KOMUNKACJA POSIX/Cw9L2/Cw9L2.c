  #include <unistd.h>
  #include <fcntl.h>
  #include <stdio.h>
  #include <stdlib.h>

  int main( void )
{
	int flags, retval;
     int fd;

     fd=open("myFile.txt",O_RDONLY);

     flags = fcntl( fd, F_GETFL,O_NONBLOCK);
    flags |= O_NONBLOCK;

     retval = fcntl( fd, F_SETFL, flags );
     if( retval == -1 ) {
         printf( "error setting flags\n" );
         return EXIT_FAILURE;
     }

     printf("setting succesful\n");
     return EXIT_SUCCESS;
 }
