/*
**  PROGRAM: Mandelbrot area
**
**  PURPOSE: Program to compute the area of a  Mandelbrot set.
**           Correct answer should be around 1.510659.
**           WARNING: this program may contain errors
**
**  USAGE:   Program runs without input ... just run the executable
**            
**  HISTORY: 
**.          Written:  (Mark Bull, August 2011).
**           Simplified so you can do the exercise with less C experience (Tim Mattson, Nov 2023).
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

# define NPOINTS 1000
# define MAXITER 1000

int testpoint(double, double);

int numoutside = 0;

int main(){
   int i, j;
   double eps  = 1.0e-5;
   double cimag, creal;


//   Loop over grid of points in the complex plane which contains the Mandelbrot set,
//   testing each point to see whether it is inside or outside the set.

#pragma omp parallel for private(j) reduction(+:numoutside)
   for (i=0; i<NPOINTS; i++) {
     for (j=0; j<NPOINTS; j++) {
       creal = -2.0+2.5*(double)(i)/(double)(NPOINTS)+eps;
       cimag = 1.125*(double)(j)/(double)(NPOINTS)+eps;
       numoutside += testpoint(creal, cimag);
     }
   }

// Calculate area of set and error estimate and output the results
   
  double area=2.0*2.5*1.125*(double)(NPOINTS*NPOINTS-numoutside)/(double)(NPOINTS*NPOINTS);
  double error=area/(double)NPOINTS;

  printf("Area of Mandlebrot set = %12.8f +/- %12.8f\n",area,error);
  printf("Correct answer should be around 1.510659\n");

}

int testpoint(double creal, double cimag){

// iterate z=z*z+c, until |z| > 2 when point is known to be outside set
// If loop count reaches MAXITER, point is considered to be inside the set

       double zreal, zimag, temp;
       int iter;
       zreal = creal;  zimag = cimag;

       for (iter=0; iter<MAXITER; iter++){
         temp = (zreal*zreal)-(zimag*zimag)+creal;
         zimag = zreal*zimag*2+cimag;
         zreal = temp;
         if ((zreal*zreal+zimag*zimag)>4.0) {
           return 1;
         }
       }
       return 0;
}

