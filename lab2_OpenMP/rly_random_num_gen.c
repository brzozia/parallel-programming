#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

int main(int argc, char ** argv){

    long int size= 243138329; // max size
    if(argc>=2){
        size = atoi(argv[1]);
    }

    int *numbers = malloc(size*sizeof(int));
    if(numbers == NULL){
        printf("Cannot allocate so much memory.");
        return 1;
    }

    long int i;
    int pid, thr, tim = time(NULL);
    unsigned int seed;

    double start = omp_get_wtime(); // + for 
    #pragma omp parallel shared(numbers, tim, size) private(i, pid, thr, seed)
    {
        thr = omp_get_num_threads();
        pid = omp_get_thread_num();
        seed = pid + tim;
        i = 0;

       for(i=pid; i<size; i+=thr){
        numbers[i] = rand_r(&seed);
       }
    }
    double end = omp_get_wtime();

    printf("time: %f \n", end-start);
    free(numbers);
}