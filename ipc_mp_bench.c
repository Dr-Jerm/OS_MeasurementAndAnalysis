/*
Jeremy Bernstein and Travis Price
CS 481 Spring 2013
Lab 4 - OS Benchmarking
*/
#define _GNU_SOURCE

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/syscall.h>
#include <sched.h>

#include "stats.h"

////////////
// Part 2 //
////////////

struct timespec timer;
long long unsigned startTime;
long long unsigned total_px_time;
long long unsigned total_read_time;

void main( int argc, char **argv) {
  int idp;
  
  int fd0[2];
  int fd1[2];
  char data[1024];
  int i;
  int j;
	
  for(i=0;i<1024;i++)  {
	data[i]='a';
	if(i==1023)  {
		data[i]='b';
	}
  }

  
  if(pipe(fd1)<0)  {
	printf("error creating pipe\n");
  }
  if(pipe(fd0)<0)  {
	printf("error creating pipe\n");
  }

  cpu_set_t mask;
  CPU_ZERO(&mask);
  CPU_SET(0, &mask);
  sched_setaffinity(0, sizeof(mask), &mask);

  idp=fork();
  if(idp<0){
	printf("fork failed\n");
  }  else if(idp==0)  {
	close(fd1[0]);
	close(fd0[1]);
	char string[9];
	string[9];
	long long unsigned read_start;
	long long unsigned read_finsih;
	for(j=0;j<10000000;j++)  {

		read_start=timeNanoSec(&timer);
		for(i=0;i<128;i++)  {
		
			read(fd0[0],string,8);
		}
		write(fd1[1],data,1024);
		read_finish=timeNanoSec(&timer);
	}
	exit(0);
  }  else  {
	close(fd1[1]);
	close(fd0[0]);
	char pstring[9];
	pstring[8]='\0';
	long long unsigned read_start;
	long long unsigned read_finish;
	for(j=0;j<100000;j++)  {
		write(fd0[1],data,1024);
		for(i=0;i<128;i++)  {
			read(fd1[0],pstring,8);
		}

	}
  }
	
 // printf("up and running\n");
}
