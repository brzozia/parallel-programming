#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
int main(int argc, char** argv){

double time0;
long SIZE = atoi(argv[1]);
int* a;
a = (int*)malloc(190000 * sizeof(int));
srand(7);

int* i;
int j;
printf("\n");
for (j = 0; j<9; j++){
time0 = omp_get_wtime();
#pragma omp parallel for shared(SIZE, a) private(i) schedule(runtime)
for (i = a[0]; i<SIZE+a[0]; i++){
*i = omp_get_thread_num();
}
printf("%f,", omp_get_wtime()-time0);
}

int ilosc_watkow;
#pragma omp parallel shared(ilosc_watkow, SIZE)
ilosc_watkow = omp_get_num_threads();
printf("%d,", ilosc_watkow);
printf("%ld", SIZE);
return 0;
 }
