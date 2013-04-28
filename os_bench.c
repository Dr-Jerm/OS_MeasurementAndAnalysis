/*
Jeremy Bernstein and Travis Price
CS 481 Spring 2013
Lab 4 - OS Benchmarking
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <time.h>
#include <unistd.h>

#include "stats.h"
#include "util.c"
////////////
// Part 1 //
////////////

// Global Variables:

Conf conf;
struct timespec timer;

// Tiny function to test non-System Function call
int tinyFunc(){
  int i = 1;
  return i;
}

// Benchmark for non-System Function calls
void nonSysFuncBench(Stat* stat){
  stat->startTime = timeNanoSec(&timer);
  printf("startTime: %lluns\n", stat->startTime);
}


void main( int argc, char **argv) {
  conf.iterations = 1000000;

  Stat nonSysFuncStat;
  nonSysFuncBench(&nonSysFuncStat);
  printf("up and running\n");
}
