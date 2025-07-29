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

double compute_sum(int start_i, int end_i) {
  if (end_i - start_i < 50) {
    double sum = 0.0;
    for (int i = start_i; i <= end_i; i++) {
        double x = (i-0.5)*step;
        sum += 4.0/(1.0+x*x);
    }
    return sum;
  }

  int midpoint = (start_i + end_i) / 2;
  double lower_sum, upper_sum;
#pragma omp task shared(lower_sum)
  lower_sum = compute_sum(start_i, midpoint-1);
#pragma omp task shared(upper_sum)
  upper_sum = compute_sum(midpoint, end_i);
#pragma omp taskwait
  return lower_sum + upper_sum;
}

int main (int argc, char* argv[])
{
	  int i;
	  double pi, sum = 0.0;
	  double start_time, run_time;

	  step = 1.0/(double) num_steps;

	  start_time = omp_get_wtime();

#pragma omp parallel private(step)
{
#pragma omp single
    sum = compute_sum(1, num_steps);
}
	  pi = step * sum;
	  run_time = omp_get_wtime() - start_time;
	  printf("pi with %ld steps is %lf in %lf seconds\n",num_steps, pi, run_time);
}	  
