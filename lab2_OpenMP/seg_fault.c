#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

int main(int argc, char ** argv){

    unsigned long int size=4294967295;
    if(argc>=2){
        size = atoi(argv[1]);
    }
    int *numbers = NULL;
    while (numbers == NULL){
        numbers = malloc(size*sizeof(int));
        size -= 100;
    }
    printf("size: %ld\r\n", size);
    long long int i;
    int pid, thr;
    
    double start = omp_get_wtime(); // + for + generator
    #pragma omp parallel shared(numbers, size) private(i, pid, thr)
    {
        thr = omp_get_num_threads();
        pid = omp_get_thread_num();
        i = pid;
        for(i; i<size; i+=thr){
           numbers[i] = pid;
       }
    }

    printf("%ld\n", size);
    double end = omp_get_wtime();

    printf("time: %f \n", end-start);
    free(numbers);

}