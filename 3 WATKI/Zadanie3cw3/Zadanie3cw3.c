#include <stdio.h>
#include <pthread.h>

#define ROZMIAR 8

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

int condition = 0;
int count = 0;
int put = 0;
int get = 0;

int BUFFER[ROZMIAR];

int consume( void )
{
   while( 1 )
   {
      pthread_mutex_lock( &mutex );
      while( condition == 0 )
         pthread_cond_wait( &cond, &mutex );
      if(BUFFER[get] != 0 ){
    	  if(get == 0 )
    		  printf("get  == 0 ! \n");
    	int temp = BUFFER[get];
    	printf("consumed %d", temp);
    	get++;
    	if(get == 7 ){
    		printf("get  == 7 ! \n");
    	}

      }
      //printf("conusme \n");

      condition = 0;
      pthread_cond_signal( &cond );
      pthread_mutex_unlock( &mutex );
   }

   return( 0 );
}

void*  produce( void * arg )
{
   while( 1 )
   {
      pthread_mutex_lock( &mutex );
      while( condition == 1 )
         pthread_cond_wait( &cond, &mutex );

      if(put < 8){
      count++;
      printf( "Produced %d\n", count );
      BUFFER[put] = count;//umieszczam element w bufferze, na pozycji get
      put++;
      //get = put;

      }
     // printf("produce\n ");
      condition = 1;
      pthread_cond_signal( &cond );
      pthread_mutex_unlock( &mutex );
   }
   return( 0 );
}

int main( void )
{
   pthread_create( NULL, NULL, &produce, NULL );
   return consume();
   //return consume();
}
