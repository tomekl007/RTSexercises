#include <stdio.h>
#include <pthread.h>

#define PLACES 4

pthread_mutex_t mutexWriter = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexReader = PTHREAD_MUTEX_INITIALIZER;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

pthread_mutex_t condReader = PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

int condition = 0;
int count = 0;
int put = 0;
int get = 0;

int readerCount;

int ReadingRoom[PLACES];

//void *reader(void *arg)
int reader(void)
{
	int threadNr = pthread_self();
	printf("create reader : %d \n",  threadNr);
   while( 1 )
   {
      pthread_mutex_lock( &mutex );
      while( condition == 0 )
         pthread_cond_wait( &cond, &mutex );
     // if(BUFFER[get] != 0 ){
    //  ++readerCount;

      		// if I am the first reader tell all others
      		// that the database is being read
      		//if (readerCount == 1){
      			//pthread_mutex_lock( &condReader );
      			printf("reader %d read\n",  threadNr );
      		//}
//
      //		pthread_mutex_unlock( &condReader );
      //		--readerCount;


    //  }
      //printf("conusme \n");

      condition = 0;
      pthread_cond_signal( &cond );

      pthread_mutex_unlock( &mutex );
   }

   return( 0 );
}

void*  writer( void * arg )
{
   while( 1 )
   {
      pthread_mutex_lock( &mutex );
      while( condition == 1 )
        pthread_cond_wait( &cond, &mutex );

     // if(put < 8){
    //  count++;
    //  printf( "Produced %d\n", count );
    ////  BUFFER[put] = count;//umieszczam element w bufferze, na pozycji get
     // put++;
      //get = put;

     // }
     // printf("produce\n ");


      printf("writer %d write\n",  pthread_self() );
      condition = 1;
      pthread_cond_signal( &cond );
      pthread_mutex_unlock( &mutex );
   }
   return( 0 );
}


pthread_t tid[5];

int main( void )
{
	int i = 0;
   pthread_create( NULL, NULL, &writer, NULL );
   for(i=0; i < PLACES ; i++){         //utworzenie wątków


  		      pthread_create(&tid[i],NULL,reader, NULL );


  		     }

}
