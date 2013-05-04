/*
Jeremy Bernstein and Travis Price
CS 481 Spring 2013
Lab 4 - OS Benchmarking
*/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#include <sys/syscall.h>
#include <time.h>

#include <sys/mman.h>
#include <sys/types.h>
#include <math.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <sched.h>

#include "util.c"
#include "stats.h"
////////////
// Part 2 //
////////////

typedef struct {
  unsigned readSize;
  long long unsigned startTime;
  long long unsigned endTime;
  struct ioStats* next;
} ioStats;

struct timespec timer;

long long unsigned tempTime;

void iostat_print(ioStats* iostats, long long unsigned fsize)  {
	while(iostats)  {
		long long unsigned elapsedTime;
		elapsedTime=iostats->endTime-iostats->startTime;
		long long unsigned nreads;
		ioStats* temp;
		nreads=floor(fsize/(iostats->readSize));
		printf("Size of read: %u \n ",iostats->readSize);
		printf("	Total runtime: %llu  \n",elapsedTime);
		printf("	Number of reads: %lli  \n",nreads);
		printf("	time/read: %f \n ",elapsedTime/(float)nreads);
		printf("	byte/time: %f  \n",fsize/(float)elapsedTime);
		printf("	time/byte: %f  \n",elapsedTime/(float)fsize);
		printf("\n");
		temp=iostats;
		iostats=(ioStats*)iostats->next;
		free(temp);
	}
}

void main( int argc, char **argv) {
  
  
  ioStats* freadStats=malloc(sizeof(ioStats));
  ioStats* readStats=malloc(sizeof(ioStats));
  ioStats* memcpyStats=malloc(sizeof(ioStats));
  ioStats* freadStart=freadStats;
  ioStats* readStart=readStats;
  ioStats* memcpyStart=memcpyStats;
  ioStats* temp;
  

  long long int filesize;
  struct stat fstats;
  if(stat(argv[1],&fstats)==-1)  {
	printf("stat error");
  }
  filesize=fstats.st_size;

  int i=1;

  char* buffer=NULL;

  FILE * infile;
  infile=fopen(argv[1],"r");

  int handle;
  handle=open(argv[1], O_RDONLY);

  char* map;
  char* fmap;
  map = mmap(0, filesize, PROT_READ, MAP_SHARED, handle, 0);	

  //cpu_set_t mask;
  //CPU_ZERO(&mask);
  //CPU_SET(0, &mask);
  //sched_setaffinity(0, sizeof(mask), &mask);
  

  while(i<=10000)  {
    buffer=(char*)malloc(i*sizeof(char));
    freadStats->readSize=i;
    freadStats->startTime=timeNanoSec(&timer);
    while(fread(buffer,1,i,infile));
    freadStats->endTime=timeNanoSec(&timer);
    if(i<10000){
      freadStats->next=malloc(sizeof(ioStats));
      freadStats=freadStats->next;
    }
    freadStats->next=NULL;
    rewind(infile);

    readStats->readSize=i;
    readStats->startTime=timeNanoSec(&timer);
    while(read(handle,buffer,i));
    readStats->endTime=timeNanoSec(&timer);
    if(i<10000){
      readStats->next=malloc(sizeof(ioStats));
      readStats=readStats->next;
    }
    readStats->next=NULL;
    lseek(handle,0,SEEK_SET);

    memcpyStats->readSize=i;
    memcpyStats->startTime=timeNanoSec(&timer);
    int j=0;
    fmap=map;
    for(j=0;j<=filesize;j+=i)  {
      memcpy(buffer,fmap,i);
      fmap=fmap+i;
    }
    memcpyStats->endTime=timeNanoSec(&timer);
    if(i<10000){
      memcpyStats->next=malloc(sizeof(ioStats));
      memcpyStats=memcpyStats->next;
    }
    memcpyStats->next=NULL;

    i=i*10;
    free(buffer);
  }
  if(munmap(map,filesize)==-1)  {
		printf("error un-mapping file\n");
  }

  printf("\nResults for fread()\n\n");
  iostat_print(freadStart,filesize);

  printf("\n\nResults for read()\n\n");
  iostat_print(readStart,filesize);

  printf("\n\nResults for mmap()/memcpy\n\n");
  iostat_print(memcpyStart,filesize);

  close(handle);
  fclose(infile);
}
