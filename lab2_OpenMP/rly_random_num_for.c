#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

int main(int argc, char ** argv){

    int size = 243138329; // max size
    if(argc>=2){
        size = atoi(argv[1]);
    }

    int *numbers = malloc(size*sizeof(int));
    if(numbers == NULL){
        printf("Cannot allocate so much memory.");
        return 1;
    }

    int i, thr;
    int tim = time(NULL);
    unsigned int seed;

    double start = omp_get_wtime(); 
    #pragma omp parallel shared(numbers, tim, size, thr) private(i, seed)
    {
        seed = omp_get_thread_num() + tim;

        #pragma omp for schedule(static)
       for(i=0; i<size; i++){
            numbers[i] = rand_r(&seed);
       }

      thr = omp_get_num_threads();
    }
     
    double end = omp_get_wtime();

    if(argc>2){
      FILE *fd;
      fd = fopen(argv[2], "a" );

      fprintf(fd,"%d; %d, %f;\n", thr, size, end-start);
      fclose(fd);
    }
    free(numbers);
}


