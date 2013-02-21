//EDF Earliest due first

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#define NUMBER_OF_THREADS 7


pthread_t tid[NUMBER_OF_THREADS];        //Tablica z numerami TID wykladow
int czasyWatkow[NUMBER_OF_THREADS];
int threadsPriority[NUMBER_OF_THREADS];


void *dummy(void *arg)
 {

	sleep(60);

 }

int main(int argc, char *argv[]) {

	char odp = 'n';
	char no = 'n';
	char yes = 't';
	int counter = 0;

    int czasWzgledny = 0;
    int czasBezwzgledny = 0;




	//while(1){
    // int i = 0;

     for(counter = 0; counter < 8; counter++){
		//printf("czy chcesz utworzyc nowy watek ? [t/n]\n");

		//scanf("%c", &odp);
		//odp = getchar();
		//printf("odpowedz to %c\n", odp);

		//if( odp == yes ){

			//printf("cos");
			printf("za jaki czas watek ma sie zakonczyc[w sekundach]:  \n");
		    scanf("%d", &czasWzgledny);

			printf("czas podany %d\n",czasWzgledny);
			//time_t temp = czasWzgledny + 24 * 60 * 60;

			//time_t time_of_day;

		   // time_of_day = time( NULL );
		   // printf("%s", &time_of_day);

			//printf( "It is now: %s", ctime( &time_of_day ) );


           // czasBezwzgledny = time_of_day + czasWzgledny;

			 pthread_attr_t attr;
			 pthread_attr_init( &attr );
             pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);

             czasBezwzgledny = czasWzgledny + 0;
            //pthread_attr_t attr;
            sched_param_t par;

           // pthread_attr_init(&attr);
          /*  int i = 0;
            int count = 0;
            for(i = 0; i < NUMBER_OF_THREADS; i++){
            	if ( (czasyWatkow[i] < czasBezwzgledny) &&(czasyWatkow[i] != 0) ){
            		count++;
            	}


            }




            printf("po przejsciu petli czasow zatrzymal sie na : %d\n",count);
            int priorytet = count + 1;

            threadsPriority[counter] = priorytet;*/

           // par.sched_priority = priorytet;

           // pthread_attr_setdetachstate( &attr, PTHREAD_CREATE_DETACHED );

           // setschedparam(0, 1, *par);

			pthread_create(&tid[counter], &attr, dummy, NULL );
			int cpid = tid[counter];
			printf("stworzylem watek %d\n", cpid);

			int j = 0;
			int i = 0;
			int count = 0;
			     for(j = 0; j < NUMBER_OF_THREADS+1; j++){

			    	// if(tid[j] == 0){
			    	//	 printf("tid[%d] == 0\n",j);
			    	//	 break;
			    	// }

			          //for thread "0"
			    	 int k = 0;
			    	 for(k = 0; k < NUMBER_OF_THREADS+1; k++ ){
			    		 count = 0;
			          for(i = 0; i < NUMBER_OF_THREADS+1; i++){
			              if ( (czasyWatkow[k] > czasyWatkow[i]) &&(czasyWatkow[i] != 0) ){
			                    count++;
			              }


			           }
			         // printf("dla tid[%d] count = %d\n",k , count);
			          int priorytet = count + 1;
			          threadsPriority[k] = priorytet;
			    	 }
			        // printf("dla tid[%d] count = %d\n",j , count);

			         //int priorytet = count + 1;
                    // threadsPriority[j] = priorytet;
                     par.sched_priority = threadsPriority[j];
                     pthread_setschedparam(tid[j], 1, &par);

			      }


			//par.sched_priority = priorytet;



			//pthread_create();

		    pthread_attr_destroy(&attr);
			czasyWatkow[counter] = czasBezwzgledny;

			//counter++;



			//time + czas podany daje termin bezwzgledy
			//utwroz watek
			//odp = 't';

		}

     for(counter = 0; counter < 8; counter++){
    	 printf("Watek %d, deadline: %d, priority : %d\n",
    			 tid[counter], czasyWatkow[counter], threadsPriority[counter]);

     }


	//	else{
	//		printf("nie chcesz dale kreowac watkow");
	//		break;

	//	}


     }

	//}
	//return EXIT_SUCCESS;
//}
