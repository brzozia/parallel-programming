#include <stdio.h>
#include <stdlib.h>
#include <omp.h>


int main(int argc, char ** argv){

    int size=16;
    // if(argc>=2){
    //     size = 10;
    // }

    int *numbers = (int*)malloc(size*sizeof(int));

    double start = omp_get_wtime();
    #pragma omp parallel
    {
        int pid = omp_get_thread_num();
       for(int i=pid; i<size; i+=4){
           numbers[i] = pid;
       }
    }
    double end = omp_get_wtime();

    printf("time: %f", end-start);
    for(int i=0;i<size;i++){
        printf("%d ", numbers[i]);
    }
    free(numbers);
}