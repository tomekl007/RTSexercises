#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

sig_atomic_t signal_count ;


void MyHandler( int sig_number )
  {


	//printf("in myHandler");
     ++signal_count;

  }

int main()
  {

	char haslo[10];
	haslo[0] = 0;

     for(;;){
    	  printf( "Set the alarm \n" );
    	  alarm( 5 );
    	  signal( SIGALRM, MyHandler );   /* set own handler */
    	  if(signal_count == 0){
    	  printf("podaj haslo : ");
    	  }else{
    		  printf("ponow probe :");
    	  }
    	  scanf("%s",haslo);
    	  if(haslo[0] != 0){
    		  printf("podales haslo!");
    		  alarm(0);
    		  break;
    	  }



      }
  }
