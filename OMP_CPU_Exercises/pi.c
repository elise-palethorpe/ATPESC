/*

This program will numerically compute the integral of

                  4/(1+x*x) 
				  
from 0 to 1.  The value of this integral is pi -- which 
is great since it gives us an easy way to check the answer.

The is the original sequential program.  It uses the timer
from the OpenMP runtime library

History: Written by Tim Mattson, 11/99.

*/
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
static long num_steps = 100000000;
double step;
int main (int argc, char* argv[])
{
    int nthreads = atoi(argv[1]);
	  int i;
	  double x, pi, sum = 0.0;
	  double start_time, run_time;

	  step = 1.0/(double) num_steps;

    omp_set_num_threads(nthreads);
	  start_time = omp_get_wtime();

    double sums[nthreads];

#pragma omp parallel 
  {
    int thread_id = omp_get_thread_num();
    int num_threads = omp_get_num_threads();
    // race condition
    if (thread_id == 0) nthreads = num_threads;
    sums[thread_id] = 0;
    for (i = thread_id + 1; i <= num_steps; i+=nthreads) {
        x = (i-0.5)*step;
        sums[thread_id] += 4.0/(1.0+x*x);
    }
  }
    for (i = 0; i < nthreads; i++) {
        sum += sums[i];
    }

	  pi = step * sum;
	  run_time = omp_get_wtime() - start_time;
	  printf("pi with %ld steps and %d threads is %lf in %lf seconds\n",num_steps, nthreads, pi, run_time);
}	  





