/*
Jeremy Bernstein
CS 481 Spring 2013
Lab 4 - OS Benchmarking
*/

#include <time.h>

typedef struct {
  int iterations;
  
} Conf;

typedef struct {
  long long unsigned startTime;
  long long unsigned endTime;

  long long unsigned totalDelta;
  long long unsigned avDelta;

} Stat;
