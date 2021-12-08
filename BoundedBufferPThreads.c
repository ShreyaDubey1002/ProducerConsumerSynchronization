#include <stdlib.h> 
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <sys/types.h>
#include <semaphore.h>
#include "buffer.h"
#define true 1

int in=0,out=0,countOfBuffer=0;
unsigned int sleep_time;
pthread_mutex_t mutex;

sem_t semaphore_full, semaphore_empty;

pthread_t thread1;      
pthread_attr_t attr;
void initialisation();
int insertion_function(BufferItem element);
void *consumer(void *param) ;
int deletion_function(BufferItem *element); 
void *producer(void *param);


typedef struct thdata
{
  int no;
} thdata;


void initialisation()
{
   countOfBuffer = 0;

   pthread_mutex_init(&mutex, NULL);

   sem_init(&semaphore_full, 0, 0);

   sem_init(&semaphore_empty, 0, BUFFERSIZE);

   pthread_attr_init(&attr);

   

}
BufferItem BUFFER[BUFFERSIZE];

int insertion_function(BufferItem element) {

/* insert item into buffer
return 0 if successful, otherwise
return -1 indicating an error condition */


if(countOfBuffer>=BUFFERSIZE)
{
return -1;
}

else
{

BUFFER[countOfBuffer]=element;
countOfBuffer=countOfBuffer+1;
return 0;
}


}


void *consumer(void *param) 

{
BufferItem element_consumed;

while (true)
{
                  
sleep(sleep_time);

sem_wait(&semaphore_full);
      
pthread_mutex_lock(&mutex);

if (deletion_function(&element_consumed)==-1)
fprintf(stderr,"Reporting error condition in consumer process\n");

else

printf("consumer consumed the item : %d\n",element_consumed);

pthread_mutex_unlock(&mutex);
      
sem_post(&semaphore_empty);


}


}





int deletion_function(BufferItem *element) 
{

/* remove an object from buffer
placing it in item
return 0 if successful, otherwise
return -1 indicating an error condition */

if(countOfBuffer<=0)
{
return -1;
}

else
{
*element=BUFFER[(countOfBuffer-1)%BUFFERSIZE];
countOfBuffer=countOfBuffer-1;
return 0;
}



}


void *producer(void *param)
{
BufferItem element_produced;
while (true) 
{

/* sleep for a random period of time */

sleep(sleep_time);

/* generate a random number */
element_produced = rand();


sem_wait(&semaphore_empty);
      
pthread_mutex_lock(&mutex);

if (insertion_function(element_produced)==-1)
fprintf(stderr,"Reporting error condition in producer process\n");

else

printf("Producer produced the item :  %d\n",element_produced);


pthread_mutex_unlock(&mutex);
     
sem_post(&semaphore_full);

}
}




int main(int argc , char* argv[])
{

int i,len1=strlen(argv[1]),flag=0;
for(i=0;i<len1;i++)
if(!isdigit(argv[1][i]))
{
flag=1;
break;
}

if (flag==0)
{
len1=strlen(argv[2]);
for(i=0;i<len1;i++)
if(!isdigit(argv[2][i]))
{
flag=1;
break;
}

}


if (flag==0)

{
len1=strlen(argv[3]);
for(i=0;i<len1;i++)
if(!isdigit(argv[3][i]))
{
flag=1;
break;
}
}

if(argc!=4||flag==1)
{
printf("Invalid command line arguments !!\n");
}

else
{

int producer_threads,consumer_threads;
sleep_time=(unsigned int)atoi(argv[1]);
producer_threads=atoi(argv[2]);
consumer_threads=atoi(argv[3]);
printf("Initializing buffer...\n");

initialisation();

printf("Buffer initialized...\n");

printf("Creating producer thread(s) and consumer thread(s)...\n");


int i;
    for(i=0;i<producer_threads;i++)
     pthread_create(&thread1,&attr,producer,NULL);
    
    for(i=0;i<consumer_threads;i++)
     pthread_create(&thread1,&attr,consumer,NULL);

printf("Producer thread(s) and consumer thread(s) created successfully...\n");


printf("Main sleep starts...\n");


        sleep(sleep_time); 

printf("Main wakes up...\n");
	
printf(" Exit !! \n");


}
}
