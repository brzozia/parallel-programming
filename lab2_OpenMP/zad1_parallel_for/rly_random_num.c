#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

int main(int argc, char ** argv){

    long long int size=1600000000000;
    if(argc>=2){
        size = atoi(argv[1]);
    }

    int *numbers = malloc(size*sizeof(int));
    long long int i;
    int pid, thr;
    
    double start = omp_get_wtime(); // + for + generator
    #pragma omp parallel shared(numbers, size) private(i, pid, thr)
    {
        thr = omp_get_num_threads();
        pid = omp_get_thread_num();
        i = 0;
	printf("xd");
        struct random_data *buf = malloc(sizeof(struct random_data));
	printf("ak");
        srandom_r(time(NULL)+pid, buf);
	printf("aa");
       for(i=pid; i<size; i+=thr){
           random_r(buf, &numbers[i]);
       }
    }
    double end = omp_get_wtime();

    printf("time: %f \n", end-start);
    free(numbers);
}
