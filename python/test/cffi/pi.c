/* filename: pi.c*/
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

/* Returns a very crude approximation of Pi
   given a int: a number of iteration */
float pi_approx(int n){

  double i,x,y,sum=0;

  for(i=0;i<n;i++){

    x=rand();
    y=rand();

    if (sqrt(x*x+y*y) < sqrt((double)RAND_MAX*RAND_MAX))
      sum++; }

   float ret = 4*(float)sum/(float)n;
  
  printf("%.3f = pi_approx(%d)\n", ret, n);

  return ret; }