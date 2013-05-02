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

// Tiny struct to hold pthread input args
typedef struct{
  Stat* stat;
  long long unsigned* startF;
} Args;

// Tiny function to time thread creation
void *threadBench(Args* args){
//  args->stat->totalDelta+= timeNanoSec(&timer) - *(args->startF);
  pthread_exit(NULL);
}

// Benchmark for Thread Creation
void createThreadBench(Stat* stat){
  stat->startTime = timeNanoSec(&timer);
  
  int i;
  for (i = 0; i < conf.iterations; i++){
    Args args;
    args.stat = stat;

    long long unsigned startF = timeNanoSec(&timer);
    pthread_t thread;
    void *threadBench(Args* args);
    args.startF = &startF;

    pthread_create(&thread, NULL, threadBench, &args);
    stat->totalDelta += timeNanoSec(&timer) - startF;
    pthread_join(thread, NULL);
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
//      stat->totalDelta += timeNanoSec(&timer) - startF;
        exit(0);
    } else {
      // parent goes down this path (original process)
      stat->totalDelta += timeNanoSec(&timer) - startF;
//      exit(0);
      int wc = wait(NULL);
    }
  }

  stat->endTime = timeNanoSec(&timer);
}

// Benchmark for Context Switching Processes
void switchProcessBench(Stat* stat){
  stat->startTime = timeNanoSec(&timer);

  int pipeA[2];
  int pipeB[2];
  int ping = 1;

  if (pipe(pipeA)==-1) {
    perror("Pipe pipeA failed.\n");
    return;
  }
  if (pipe(pipeB)==-1) {
    perror("Pipe pipeB failed.\n");
    return;
  }
  
  int rc = fork();
  if (rc < 0) {
    // fork failed; exit
    fprintf(stderr, "fork failed\n");
    exit(1);
  } else if (rc == 0) {
    // child (new process)
    while( ping == 1){
      read(pipeA[0], &ping, sizeof(ping));
      write(pipeB[1], &ping, sizeof(ping));
    }
    exit(0);
    
  } else {
    // parent goes down this path (original process)
    int count = 0;
    while (count < conf.iterations){
      write(pipeA[1], &ping, sizeof(ping));
      read(pipeB[0], &ping, sizeof(ping));
      count++;
    }
    ping = -1;
    write(pipeA[1], &ping, sizeof(ping));
    wait(NULL);
  }
 
  stat->endTime = timeNanoSec(&timer);
}



void main( int argc, char **argv) {
  conf.iterations = 10000000;

  int test = 0;
  if (argc == 2){
    test = atoi(argv[1]);
  }
 
  switch(test) {
    case 0: // RUN ALL
     
    case 1:
      ;// Testing non-System function calls
      Stat* nonSysFuncStat = malloc(sizeof(Stat));
      nonSysFuncStat-> totalDelta = 0;
      char* nonSysFuncName = "Non-System Function Benchmark";
      nonSysFuncStat->testName = nonSysFuncName;
      nonSysFuncBench(nonSysFuncStat);
      printStats(nonSysFuncStat, &conf);
      free(nonSysFuncStat);
      if ( test > 0 ){ break; }

    case 2:
      ;// Testing System function calls
      Stat* sysFuncStat = malloc(sizeof(Stat));
      sysFuncStat->totalDelta = 0;
      char* sysFuncName = "System Function Benchmark";
      sysFuncStat->testName = sysFuncName;
      sysFuncBench(sysFuncStat);
      printStats(sysFuncStat, &conf);
      free(sysFuncStat);
      if ( test > 0 ){ break; }

    case 3:
      conf.iterations = 10000;
      ;// Testing process creation
      Stat* createThreadStat = malloc(sizeof(Stat));
      createThreadStat-> totalDelta = 0;
      char* createThreadName = "Thread Creation Benchmark";
      createThreadStat->testName = createThreadName;
      createThreadBench(createThreadStat);
      printStats(createThreadStat, &conf);
      free(createThreadStat);
      if ( test > 0 ){ break; }

    case 4:
      conf.iterations = 10000;
      ;// Testing process creation
      Stat* createProcessStat = malloc(sizeof(Stat));
      createProcessStat->totalDelta = 0;
      char* createProcessName = "Process Creation Benchmark";
      createProcessStat->testName = createProcessName;
      createProcessBench(createProcessStat);
      printStats(createProcessStat, &conf);
      free(createProcessStat);
      if ( test > 0 ){ break; }

    case 5:
      conf.iterations = 100000;
      ;// Testing process context switching
      Stat* switchProcessStat = malloc(sizeof(Stat));
      switchProcessStat->totalDelta = 0;
      char* switchProcessName = "Process Context Switch Benchmark";
      switchProcessStat->testName = switchProcessName;
      switchProcessBench(switchProcessStat);
      printStats(switchProcessStat, &conf);
      free(switchProcessStat);
      if ( test > 0 ){ break; }
  }

}
