/*
Jeremy Bernstein and Travis Price
CS 481 Spring 2013
Lab 4 - OS Benchmarking
*/

#include <stdio.h>
#include <pthread.h>
#include "stats.h"
////////////
// Part 1 //
////////////

pthread_mutex_t lock;
pthread_cond_t mainSig;
pthread_cond_t passerSig;
char buffer[1024];
int i=10000;
int mainfull=0;
int passerfull=0;
char* tempbuff;

void* passer(void* x)
{
	int i;
    for(i=0;i<10000000;i++)  {
		pthread_mutex_lock(&lock);
		while(mainfull==0)  {
			pthread_cond_wait(&mainSig,&lock);
		}
		mainfull=0;
		pthread_mutex_unlock(&lock);
		
		char string[9];
		int n=0;
		tempbuff=buffer;

		while(n<1024)
		{
			sscanf(tempbuff,"%8s",string);
			n+=8;
			tempbuff+=8;
		}
	
		pthread_mutex_lock(&lock);
		passerfull=1;
		pthread_cond_signal(&passerSig);
		pthread_mutex_unlock(&lock);
    }
	pthread_exit(NULL);
}
void main( int argc, char **argv) {
  buffer[1023]='\0';
  pthread_t thread;

  int i;
  for(i=0;i<1023;i++)  {
		buffer[i]='a';
  }

  int rc;
  int t=1;
  rc=pthread_create(&thread,NULL,passer,(void*)t);

  for(i=0;i<10000000;i++)  {
	pthread_mutex_lock(&lock);
	mainfull=1;
	pthread_cond_signal(&mainSig);
	pthread_mutex_unlock(&lock);

	char string[9];
	int n=0;
	tempbuff=buffer;

	while(n<1024)
	{
		sscanf(tempbuff,"%8s",string);
		n+=8;
		tempbuff+=8;
	}

	pthread_mutex_lock(&lock);
		while(passerfull==0)  {
			pthread_cond_wait(&passerSig,&lock);
		}
		passerfull=0;
		pthread_mutex_unlock(&lock);
  }

  pthread_exit(NULL);
}
