#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>


pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;


// calculates a^b mod m
int power_modulo_fast(int a, int b, int m)
{
int i;
int result = 1;
long int x = a%m;

for (i=1; i<=b; i<<=1)
{
x %= m;
if ((b&i) != 0)
{
result *= x;
result %= m;
}
x *= x;
}

return result%m;
}

//Fermat test
//retrun: 1 - probably prime
//        0 - not prime
int Fermat(int n, int k)
{
int a, i;
srand(time(NULL));

if (n<4)
{
return 1;
}

for (i=0; i<k; i++)
{
a = 2+(int) ((n-2)*rand()/(RAND_MAX+1.0));
if (power_modulo_fast(a, n-1, n) != 1)
{
return 0;
}
}

return 1;
}

pthread_t tid[5];        //Tablica z numerami TID wątków

typedef struct {
   int pocz;    // poczatek zakresu
   int kon;      // koniec zakresu
   int numer;  // numer watku
} par_t;


void *findPrimes(void *arg)
 {
     par_t *bounds =  (par_t*)arg;
     //int pocz = (int)bounds->pocz;

     bounds->numer = pthread_self();

     printf( "This is thread %d\n", bounds->numer );


     int i = 0;

     int zd = (int)bounds->pocz;
     int zg = (int)bounds->kon;
     printf("in findPrimes zd : %d, zg : %d\n", zd, zg );

    // printf("zd : %d zg : %d ",zd, zg);

     int arrayLength = zg - zd;

     int primeNumbers[arrayLength];
     int input[arrayLength];

     for(; zd < zg; zd++  ){
     		input[i] = zd;
     		i++;
     		//printf("zd : %d\n", zd);

     	}


     int n = 0;
     	int k = 1;

     	int j = 0;

     	for(i = 0; i < zg; i++){

     	n = input[i];
     		if (Fermat(n, k) == 1)
     		{
     			primeNumbers[j] = n;
     			 pthread_mutex_lock( &mutex );
     			 j++;
     			 pthread_mutex_unlock( &mutex );

     			}

     		}

     	int counter = 0;
     		for(i = 0; i < j - 1; i++){
    		//	printf("pierwsze: %d\n", primeNumbers[i]);
    			if(primeNumbers[i] > 0)
     			counter++;
     		}



     	//for(i = 0; i < j - 1; i++){
     	//	printf("pierwsze: %d\n", primeNumbers[i]);
     	//}

     return (void*) counter;        //lub pthread_exit((void)*numer);
 }






int main(int argc, char *argv[]) {

	int numThreads = atoi(argv[3]) ;


	int zd = atoi(argv[1]);
	int zg = atoi(argv[2]);


	int arrayLength = zg - zd;

	par_t *pBounds = malloc(sizeof(par_t) );


    //par_t bounds;
	//pBounds->pocz = zd;
	//pBounds->kon = zg;
	//pBounds->numer = 1;

	//printf("%d      %d\n", pBounds->pocz, pBounds->kon);


//	int primeNumbers[arrayLength];
	//int input[arrayLength];


	//int numberOfRanges = arrayLength / numThreads;
	//printf("number of ranges : %d", numberOfRanges);
	/*int count = 0;
	int temp2 = 0;
	par_t* ranges[numThreads]; //=  malloc(sizeof(par_t[numThreads]) );;
	ranges[0] = malloc(sizeof(par_t) );
	ranges[1] = malloc(sizeof(par_t) );

	for(count = 0; count < numThreads; count++){

		 ranges[count]->pocz = zd;
		 // temp = pBounds->kon;
		 ranges[count]->kon = (int)(zg/numThreads) + temp2;
	     temp2 = ranges[count]->kon;
	     zd = ranges[count]->kon;
	    // printf("ranges : %d - %d\n", ranges[i]->pocz, ranges[i]->kon);

	}

	for(count = 0 ; count < numThreads; count++){
		printf("ranges : %d - %d\n", ranges[count]->pocz, ranges[count]->kon);
	}*/



	//void* - wsk nieznanego typu, trzeba go pozniej castowac
		int status;
		int temp = 0;
		int i = 0;
		     for(i=0; i < numThreads ; i++){         //utworzenie wątków

		    	 pBounds->pocz = zd;
		        // temp = pBounds->kon;
		         pBounds->kon = (int)(zg/numThreads) + temp;
		         temp = pBounds->kon;
		         zd = pBounds->kon;
		       //  printf("before create thread zd : %d--- \n", (int)(pBounds->pocz));
		         printf("ranges : %d - %d\n", pBounds->pocz, pBounds->kon);

		              pthread_create(&tid[i],NULL,findPrimes,(void*) pBounds );
		    // for(i=0;i<NUM_THREADS;i++) {        //oczekiwanie na zakończenie
		      //        pthread_join(tid[i],(void*)&status);
		        //      printf("watek %d zakonczony, kod powrotu %d\n",tid[i],status);
		        //  }
		     }
		    // return 0;


		     for(i=0; i < numThreads ; i++) {        //oczekiwanie na zakończenie
		                 pthread_join(tid[i],(void*)&status);
		                 printf("watek %d zakonczony, w zadanym przedziale znalazl %d liczb pierwszych\n",tid[i],status);
		             }
		        return 0;




	/*for(; zd < zg; zd++  ){
		input[i] = zd;
		i++;
		printf("zd : %d\n", zd);

	}
	int n = 0;
	int k = 1;

	int j = 0;

	for(i = 0; i < zg; i++){

	n = input[i];
		if (Fermat(n, k) == 1)
		{
			primeNumbers[j] = n;
            j++;
		}

	}

	for(i = 0; i < j - 1; i++){
		printf("pierwsze: %d\n", primeNumbers[i]);
	}


*/






}
