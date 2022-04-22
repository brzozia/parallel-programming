#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

typedef struct node{
    int data;
    struct node * next;
};

void print_bucket(struct node * bucket){   
    while(bucket != NULL){
        printf("%d \n", bucket->data);
        bucket = bucket->next;
    }
    printf("\n");
}

void generate_numbers(int size, int *array, int max){
    int i;
    // #pragma omp parallel shared(size, max, array) private(i)
    // {
        unsigned int seed = omp_get_thread_num()+(unsigned int)time(NULL);
        
        //Random numbers generating
        // #pragma omp for schedule(runtime)
        for (i = 0; i<size; i++){
            array[i] = rand_r(&seed)%max;
        }
    // }
}

void separate_numbers(int size, int *array, int range, int min, int buckets, struct node *buckets_array){
    int i;
    for(i=0; i<size; i++){
        int bucket_id = (array[i]-min)/range;
        if(bucket_id >= buckets){
            bucket_id--;
        }
        
        struct node *head = malloc(sizeof(struct node));
        head->data = array[i];
        head->next = buckets_array[bucket_id].next;
        buckets_array[bucket_id].next = head;
        
        // print_bucket(&buckets_array[bucket_id]);
    }
}

void sort_buckets(int buckets, struct node *buckets_array){
    int i;
    for(i=0; i<buckets; i++){
        // printf("sort %d \n", i);
        // print_bucket(buckets_array[i].next);
        struct node *ptr = buckets_array[i].next;
        
        while(ptr != NULL && ptr->next != NULL){

            if(ptr->data > ptr->next->data){
                struct node *next_move = buckets_array[i].next;
                struct node *prev_move = &buckets_array[i];

                while(ptr->next->data > next_move->data){
                    next_move = next_move->next;
                    prev_move = prev_move->next;
                }

                struct node *move = ptr->next;
                ptr->next = move->next;
                prev_move->next = move;
                move->next = next_move;
            }
            else{
                ptr = ptr -> next;
            }
        }
        // print_bucket(buckets_array[i].next);
    }
}

void concat_numbers(int buckets, struct node * buckets_array, int *array){
    int i, j = 0;
    for(i=0; i<buckets; i++){
        struct node *bucket = buckets_array[i].next;

        while(bucket != NULL){
            array[j++] = bucket->data;
            bucket = bucket->next;
        }
    }
}

// args: SIZE, MAX_VALUE, PREFFERED_ELEMENTS_IN_BUCKET, RESULTS_FILENAME
int main(int argc, char** argv){
    double time_generate, time_separate, time_sort, time_concat, time_all, time_allocate, time_deallocate;
    time_all = omp_get_wtime();
    
    int size = 10;
    int max = RAND_MAX;
    int mean_elements_in_bucket = 3;
    if(argc>=2){
        size = atoi(argv[1]);
    }
    if(argc>=3){
        max = atoi(argv[2]);
    }
    if(argc>=4){
        mean_elements_in_bucket = atoi(argv[3]);
    }
    int* array  = malloc(size * sizeof(int));
    int i;

    time_generate = omp_get_wtime();
    generate_numbers(size, array, max);
    time_generate = omp_get_wtime() - time_generate;

    // int min = max;
    // for (i = 0; i<size; i++){
    //     if(array[i]<min){
    //         min = array[i];
    //     }
    //     printf("%d ",array[i]);
    // }
    int min = 0;

    int buckets = ceil((float)size/mean_elements_in_bucket);
    int range = ceil((float)(max-min)/buckets);
    printf("\n Range of numbers in array %d:%d \nElements to sort: %d \n", min,max,size);
    printf("Number of buckets: %d \nWhole range of numbers is divided into parts of about: %d elements\n", buckets,range);

    // Array containing all buckets
    time_allocate = omp_get_wtime();
    struct node* buckets_array = malloc(buckets*sizeof(struct node));
    if(buckets_array == NULL){
        printf("Error while allocating memory for buckets array");
    }

    // Allocate buckets with guardian
    for(i=0; i<buckets; i++){
        buckets_array[i].data = -1;
        buckets_array[i].next = NULL;
    }
    time_allocate = omp_get_wtime() - time_allocate;


    // Put generated numbers in buckets
    time_separate = omp_get_wtime();
    separate_numbers(size, array, range, min, buckets, buckets_array);
    time_separate = omp_get_wtime() - time_separate;


   // Sort buckets - in-place instertion sort
   time_sort = omp_get_wtime();
   sort_buckets(buckets, buckets_array);
   time_sort = omp_get_wtime() - time_sort;


    // Write numbers to array
    time_concat = omp_get_wtime();
    concat_numbers(buckets, buckets_array, array);
    time_concat = omp_get_wtime() - time_concat;
    
     // Print results
    // for(i=0;i<size;i++){
    //     printf("%d ", array[i]);
    // }

    time_deallocate = omp_get_wtime();
    free(buckets_array);
    free(array);
    time_deallocate = omp_get_wtime() - time_deallocate;

   
    time_all = omp_get_wtime()-time_all;
    
    printf("Time generate: %lf\n Time allocate: %lf\n Time separate: %lf\n Time sort: %lf\n Time concat buckets: %lf\n Time deallocate %lf\n Time all: %lf", time_generate, time_allocate, time_separate, time_sort, time_concat, time_deallocate, time_all);

    // Write to file
    if(argc>=5){
        FILE *fd;
        fd = fopen(argv[4], "a" );
        fprintf(fd,"%d;%d;%d;%d;%lf;%lf;%lf;%lf;%lf;%lf;%lf;\n", min, max, size, buckets, time_generate, time_allocate, time_separate, time_sort, time_concat, time_deallocate, time_all);
        fclose(fd);
    }

    // Free memory
    

    return 0;
}
