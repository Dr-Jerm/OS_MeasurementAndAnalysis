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
#include <limits.h>

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
}

// Benchmark for System Function calls
void sysFuncBench(Stat* stat){
  stat->startTime = timeNanoSec(&timer);
  
  int i;
  for (i = 0; i < conf.iterations; i++){
    long long unsigned startF = timeNanoSec(&timer);
    syscall(SYS_gettid); // does the system call for system ID
    stat->totalDelta += timeNanoSec(&timer) - startF;
  }

  stat->endTime = timeNanoSec(&timer);
}

// Benchmark for Process Creation
void createProcessBench(Stat* stat){
  stat->startTime = timeNanoSec(&timer);
  
  int i;
  for (i = 0; i < conf.iterations; i++){
    long long unsigned startF = timeNanoSec(&timer);
    
    int rc = fork();
    if (rc < 0) {
      // fork failed; exit
      fprintf(stderr, "fork failed\n");
      exit(1);
    } else if (rc == 0) {
      // child (new process)
      stat->totalDelta += timeNanoSec(&timer) - startF;
    } else {
      // parent goes down this path (original process)
      exit(0);
    }
//    stat->totalDelta += timeNanoSec(&timer) - startF;
  }

  stat->endTime = timeNanoSec(&timer);
}


void main( int argc, char **argv) {
  conf.iterations = 10000000;
  
  // Testing non-System function calls
  Stat* nonSysFuncStat = malloc(sizeof(Stat));;
  char* nonSysFuncName = "Non-System Function Benchmark";
  nonSysFuncStat->testName = nonSysFuncName;
  nonSysFuncBench(nonSysFuncStat);
  printStats(nonSysFuncStat, &conf);
  free(nonSysFuncStat);

  // Testing System function calls
  Stat* sysFuncStat = malloc(sizeof(Stat));
  char* sysFuncName = "System Function Benchmark";
  sysFuncStat->testName = sysFuncName;
  sysFuncBench(sysFuncStat);
  printStats(sysFuncStat, &conf);
  free(sysFuncStat);

  conf.iterations = 1000;
  // Testing process creation
  Stat* createProcessStat = malloc(sizeof(Stat));;
  char* createProcessName = "Process Creation Benchmark";
  createProcessStat->testName = createProcessName;
  createProcessBench(createProcessStat);
  printStats(createProcessStat, &conf);
  free(createProcessStat);
}
