#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#ifdef _OPENMP
 #include <omp.h>
#endif
int *iteration;
double f(double x);    
double Trap(double a, double b, int n, int thread_count);
double TrapDynamic(double a, double b, int n, int thread_count);
double TrapGuided(double a, double b, int n, int thread_count);
double TrapRunTime(double a, double b, int n, int thread_count);
int main(int argc, char* argv[]) {
   double  global_result = 0.0; 
   double  global_result2 = 0.0; 
   double  global_result3 = 0.0; 
   double  global_result4 = 0.0; 
   
   double  a, b;                 
   int     n;                    
   int     thread_count;

   thread_count = strtol(argv[1], NULL, 10);
   printf("Enter a, b, and n\n");
   scanf("%lf %lf %d", &a, &b, &n);


   global_result = Trap(a, b, n, thread_count);
   global_result2 = TrapDynamic(a, b, n, thread_count);
   global_result3 = TrapGuided(a, b, n, thread_count);
   global_result4 = TrapRunTime(a, b, n, thread_count);

   printf("With n = %d trapezoids, our estimate(static)\n", n);
   printf("of the integral from %f to %f = %.14e\n",
      a, b, global_result);

   printf("With n = %d trapezoids, our estimate (dynamic)\n", n);
   printf("of the integral from %f to %f = %.14e\n",
      a, b, global_result2);

   printf("With n = %d trapezoids, our estimate (guided)\n", n);
   printf("of the integral from %f to %f = %.14e\n",
      a, b, global_result3);
   
   printf("With n = %d trapezoids, our estimate (runtime)\n", n);
   printf("of the integral from %f to %f = %.14e\n",
      a, b, global_result4);

   int loop;


   for(loop = 0; loop < n; loop++)
      printf("iteracion %d ", iteration[loop]);

   return 0;
   
   return 0;
}  


double f(double x) {
   double return_val;

   return_val = x*x*x*x;
   return return_val;
}  

double Trap(double a, double b, int n, int thread_count) {
   double  h, approx;
   int  i;
  
   double start =omp_get_wtime();
   h = (b-a)/n; 
   approx = (f(a) + f(b))/2.0;

   # pragma omp parallel for schedule(static,6) default(none) \
	shared(a,h,n) private(i) reduction(+:approx) num_threads(thread_count)
   for (i = 1; i <= n-1; i++)
     approx += f(a + i*h);
  //   iteration[i] = approx; 
   approx = h*approx; 
   double stop = omp_get_wtime();
   double time = stop - start;
   printf("Ellapsed Time is:%f\n", time);
   return approx;


} 
double TrapDynamic(double a, double b, int n, int thread_count) {
   double  h, approx;
   int  i;
  
   double start =omp_get_wtime();
   h = (b-a)/n; 
   approx = (f(a) + f(b))/2.0;

   # pragma omp parallel for schedule(dynamic,3) default(none) \
	shared(a,h,n) private(i) reduction(+:approx) num_threads(thread_count)
   for (i = 1; i <= n-1; i++)
     approx += f(a + i*h);
    // iteration[i] = approx; 
   approx = h*approx; 

   double stop = omp_get_wtime();
   double time = stop - start;
   printf("Ellapsed Time is Dynamic:%f\n", time);
   
   return approx;
}   
double TrapGuided(double a, double b, int n, int thread_count) {
   double  h, approx;
   int  i;
  
   double start =omp_get_wtime();
   h = (b-a)/n; 
   approx = (f(a) + f(b))/2.0;

   # pragma omp parallel for schedule(guided,7) default(none) \
	shared(a,h,n) private(i) reduction(+:approx) num_threads(thread_count)
   for (i = 1; i <= n-1; i++)
     approx += f(a + i*h);
    // iteration[i] = approx; 
   approx = h*approx; 

   double stop = omp_get_wtime();
   double time = stop - start;
   printf("Ellapsed Time is Dynamic:%f\n", time);
   return approx;
}   
double TrapRunTime(double a, double b, int n, int thread_count) {
   double  h, approx;
   int  i;
  
   double start =omp_get_wtime();
   h = (b-a)/n; 
   approx = (f(a) + f(b))/2.0;

   # pragma omp parallel for schedule(runtime) default(none) \
	shared(a,h,n) private(i) reduction(+:approx) num_threads(thread_count)
   for (i = 1; i <= n-1; i++)
     approx += f(a + i*h);
   approx = h*approx; 

   double stop = omp_get_wtime();
   double time = stop - start;
   printf("Ellapsed Time is Runtime:%f\n", time);
   return 
   approx;
} 