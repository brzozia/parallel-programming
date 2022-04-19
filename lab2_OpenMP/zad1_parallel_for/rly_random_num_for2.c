#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

int main(int argc, char ** argv){

    int size = 243138329; // max size
    if(argc>=2){
        size = atoi(argv[1]);
    }

    double *numbers = malloc(size*sizeof(double));
    if(numbers == NULL){
        printf("Cannot allocate so much memory.");
        return 1;
    }

    int i, thr;
    // int tim = time(NULL);
    // unsigned int seed;
    unsigned short xi[3];
    xi[0] = time(NULL);
    xi[1] = time(NULL)+13;
    xi[2] = time(NULL)+27;

    double start = omp_get_wtime(); 
    #pragma omp parallel shared(numbers, size, xi) private(i)
    {
        // seed = omp_get_thread_num() + tim;

        #pragma omp for schedule(static)
       for(i=0; i<size; i++){
            numbers[i] = erand48(xi);
       }

      thr = omp_get_num_threads();
    }
     
    double end = omp_get_wtime();

    if(argc>2){
      FILE *fd;
      fd = fopen(argv[2], "a" );

      fprintf(fd,"%d; %d; %f;\n", thr, size, end-start);
      fclose(fd);
    }
    free(numbers);
}


