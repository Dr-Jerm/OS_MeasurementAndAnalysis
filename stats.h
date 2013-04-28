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
  printf("Start time : %lluns\n", stat->startTime);
  printf("End time   : %lluns\n", stat->endTime);

  printf("Average time per test: %fns/test\n", stat->totalDelta /(float) conf->iterations);
}

