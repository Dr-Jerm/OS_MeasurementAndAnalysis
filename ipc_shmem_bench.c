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
pthread_cond_t passerLock;
char buffer[1024];
int i=10000;
int count=0;

void* passer(void* x)
{
	int i;
    for(i=0;i<10000;i++)  {
		pthread_mutex_lock(&lock);
		while(count==0)  {
			pthread_cond_wait(&mainSig);
		}
    }
	pthread_exit(NULL);
}
void main( int argc, char **argv) {
  buffer[1023]='\0':
  pthread_t thread;
  int rc;
  int t=1;
  rc=pthread_create(&thread,NULL,passer,(void*)t);

  int i;
  for(i=0;i<10000;i++)  {

  }


  printf("up and running\n");
  pthread_exit(NULL);
}
