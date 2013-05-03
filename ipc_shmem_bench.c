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

pthread_mutex_t mutex1;
pthread_mutex_t mutex2;

void* passer(void* x)
{
	printf("this is the passer thread\n");
	pthread_exit(NULL);
}
void main( int argc, char **argv) {
  pthread_t thread;
  int rc;
  int t=1;
  rc=pthread_create(&thread,NULL,passer,(void*)t);
  printf("up and running\n");
  pthread_exit(NULL);
}
