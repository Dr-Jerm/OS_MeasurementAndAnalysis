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
  
  int i;
  for (i = 0; i < conf.iterations; i++){
    long long unsigned startF = timeNanoSec(&timer);
    tinyFunc();
    stat->totalDelta += timeNanoSec(&timer) - startF;
  }

  stat->endTime = timeNanoSec(&timer);
  printStats(stat, &conf);
}

void sysFuncBench(Stat* stat){

}

void main( int argc, char **argv) {
  conf.iterations = 1000000;
  
  // Testing non-System function calls
  Stat nonSysFuncStat;
  char* nonSysFuncName = "Non-System Function Benchmark";
  nonSysFuncStat.testName = nonSysFuncName;
  nonSysFuncBench(&nonSysFuncStat);

  // Testing System function calls
  Stat sysFuncStat;
  char* sysFuncName = "Non-System Function Benchmark";
  sysFuncStat.testName = sysFuncName;
  sysFuncBench(&sysFuncStat);
}
