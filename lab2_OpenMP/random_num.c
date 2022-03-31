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
    int i, pid;
    #pragma omp parallel shared(numbers, size) private(i, pid)
    {
        pid = omp_get_thread_num();
        printf("my pid: %d", pid);
        i =0;
       for(i=pid; i<size; i+=4){
           numbers[i] = pid;
       }
    }
    double end = omp_get_wtime();

    printf("time: %f \n", end-start);
    int j;
    for(j=0;j<size;j++){
        printf("%d ", numbers[i]);
    }
    free(numbers);
}