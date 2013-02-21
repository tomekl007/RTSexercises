#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

int jasnosc;

void* ustawJasnosc(void* arg){

printf("Nowy watek\n");
FILE* in1;
in1 = fopen("/sys/class/leds/pandaboard::status1/brightness","wb");
if(!in1){
printf("blad pliku");}
else{
 printf("otwarlem");
}

while(1)
{
   

fwrite("1",1,1,in1);
fflush(in1);
usleep(100*jasnosc);


fwrite("0",1,1,in1);
fflush(in1);
usleep(100*(10-jasnosc));

}

fclose(in1);
return 0;
}


int main(void){


pthread_attr_t attr;

int pid;

printf ("podaj jasnosc [0-10]\n");
scanf("%d",&jasnosc);

pthread_t t;

pthread_attr_init(&attr);
pid = pthread_create(&t,&attr,&ustawJasnosc,NULL);
printf("%d\n",pid);
while(1)
{
    printf("podaj jasnosc [0-10]\n");
    scanf("%d",&jasnosc);
}
return 0;
}

	
Klik