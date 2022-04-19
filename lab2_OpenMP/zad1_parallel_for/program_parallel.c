#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
int main(int argc, char** argv){

//Deklaracja zmiennej mierzącej czas
double time0;
double time_end;

//Odczytanie rozmiaru, utworzenie tablicy, utworzenie ziarna
int SIZE = atoi(argv[1]);
int* a;
a = (int*)malloc(SIZE * sizeof(int));

//Deklaracja iteratorów
int i;
int j;
int n_threads;

//Pętla licząca kilka przykładów
for (j = 0; j<9; j++){
    time0 = omp_get_wtime();
    #pragma omp parallel shared(SIZE, a, n_threads) private(i)
    {
        unsigned int seed = omp_get_thread_num()+(unsigned int)time(NULL);
        
            //Pętla iterująca po tablicy i przypisująca losowy int do elementu tablicy
            #pragma omp for schedule(runtime)
            for (i = 0; i<SIZE; i++){
                a[i] = rand_r(&seed);
            }
        n_threads = omp_get_num_threads();
    }
    time_end = omp_get_wtime();
    printf("%f,", time_end-time0);
}

printf("%d,", n_threads);
printf("%d \n", SIZE);
free(a);
return 0;
}