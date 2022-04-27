#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

void generate_numbers(unsigned long long *size, int *my_pid, int *array, long *max){
    unsigned int seed = *my_pid+(unsigned int)time(NULL);
    unsigned short randstate[3];
    // fgetc() returns a `char`, we need to fill a `short`
    randstate[0] = *my_pid+(unsigned int)time(NULL);
    randstate[1] = *my_pid+(unsigned int)time(NULL)-17;
    randstate[2] = *my_pid+(unsigned int)time(NULL)-21;
    int i;
    printf(" %d %d", *max, seed);
    //Random numbers generating
    #pragma omp for schedule(static)
    for (i = 0; i<(*size); i++){
        int xd = rand_r(&seed);
        // long xd = nrand48(randstate);
        // printf(" %d ", xd);
        array[i] = xd%(*max);
        // array[i] = xd;
    }
}

int main(int argc, char **argv){
    unsigned long long size = 1100000000;
    long maxi = RAND_MAX;
    if(argc>=2){
        size = atoll(argv[1]);
    }
    if(argc>=3){
        maxi = atoi(argv[2]);
    }
    printf(" %d ", maxi);
    int* array = malloc(size * sizeof(int));

    if(array==NULL){
        FILE *fd;
        fd = fopen("test_rand11.txt", "a" );
        fprintf(fd,"nok %llu;\n", size);
        fclose(fd);
        exit(1);
    }

    #pragma omp parallel shared(array, maxi) 
    {   
        int my_pid = omp_get_thread_num();
        generate_numbers(&size, &my_pid, array, &maxi);
    }

    FILE *fd;
    fd = fopen("test_rand11.txt", "a" );
    fprintf(fd,"%lld;\n%lld;\n", size, maxi);
    int i;
    for(i=0;i<size;i++){
        fprintf(fd,"%d;\n", array[i]);
        printf("%d;", array[i]);
    }
    fclose(fd);

    return 0;
}