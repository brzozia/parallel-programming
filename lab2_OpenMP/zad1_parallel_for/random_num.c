#include <stdio.h>
#include <stdlib.h>
#include <omp.h>


int main(int argc, char ** argv){

    int size=1600000000;
    // if(argc>=2){
    //     size = 10;
    // }

    int *numbers = (int*)malloc(size*sizeof(int));
    int i, pid, thr;
    
    double start = omp_get_wtime();
    #pragma omp parallel shared(numbers, size) private(i, pid, thr)
    {
        thr = omp_get_num_threads();
        pid = omp_get_thread_num();
        i = 0;
       for(i=pid; i<size; i+=thr){
           //printf("my pid: %d", pid);
           numbers[i] = pid;
       }
    }
    double end = omp_get_wtime();

    printf("time: %f \n", end-start);
    //int j;
  //  for(j=0;j<size;j++){
    //    printf("%d ", numbers[j]);
   // }
    free(numbers);
}
