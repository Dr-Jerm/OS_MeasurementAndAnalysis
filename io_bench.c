/*
Jeremy Bernstein and Travis Price
CS 481 Spring 2013
Lab 4 - OS Benchmarking
*/

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include "stats.h"
////////////
// Part 1 //
////////////


void main( int argc, char **argv) {
  
  struct stat fstats;
  long long int filesize;
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
  map = mmap(0, filesize, PROT_READ, MAP_SHARED, handle, 0);	

  while(i<=10000)  {
	printf("up and running\n");
	buffer=(char*)malloc(i*sizeof(char));

	while(fread(buffer,1,i,infile));

	rewind(infile);	

	while(read(handle,buffer,i));

	lseek(handle,0,SEEK_SET);

 	memcpy(buffer,map,i);
	printf("%s",buffer);
	
	i=i*10;
	free(buffer);
  }
  printf("\n\nloop exited\n\n");
  if(munmap(map,filesize)==-1)  {
		printf("error un-mapping file\n");
  }
  close(handle);
  fclose(infile);
}
