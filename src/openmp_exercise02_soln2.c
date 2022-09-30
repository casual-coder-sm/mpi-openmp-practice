#include <omp.h>
#include <stdio.h>

static long num_steps = 100000;
double step;
#define PAD 8 //assumes 64 byte L1 cache line size
#define NUM_THREADS 4

int main()
{
    int i, nthreads;
    double pi, sum[NUM_THREADS][PAD];
    double exec_start_time, exec_time;

    exec_start_time = omp_get_wtime();
    
    step = 1.0 / (double)num_steps;
    omp_set_num_threads(NUM_THREADS);    
#pragma omp parallel
    {
        int i, id, nthrds;
        double x;
        
        nthrds = omp_get_num_threads();
        id = omp_get_thread_num();
        if(id == 0)
            nthreads = nthrds;

        for (i = id, sum[id][0]=0.0; i < num_steps; i+=nthrds)
        {
            x = (i + 0.5) * step;
            sum[id][0] += 4.0 / (1.0 + x * x);
        }
    }
    for(i=0, pi=0.0; i < nthreads; ++i)
    {
        pi += sum[i][0] * step;
    }    
    exec_time = omp_get_wtime() - exec_start_time;
    printf("Executiont Time =%f\n", exec_time);
    printf("Sum = %f\n", pi);
}