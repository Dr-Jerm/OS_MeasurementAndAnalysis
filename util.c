/*
Jeremy Bernstein
CS 481 Spring 2013
Lab 4 - OS Benchmarking
*/

#include <time.h>

// helperfunction to extract the nanoseconds from timespec
long long unsigned timeNanoSec(struct timespec* time) {
  if (clock_gettime(CLOCK_REALTIME, time)) {
    exit(1);
  }

  long long unsigned ns_time;
  ns_time = ((long long unsigned) time->tv_sec) * 1000000000LLU;
  ns_time = ns_time + (long long unsigned) time->tv_nsec;

  return ns_time;
}
