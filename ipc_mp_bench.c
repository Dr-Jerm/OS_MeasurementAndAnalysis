/*
Jeremy Bernstein and Travis Price
CS 481 Spring 2013
Lab 4 - OS Benchmarking
*/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include "stats.h"
////////////
// Part 1 //
////////////

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

  idp=fork();
  if(idp<0){
	printf("fork failed\n");
  }  else if(idp==0)  {
	close(fd1[0]);
	close(fd0[1]);
	char string[9];
	string[9];
	for(j=0;j<100000;j++)  {

		for(i=0;i<128;i++)  {
			read(fd0[0],string,8);
		}
		write(fd1[1],data,1024);
	}
	exit(0);
  }  else  {
	close(fd1[1]);
	close(fd0[0]);
	char pstring[9];
	pstring[8]='\0';
	
	for(j=0;j<100000;j++)  {
		write(fd0[1],data,1024);
		for(i=0;i<128;i++)  {
			read(fd1[0],pstring,8);
		}

	}
  }
	
 // printf("up and running\n");
}
