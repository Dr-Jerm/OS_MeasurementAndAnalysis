/*
Jeremy Bernstein and Travis Price
CS 481 Spring 2013
Lab 4 - OS Benchmarking
*/

#include <time.h>

typedef struct {
  int iterations;
  
} Conf;

typedef struct {
  char* testName;
  long long unsigned startTime;
  long long unsigned endTime;

  long long unsigned totalDelta;
  long long unsigned avDelta;

} Stat;

void printStats(Stat* stat, Conf* conf) {
  printf("\n");
  printf("Test: %s\n---------------------\n", stat->testName); 

  printf("Iterations : %d\n", conf->iterations);
  printf("Start time : %llu ns\n", stat->startTime);
  printf("End time   : %llu ns\n", stat->endTime);
//  printf("Total Time : %lluns\n", stat->endTime - stat->startTime);
  printf("Test Time  : %llu ns\n", stat->totalDelta);

  printf("Average time per test: %f ns/test\n", stat->totalDelta /(float) conf->iterations);
}

