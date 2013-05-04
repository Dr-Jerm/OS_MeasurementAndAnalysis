/*
Jeremy Bernstein and Travis Price
CS 481 Spring 2013
Lab 4 - OS Benchmarking
*/

#define _GNU_SOURCE

#include <stdio.h>
#include <pthread.h>
#include <sched.h>
#include "stats.h"
#include "util.c"
////////////
// Part 2 //
////////////

pthread_mutex_t lock;
pthread_cond_t mainSig;
pthread_cond_t passerSig;
char buffer[1024];
int i=10000;
int mainfull=0;
int passerfull=0;
char* tempbuff;
struct timespec timer;
long long unsigned startTime;
long long unsigned total_px_time;
long long unsigned total_read_time;


void* passer(void* x)
{
	int i;
	char string[9];
	long long unsigned read_start_time;

	cpu_set_t mask;
    CPU_ZERO(&mask);
    CPU_SET(0, &mask);
	sched_setaffinity(0, sizeof(mask), &mask);

    for(i=0;i<10000000;i++)  {
		pthread_mutex_lock(&lock);
		while(mainfull==0)  {
			pthread_cond_wait(&mainSig,&lock);
		}
		mainfull=0;
		pthread_mutex_unlock(&lock);
		
		
		int n=0;
		tempbuff=buffer;

		
		read_start_time=timeNanoSec(&timer);
		sscanf(tempbuff,"%8s",string);
		n+=8;
		tempbuff+=8;

		while(n<1024)
		{
			sscanf(tempbuff,"%8s",string);
			n+=8;
			tempbuff+=8;
		}
	
		total_read_time+=timeNanoSec(&timer)-read_start_time;
		total_px_time+=read_start_time-startTime;

		pthread_mutex_lock(&lock);
		passerfull=1;
		startTime = timeNanoSec(&timer);
		pthread_cond_signal(&passerSig);
		pthread_mutex_unlock(&lock);
    }
	pthread_exit(NULL);
}
void main( int argc, char **argv) {
  startTime=timeNanoSec(&timer);
  buffer[1023]='\0';
  pthread_t thread;
  long long start_read_time;
  int i;
  fflush(stdout);
  cpu_set_t mask;
  CPU_ZERO(&mask);
  CPU_SET(0, &mask);
  sched_setaffinity(0, sizeof(mask), &mask);

  for(i=0;i<1023;i++)  {
		buffer[i]='a';
  }

  int rc;
  int t=1;
  char string[9];
  rc=pthread_create(&thread,NULL,passer,(void*)t);

  for(i=0;i<10000000;i++)  {
	pthread_mutex_lock(&lock);
	mainfull=1;
    startTime = timeNanoSec(&timer);
	pthread_cond_signal(&mainSig);
	pthread_mutex_unlock(&lock);

	pthread_mutex_lock(&lock);
	while(passerfull==0)  {
		pthread_cond_wait(&passerSig,&lock);
	}
	passerfull=0;
	pthread_mutex_unlock(&lock);

		int n=0;
		
		start_read_time=timeNanoSec(&timer);
		sscanf(tempbuff,"%8s",string);
		n+=8;
		tempbuff+=8;

		while(n<1024)
		{
			sscanf(tempbuff,"%8s",string);
			n+=8;
			tempbuff+=8;
		}
		total_read_time+=timeNanoSec(&timer)-start_read_time;
		total_px_time+=start_read_time-startTime;
		
  }
  printf("total process exchange time %llu\n",(total_px_time/20000000));
  printf("total read time %llu \n",(total_read_time/10000000));
  printf("total runtime %llu \n", timeNanoSec(&timer)-startTime);

  pthread_exit(NULL);
}
