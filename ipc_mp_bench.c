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
#include "util.c"
#include "stats.h"

////////////
// Part 2 //
////////////

struct timespec timer;
long long unsigned startTime;
long long unsigned total_px_time;
long long unsigned total_read_time;
long long unsigned finishTime;

void main( int argc, char **argv) {
  startTime=timeNanoSec(&timer);
  int idp;
  int fd0[2];
  int fd1[2];
  long long unsigned data[128];
  int i;
  int j;

  
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

	long long unsigned placeholder;
	long long unsigned read_start;
	long long unsigned read_finsih;
	for(j=0;j<100000;j++)  {

		read(fd0[0],&placeholder,8);
		data[0]=timeNanoSec(&timer);//take the time after the first read operationg
		for(i=0;i<127;i++)  {
			read(fd0[0],&placeholder,8);
		}
		data[127]=timeNanoSec(&timer);
		write(fd1[1],data,1024);
		
	}
	exit(0);
  }  else  {
	close(fd1[1]);
	close(fd0[0]);
	char pstring[9];
	pstring[8]='\0';
	long long unsigned write_start;
	long long unsigned read_start;
	long long unsigned child_start;
	long long unsigned child_finish;
	long long unsigned read_finish;
	for(j=0;j<100000;j++)  {
		write_start=timeNanoSec(&timer);
		write(fd0[1],data,1024);
		read(fd1[0],&child_start,8);
		read_start=timeNanoSec(&timer);
		for(i=0;i<127;i++)  {
			read(fd1[0],&child_finish,8);
		}
		read_finish=timeNanoSec(&timer);
		total_read_time+=child_finish-child_start;
		total_read_time+=read_finish-read_start;
	
		total_px_time+=child_start-write_start;
		total_px_time+=read_start-child_finish;
	}
  }
  printf("Total runtime: %llu\n",timeNanoSec(&timer)-startTime);
  printf("Total time striding buffer: %llu\n",total_read_time);
  printf("Average time striding buffer: %llu\n",total_read_time/200000);
  printf("Average process exchange time: %llu\n",total_px_time/200000);
  
}
